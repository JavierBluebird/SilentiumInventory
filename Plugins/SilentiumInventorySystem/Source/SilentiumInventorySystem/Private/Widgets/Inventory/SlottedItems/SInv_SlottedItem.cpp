// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/SlottedItems/SInv_SlottedItem.h"
#include "Items/SInv_InventoryItem.h"
#include "Components/Image.h"

void USInv_SlottedItem::SetInventoryItem(USInv_InventoryItem* Item)
{
	InventoryItem = Item; 
}

void USInv_SlottedItem::SetImageBrush(const FSlateBrush Brush) const
{
	ImageIcon->SetBrush(Brush);
}
