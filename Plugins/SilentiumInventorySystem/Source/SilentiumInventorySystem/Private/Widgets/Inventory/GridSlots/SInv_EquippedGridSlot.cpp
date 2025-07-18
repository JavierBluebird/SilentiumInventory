// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/GridSlots/SInv_EquippedGridSlot.h"

#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Widgets/Inventory/HoverItem/SInv_HoverItem.h"

void USInv_EquippedGridSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!IsAvailable()) return;

	USInv_HoverItem* HoverItem = USInv_InventoryStatics::GetHoveredItem(GetOwningPlayer());
	if (!HoverItem) return;

	if (HoverItem->GetItemType().MatchesTag(EquipmentTypeTag)) // Partially matches hierarchy
	{
		SetOccupiedTexture();	
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
	}
}

FReply USInv_EquippedGridSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	EquippedGridSlotClicked.Broadcast(this, EquipmentTypeTag);
	return FReply::Handled();
}
