#include "Items/Manifest/SInv_ItemManifest.h"

#include "Items/SInv_InventoryItem.h"

TObjectPtr<USInv_InventoryItem> FSInv_ItemManifest::Manifest(UObject* NewOuter)
{
	USInv_InventoryItem* Item = NewObject<USInv_InventoryItem>(NewOuter,
		USInv_InventoryItem::StaticClass());

	Item->SetItemManifest(*this);
	return Item;
}
