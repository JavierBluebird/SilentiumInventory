// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/SlottedItems/SInv_EquippedSlottedItem.h"

FReply USInv_EquippedSlottedItem::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnEquippedSlottedItemClicked.Broadcast(this);
	return FReply::Handled();
}
