// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/GridSlots/SInv_EquippedGridSlot.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/OverlaySlot.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Fragments/SInv_FragmentTags.h"
#include "Items/Fragments/SInv_ItemFragment.h"
#include "Widgets/Inventory/HoverItem/SInv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/SInv_EquippedSlottedItem.h"

void USInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;

	USInv_HoverItem* HoverItem = USInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag)) // Partially matches hierarchy
	{
		SetOccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USInv_EquippedGridSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;

	USInv_HoverItem* HoverItem = USInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag)) // Partially matches hierarchy
	{
		SetUnoccupiedTexture();
		Image_GrayedOutIcon->SetVisibility(ESlateVisibility::Visible);
	}
}

FReply USInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}

USInv_EquippedSlottedItem* USInv_EquippedGridSlot::OnItemEquipped(USInv_InventoryItem* Item,
	const FGameplayTag& EquipmentTag, float SlotSize)
{
	// Check the Equipment Type Tag
	if (!EquipmentTag.MatchesTag(EquipmentTag)) return nullptr;
	
	// Get Grid Dimensions
	const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(Item, FragmentTags::GridFragment);
	if (!GridFragment) return nullptr;
	const FIntPoint GridDimensions = GridFragment->GetGridSize();

	// Calculate the Draw Size for the Equipped Slotted Item
	const float IconTileWidth = SlotSize - GridFragment->GetGridPadding() * 2;
	const FVector2D DrawSize = GridDimensions * IconTileWidth;

	// Create the Equipped Slotted Item Widget
	EquippedSlottedItem = CreateWidget<USInv_EquippedSlottedItem>(GetOwningPlayer(), EquippedSlottedItemClass);
	
	// Set the Slotted Item's Inventory Item
	EquippedSlottedItem->SetInventoryItem(Item);
	
	// Set the Slotted Item's Equipment Type Tag
	EquippedSlottedItem->SetEquipmentTypeTag(EquipmentTag);
	
	// Hide the Stack Count widget on the Slotted Items
	EquippedSlottedItem->UpdateStackCount(0);
	
	// Set Inventory Item on this class (the Equipped Grid Slot)
	SetInventoryItem(Item);
	
	// Set the Image Brush on the Equipped Slotted Item
	const FSInv_ImageFragment* ImageFragment = GetFragment<FSInv_ImageFragment>(Item, FragmentTags::ImageFragment);
	if (!ImageFragment) return nullptr;

	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetItemIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = DrawSize;
	
	EquippedSlottedItem->SetImageBrush(Brush);
	
	// Add the Slotted Item as a Child to this Widget's Overlay
	Overlay_Root->AddChildToOverlay(EquippedSlottedItem);
	FGeometry OverlayGeometry = Overlay_Root->GetCachedGeometry();
	auto OverlayPos =  OverlayGeometry.Position;
	auto OverlaySize =  OverlayGeometry.Size;

	const float LeftPadding = OverlaySize.X / 2.0f - DrawSize.X / 2.0f;
	const float TopPadding = OverlaySize.Y / 2.0f - DrawSize.Y / 2.0f;

	UOverlaySlot* OverlaySlot = UWidgetLayoutLibrary::SlotAsOverlaySlot(EquippedSlottedItem);
	OverlaySlot->SetPadding(FMargin(LeftPadding, TopPadding));
	
	// Return the Equipped Slotted Item Widget.
	return EquippedSlottedItem;
}
