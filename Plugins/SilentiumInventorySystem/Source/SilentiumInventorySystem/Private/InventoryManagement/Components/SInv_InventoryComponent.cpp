// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SInv_InventoryComponent.h"

#include "Items/Components/SInv_ItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Fragments/SInv_ItemFragment.h"
#include "Runtime/Engine/Internal/VT/VirtualTextureVisualizationData.h"
#include "Widgets/Inventory/InventoryBase/SInv_InventoryBase.h"

USInv_InventoryComponent::USInv_InventoryComponent() : InventoryList(this)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	SetIsReplicatedByDefault(true);
	bReplicateUsingRegisteredSubObjectList = true;
	bInventoryMenuOpen = false;
}

void USInv_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructInventory();
}

void USInv_InventoryComponent::ConstructInventory()
{
	OwningController = Cast<APlayerController>(GetOwner());

	// We must be sure the owner is a Player Controller, and a Local one in order to proceed.
	checkf(OwningController.IsValid(), TEXT("Inventory Component should have a Player Controller as Owner."));
	if (!OwningController->IsLocalController()) return;

	InventoryMenuReference = CreateWidget<USInv_InventoryBase>(OwningController.Get(), InventoryMenuClass);
	InventoryMenuReference->AddToViewport();

	CloseInventoryMenu(); 
}


void USInv_InventoryComponent::Server_DropItem_Implementation(USInv_InventoryItem* Item, int32 StackCount)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - StackCount;
	
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveItemEntry(Item); // Remove from Fast Array
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}
	
	SpawnDroppedItem(Item,StackCount);
}

void USInv_InventoryComponent::SpawnDroppedItem(USInv_InventoryItem* Item, const int32 StackCount)
{
	
	const APawn* OwningPawn = OwningController->GetPawn();
	
	FVector RotatedForward = OwningPawn->GetActorForwardVector();
	RotatedForward = RotatedForward.RotateAngleAxis(FMath::FRandRange(DropSpawnAngleMin,DropSpawnAngleMax), FVector::UpVector);

	FVector SpawnLocation = OwningPawn->GetActorLocation() + RotatedForward * FMath::FRandRange(DropSpawnDistanceMin,DropSpawnDistanceMax);
	SpawnLocation.Z -= RelativeSpawnElevation;
	const FRotator SpawnRotator = FRotator::ZeroRotator;

	FSInv_ItemManifest& ItemManifest = Item->GetItemManifestMutable();
	if (FSInv_StackableFragment* StackableFragment = ItemManifest.GetFragmentOfTypeMutable<FSInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(StackCount);
	}

	ItemManifest.SpawnPickUpActor(this,SpawnLocation,SpawnRotator);
	
}

USInv_InventoryBase* USInv_InventoryComponent::GetInventoryMenu() const
{
	return InventoryMenuReference;
}

void USInv_InventoryComponent::Server_ConsumeItem_Implementation(USInv_InventoryItem* Item)
{
	const int32 NewStackCount = Item->GetTotalStackCount() - 1;
	if (NewStackCount <= 0)
	{
		InventoryList.RemoveItemEntry(Item);
	}
	else
	{
		Item->SetTotalStackCount(NewStackCount);
	}
	// Get the consumable fragment and call Consume()
	if (FSInv_ConsumableFragment* ConsumableFragment = Item->GetItemManifestMutable().GetFragmentOfTypeMutable<FSInv_ConsumableFragment>())
	{
		ConsumableFragment->OnConsume(OwningController.Get());
	}
}

void USInv_InventoryComponent::AddRepSubObj(UObject* SubObj)
{
	if (IsUsingRegisteredSubObjectList() == true && IsReadyForReplication() == true
		&& IsValid(SubObj))
	{
		AddReplicatedSubObject(SubObj);
	}
}

void USInv_InventoryComponent::ToggleInventoryMenu()
{
	if (bInventoryMenuOpen)
	{
		CloseInventoryMenu();
	}
	else
	{
		OpenInventoryMenu();
	}
}

void USInv_InventoryComponent::OpenInventoryMenu()
{
	if (!IsValid(InventoryMenuReference)) return;

	InventoryMenuReference->SetVisibility(ESlateVisibility::Visible);
	bInventoryMenuOpen = true;

	if (!OwningController.IsValid()) return;

	FInputModeGameAndUI InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(true);
}

void USInv_InventoryComponent::CloseInventoryMenu()
{
	if (!IsValid(InventoryMenuReference)) return;

	InventoryMenuReference->SetVisibility(ESlateVisibility::Collapsed);

	bInventoryMenuOpen = false;

	if (!OwningController.IsValid()) return;

	FInputModeGameOnly InputMode;
	OwningController->SetInputMode(InputMode);
	OwningController->SetShowMouseCursor(false);
}

void USInv_InventoryComponent::TryAddItem(USInv_ItemComponent* ItemComponent)
{
	FSInv_SlotAvailabilityResult Result = InventoryMenuReference->HasRoomForItem(ItemComponent);

	// Checks item type and fills RESULT with it
	USInv_InventoryItem* FoundItem = InventoryList.FindFirstItemByType(ItemComponent->GetItemManifest().GetItemType());
	Result.Item = FoundItem;

	// Zero value will be taken as No Room for this Item in Inventory.
	if (Result.TotalRoomToFill == 0)
	{
		NoRoomInInventory.Broadcast();
		return;
	}
	
	if (Result.Item.IsValid() && Result.bStackable == true)
	{
		// Add Stacks to an Item that already exists in the Inventory. We only want to update the stack count,
		// not create a new Item of this type.
		
		OnStackChange.Broadcast(Result);
		Server_AddStacksToItem(ItemComponent,Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0) 
	{
		// This item type doesn't exist in the inventory. Create a new one and update all pertinent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
}

// SERVER RPCS

void USInv_InventoryComponent::Server_AddNewItem_Implementation(USInv_ItemComponent* ItemComponent, int32 StackCount)
{
	USInv_InventoryItem* NewItem = InventoryList.AddItemEntry(ItemComponent); // Adds the item to Inventory Array
	NewItem->SetTotalStackCount(StackCount);
	
	if (GetOwner()->GetNetMode() == NM_ListenServer ||
		GetOwner()->GetNetMode() == NM_Standalone)
	{
		OnItemAdded.Broadcast(NewItem); // Instant broadcast since we are the Server, not the Client. 
	}
	ItemComponent->PickedUp();
}

void USInv_InventoryComponent::Server_AddStacksToItem_Implementation(USInv_ItemComponent* ItemComponent,
	int32 StackCount, int32 Remainder)
{
	const FGameplayTag& ItemType = IsValid(ItemComponent) ? ItemComponent->GetItemManifest().GetItemType() : FGameplayTag::EmptyTag;
	USInv_InventoryItem* Item = InventoryList.FindFirstItemByType(ItemType);
	if (!IsValid(Item)) return;

	Item->SetTotalStackCount(Item->GetTotalStackCount() + StackCount);

	//Destroy the item if the Remainder is zero.
	if (Remainder == 0)
	{
		ItemComponent->PickedUp();
	}
	// Otherwise, update the stack count for the item pickup.
	else if (FSInv_StackableFragment* StackableFragment = ItemComponent->GetItemManifest().GetFragmentOfTypeMutable<FSInv_StackableFragment>())
	{
		StackableFragment->SetStackCount(Remainder);
	}
}

void USInv_InventoryComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, InventoryList);
}

