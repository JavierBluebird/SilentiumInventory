// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_SpatialInventory.h"

#include "SilentiumInventorySystem.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Items/SInv_InventoryItem.h"
#include "Widgets/Inventory/HoverItem/SInv_HoverItem.h"
#include "Widgets/Inventory/GridSlots/SInv_EquippedGridSlot.h"
#include "Widgets/Inventory/SlottedItems/SInv_EquippedSlottedItem.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Widgets/ItemDescription/SInv_ItemDescription.h"


void USInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// ------------------------------//
	// Button callbacks Binding      //
	// -----------------------------//
	Button_Consumables->OnClicked.AddDynamic(this,&ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this,&ThisClass::ShowCraftables);
	Button_Equippables->OnClicked.AddDynamic(this,&ThisClass::ShowEquippables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);
	
	ShowEquippables();

	// Iterate over every widget child to find the ones we are looking for.
	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		USInv_EquippedGridSlot* EquippedGridSlot = Cast<USInv_EquippedGridSlot>(Widget);
		if (IsValid(EquippedGridSlot))
			{
				EquippedGridSlots.Add(EquippedGridSlot);
				EquippedGridSlot->EquippedGridSlotClicked.AddDynamic(this, &ThisClass::EquippedGridSlotClicked);
			}
	});
}

void USInv_SpatialInventory::EquippedGridSlotClicked(USInv_EquippedGridSlot* EquippedGridSlot,
	const FGameplayTag& EquipmentTypeTag)
{
	// Check to see if we can equip the hover item
	if (!CanEquipHoverItem(EquippedGridSlot, EquipmentTypeTag)) return;

	USInv_HoverItem* HoverItem = GetHoverItem();
	
	// Create an equipped slotted item and add it to the equipped grid slot. (Call EquippedGridSlot->OnItemEquipped)
	const float SlotSize = USInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetSlotSize();
	USInv_EquippedSlottedItem* EquippedSlottedItem = EquippedGridSlot->OnItemEquipped(
			HoverItem->GetInventoryItem(),
			EquipmentTypeTag,
			SlotSize
		);
	EquippedSlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	
	// Clear the Hover Item
	Grid_Equippables->ClearHoveredItem();
	
	// Inform the server that we've equipped an Item (potentially Unequiping an item as well)
	USInv_InventoryComponent* InventoryComponent = USInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));
	
	InventoryComponent->Server_EquipSlotClicked(HoverItem->GetInventoryItem(),nullptr);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(HoverItem->GetInventoryItem());
	}
}

void USInv_SpatialInventory::EquippedSlottedItemClicked(USInv_EquippedSlottedItem* EquippedSlottedItem)
{
	// Remove the Item Description
	USInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());

	if (IsValid(GetHoverItem()) && GetHoverItem()->IsStackable()) return;
	// Get Item to Equip
	USInv_InventoryItem* ItemToEquip = IsValid(GetHoverItem()) ? GetHoverItem()->GetInventoryItem() : nullptr;
	
	// Get Item to Unequip
	USInv_InventoryItem* ItemToUnequip = EquippedSlottedItem->GetInventoryItem();
	
	// Get the Equipped Grid Slot holding this Item.
	USInv_EquippedGridSlot* EquippedGridSlot = FindSlotWithEquippedItem(ItemToUnequip);
	
	// Clear the slot of this item. (Set it's inventory Item to nullptr)
	ClearSlotOfItem(EquippedGridSlot);
	
	// Assign previously Equipped Item as the Hover Item
	Grid_Equippables->AssignHoverItem(ItemToUnequip);
	
	// Remove of the equipped slotted item from the equipped grid slot
	RemoveEquippedSlottedItem(EquippedSlottedItem);
	
	// Make a new equipped slotted item (for the item we held in HoverItem)
	MakeEquippedSlottedItem(EquippedSlottedItem, EquippedGridSlot, ItemToEquip);
	
	// Broadcast delegates for OnItemEquipped / OnItemUnequipped (from the IC)
	BroadcastSlotClickedDelegates(ItemToEquip,ItemToUnequip);
}

USInv_EquippedGridSlot* USInv_SpatialInventory::FindSlotWithEquippedItem(USInv_InventoryItem* EquippedItem) const
{
	auto* FoundEquippedGridSlot = EquippedGridSlots.FindByPredicate([EquippedItem](const USInv_EquippedGridSlot* GridSlot)
	{
		return GridSlot->GetInventoryItem() == EquippedItem;
	});
	return FoundEquippedGridSlot ? *FoundEquippedGridSlot : nullptr;
}
void USInv_SpatialInventory::RemoveEquippedSlottedItem(USInv_EquippedSlottedItem* EquippedSlottedItem)
{
	if (!IsValid(EquippedSlottedItem)) return;

	if (EquippedSlottedItem->OnEquippedSlottedItemClicked.IsAlreadyBound(this, &ThisClass::EquippedSlottedItemClicked))
	{
		EquippedSlottedItem->OnEquippedSlottedItemClicked.RemoveDynamic(this, &ThisClass::EquippedSlottedItemClicked);
	}
	EquippedSlottedItem->RemoveFromParent();
}

