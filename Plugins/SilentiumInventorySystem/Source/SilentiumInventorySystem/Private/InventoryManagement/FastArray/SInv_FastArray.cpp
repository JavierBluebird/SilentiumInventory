#include "InventoryManagement/FastArray/SInv_FastArray.h"

TArray<USInv_InventoryItem*> FSInv_InventoryFastArray::GetAllItems()
{
	
}

void FSInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
}

void FSInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
}

USInv_InventoryItem* FSInv_InventoryFastArray::AddItemEntry(USInv_InventoryComponent* ItemComponent)
{
}

USInv_InventoryItem* FSInv_InventoryFastArray::AddItemEntry(USInv_InventoryItem* Item)
{
}

void FSInv_InventoryFastArray::RemoveItemEntry(USInv_InventoryItem* Item)
{
}
