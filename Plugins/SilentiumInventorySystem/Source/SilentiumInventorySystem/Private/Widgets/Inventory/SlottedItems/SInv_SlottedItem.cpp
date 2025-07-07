// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/SlottedItems/SInv_SlottedItem.h"
#include "Items/SInv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

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
