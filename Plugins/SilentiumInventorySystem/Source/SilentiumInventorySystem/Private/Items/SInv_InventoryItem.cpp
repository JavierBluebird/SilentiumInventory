// Developed by Javier Gonzalez [Lumina Division]


#include "Items/SInv_InventoryItem.h"

#include "Net/UnrealNetwork.h"

void USInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}

void USInv_InventoryItem::SetItemManifest(const FSInv_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FSInv_ItemManifest>(Manifest);
}
