// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/SlottedItems/SInv_SlottedItem.h"
#include "Items/SInv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"

FReply USInv_SlottedItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnSlottedItemClicked.Broadcast(GridIndex,InMouseEvent);
	return FReply::Handled();
}

void USInv_SlottedItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	USInv_InventoryStatics::ItemHovered(GetOwningPlayer(),InventoryItem.Get());
}

void USInv_SlottedItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	USInv_InventoryStatics::ItemUnhovered(GetOwningPlayer());
}

void USInv_SlottedItem::SetInventoryItem(USInv_InventoryItem* Item)
{
	InventoryItem = Item; 
}

void USInv_SlottedItem::SetImageBrush(const FSlateBrush Brush) const
{
	ImageIcon->SetBrush(Brush);
}

void USInv_SlottedItem::UpdateStackCount(int32 StackCount)
{
	if (StackCount > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(StackCount));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}
