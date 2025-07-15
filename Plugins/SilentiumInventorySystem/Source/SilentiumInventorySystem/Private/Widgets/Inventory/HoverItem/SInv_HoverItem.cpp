// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/HoverItem/SInv_HoverItem.h"

#include "GameplayTagContainer.h"
#include "Items/SInv_InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USInv_HoverItem::SetImageBrush(const FSlateBrush& Brush) const
{
	Image_Icon->SetBrush(Brush);
}

void USInv_HoverItem::UpdateStackCount(int32 Count)
{
	StackCount = Count;
	if (Count > 0)
	{
		Text_StackCount->SetText(FText::AsNumber(Count));
		Text_StackCount->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

FGameplayTag USInv_HoverItem::GetItemType() const
{
	if (InventoryItem.IsValid())
	{
		return InventoryItem->GetItemManifest().GetItemType();
	}
	return FGameplayTag();
}

void USInv_HoverItem::SetIsStackable(bool bStacks)
{
	bIsStackable = bStacks;
	if (!bStacks)
	{
		Text_StackCount->SetVisibility(ESlateVisibility::Collapsed);
	}
}

USInv_InventoryItem* USInv_HoverItem::GetInventoryItem() const
{
	return InventoryItem.Get();
}

void USInv_HoverItem::SetInventoryItem(USInv_InventoryItem* Item)
{
	InventoryItem = Item;
}
