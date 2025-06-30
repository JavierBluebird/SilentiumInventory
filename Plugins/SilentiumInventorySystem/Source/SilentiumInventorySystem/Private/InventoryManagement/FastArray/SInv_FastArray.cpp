#include "InventoryManagement/FastArray/SInv_FastArray.h"

#include "ToolMenusEditor.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Tests/ToolMenusTestUtilities.h"

TArray<USInv_InventoryItem*> FSInv_InventoryFastArray::GetAllItems()
{
	TArray<USInv_InventoryItem*> Results;
	
	Results.Reserve(ItemEntriesArray.Num()); // Reserves space in memory

	for (const auto& Entry: ItemEntriesArray)
	{
		if (!IsValid(Entry.Item)) continue; // skips this index if Item is not valid
		Results.Add(Entry.Item); // if valid, add to our Results array
	}
	
	return Results;
}

void FSInv_InventoryFastArray::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	USInv_InventoryComponent* IC = Cast<USInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : RemovedIndices)
	{
		IC->OnItemRemoved.Broadcast(ItemEntriesArray[Index].Item);
	}
}

void FSInv_InventoryFastArray::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	USInv_InventoryComponent* IC = Cast<USInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return;

	for (int32 Index : AddedIndices)
	{
		IC->OnItemAdded.Broadcast(ItemEntriesArray[Index].Item);
	}
}

// This Function creates a New Item via Item Manifest.
USInv_InventoryItem* FSInv_InventoryFastArray::AddItemEntry(USInv_ItemComponent* ItemComponent)
{
	// Inventory Component getters
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	
	check(OwningActor->HasAuthority()); // Must be ran on Server
	USInv_InventoryComponent* IC = Cast<USInv_InventoryComponent>(OwnerComponent);
	if (!IsValid(IC)) return nullptr;

	FSInv_InventoryEntry& NewItemEntry = ItemEntriesArray.AddDefaulted_GetRef(); // Adds an empty struct and get the ref.
	NewItemEntry.Item = ItemComponent->GetItemManifest().Manifest(OwningActor); // Item Manifest createst the item

	IC->AddRepSubObj(NewItemEntry.Item); // Adds Item as Replicated Subobject to Inv Component.
	MarkItemDirty(NewItemEntry); // Fast Serializer
	
	return NewItemEntry.Item;
}

USInv_InventoryItem* FSInv_InventoryFastArray::AddItemEntry(USInv_InventoryItem* Item)
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner(); // Should be an Inventory Component.
	check(OwningActor->HasAuthority());

	FSInv_InventoryEntry& NewItemEntry = ItemEntriesArray.AddDefaulted_GetRef(); // Adds an empty struct.
	NewItemEntry.Item = Item; // Sets the item in the recently added struct in the array
	
	MarkItemDirty(NewItemEntry);
	
	return NewItemEntry.Item;
}

void FSInv_InventoryFastArray::RemoveItemEntry(USInv_InventoryItem* Item)
{
	for (auto EntryIt = ItemEntriesArray.CreateIterator(); EntryIt; ++EntryIt)
	{
		FSInv_InventoryEntry& Entry = *EntryIt; // DEREFERENCED ITERATOR
		if (Entry.Item == Item)
		{
			EntryIt.RemoveCurrent(); 
			MarkArrayDirty();
		}
	}
}