void USInv_SpatialInventory::MakeEquippedSlottedItem(USInv_EquippedSlottedItem* EquippedSlottedItem, USInv_EquippedGridSlot* EquippedGridSlot,
	USInv_InventoryItem* ItemToEquip)
{
	if (!IsValid(EquippedGridSlot)) return;

	USInv_EquippedSlottedItem* SlottedItem = EquippedGridSlot->OnItemEquipped(
		ItemToEquip,
		EquippedSlottedItem->GetEquipmentTypeTag(),
		USInv_InventoryStatics::GetInventoryWidget(GetOwningPlayer())->GetSlotSize());
	if (IsValid(SlottedItem)) SlottedItem->OnEquippedSlottedItemClicked.AddDynamic(this,&ThisClass::EquippedSlottedItemClicked);

	EquippedGridSlot->SetEquippedSlottedItem(SlottedItem);
}

void USInv_SpatialInventory::BroadcastSlotClickedDelegates(USInv_InventoryItem* ItemToEquip, USInv_InventoryItem* ItemToUnequip) const
{
	USInv_InventoryComponent* InventoryComponent = USInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	check(IsValid(InventoryComponent));

	InventoryComponent->Server_EquipSlotClicked(ItemToEquip, ItemToUnequip);

	if (GetOwningPlayer()->GetNetMode() != NM_DedicatedServer)
	{
		InventoryComponent->OnItemEquipped.Broadcast(ItemToEquip);
		InventoryComponent->OnItemEquipped.Broadcast(ItemToUnequip);
	}
}

void USInv_SpatialInventory::ClearSlotOfItem(USInv_EquippedGridSlot* EquippedGridSlot)
{
	if (IsValid(EquippedGridSlot))
	{
		EquippedGridSlot->SetEquippedSlottedItem(nullptr);
		EquippedGridSlot->SetInventoryItem(nullptr);
	}
}

bool USInv_SpatialInventory::CanEquipHoverItem(USInv_EquippedGridSlot* EquippedGridSlot,
                                               const FGameplayTag& EquipmentTypeTag) const
{
	if (!IsValid(EquippedGridSlot) || EquippedGridSlot->GetInventoryItem().IsValid()) return false;

	USInv_HoverItem* HoverItem = GetHoverItem();
	if (!IsValid(HoverItem)) return false;

	USInv_InventoryItem* HeldItem = HoverItem->GetInventoryItem();

	return HasHoverItem()
			&& IsValid(HeldItem)
			&& !HoverItem->IsStackable()
			&& HeldItem->GetItemManifest().GetItemCategory() == ESInv_ItemCategory::Equippable
			&&HeldItem->GetItemManifest().GetItemType().MatchesTag(EquipmentTypeTag);
}


FReply USInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
}

void USInv_SpatialInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(ItemDescription)) return;

	SetItemDescriptionSizeAndPosition(ItemDescription,CanvasPanel);
}

void USInv_SpatialInventory::SetItemDescriptionSizeAndPosition(USInv_ItemDescription* Description,
	UCanvasPanel* Canvas) const
{
	UCanvasPanelSlot* ItemDescriptionCPS = UWidgetLayoutLibrary::SlotAsCanvasSlot(Description);

	if (!IsValid(ItemDescriptionCPS)) return;

	const FVector2D ItemDescriptionSize = Description->GetBoxSize();
	ItemDescriptionCPS->SetSize(ItemDescriptionSize);

	FVector2D ClampedPosition = USInv_WidgetUtils::GetClampedWidgetPosition(USInv_WidgetUtils::GetWidgetSize(Canvas),
																			ItemDescriptionSize,
																			UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer()));
	ItemDescriptionCPS->SetPosition(ClampedPosition);
}

FSInv_SlotAvailabilityResult USInv_SpatialInventory::HasRoomForItem(USInv_ItemComponent* ItemComponent) const
{
	switch (USInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
		case ESInv_ItemCategory::Equippable :
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		
		case ESInv_ItemCategory::Consumable :
			return Grid_Consumables->HasRoomForItem(ItemComponent);

		case ESInv_ItemCategory::Craftable :
			return Grid_Craftables->HasRoomForItem(ItemComponent);

		default:
			 // This will be an error, hence we will log it in our custom Log Channel
			UE_LOG(LogSilentiumInventory, Error, TEXT("SInv_SpatialInventory: ItemComponent doesn't have a valid Item Category "));
			return FSInv_SlotAvailabilityResult();
	}
}

void USInv_SpatialInventory::OnItemHovered(USInv_InventoryItem* InventoryItem)
{
	const auto& Manifest = InventoryItem->GetItemManifest();
	
	USInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer); // If it's active, deactivate first.
	
	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this, &Manifest,DescriptionWidget]() // Defines the delegate functionality
		{
			Manifest.AssimilateInventoryFragments(DescriptionWidget);
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	);
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer,  // Actually Starts timer
														  DescriptionTimerDelegate,
														  DescriptionTimerDelay,
														  false);
}

void USInv_SpatialInventory::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

USInv_HoverItem* USInv_SpatialInventory::GetHoverItem() const
{
	if (!ActiveGrid.IsValid()) return nullptr;

	return ActiveGrid->GetHoverItem();
}

float USInv_SpatialInventory::GetSlotSize() const
{
	return Grid_Equippables->GetSlotSize();
}

bool USInv_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;

	return false;
}

USInv_ItemDescription* USInv_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<USInv_ItemDescription>(GetOwningPlayer(),ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
}

void USInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void USInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void USInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}


void USInv_SpatialInventory::SetActiveGrid(USInv_InventoryGrid* Grid, UButton* Button)
{

	if (ActiveGrid.IsValid()) ActiveGrid->HideCursor(); // Hide, then update and show cursor

	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	
	DisableButton(Button);
	Switcher->SetActiveWidget(Grid);
}

void USInv_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

