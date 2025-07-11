// Developed by Javier Gonzalez [Lumina Division]


#include "Items/SInv_InventoryItem.h"

#include "Items/Fragments/SInv_ItemFragment.h"
#include "Net/UnrealNetwork.h"


void USInv_InventoryItem::SetItemManifest(const FSInv_ItemManifest& Manifest)
{
	ItemManifest = FInstancedStruct::Make<FSInv_ItemManifest>(Manifest);
}

bool USInv_InventoryItem::IsStackable() const
{
	const FSInv_StackableFragment* Stackable = GetItemManifest().GetFragmentOfType<FSInv_StackableFragment>();
	return Stackable != nullptr;
}

void USInv_InventoryItem::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	UObject::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
	DOREPLIFETIME(ThisClass, TotalStackCount);
}
