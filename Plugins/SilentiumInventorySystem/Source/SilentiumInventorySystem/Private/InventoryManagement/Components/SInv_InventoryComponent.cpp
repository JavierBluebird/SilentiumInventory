// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Widgets/Inventory/InventoryBase/SInv_InventoryBase.h"

USInv_InventoryComponent::USInv_InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

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
		Server_AddStacksToItem(ItemComponent,Result.TotalRoomToFill, Result.Remainder);
	}
	else if (Result.TotalRoomToFill > 0) 
	{
		// This item type doesn't exist in the inventory. Create a new one and update all pertinent slots.
		Server_AddNewItem(ItemComponent, Result.bStackable ? Result.TotalRoomToFill : 0);
	}
}

void USInv_InventoryComponent::Server_AddNewItem_Implementation(USInv_ItemComponent* ItemComponent, int32 StackCount)
{
	
}

void USInv_InventoryComponent::Server_AddStacksToItem_Implementation(USInv_ItemComponent* ItemComponent,
	int32 StackCount, int32 Remainder)
{
	
}
