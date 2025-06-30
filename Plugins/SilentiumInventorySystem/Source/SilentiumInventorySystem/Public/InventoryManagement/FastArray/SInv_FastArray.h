#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "SInv_FastArray.generated.h"


class USInv_ItemComponent;
class USInv_InventoryComponent;
class USInv_InventoryItem;

/*---------------------------------------------*/
/*											   */
/*		Single Item Entry in Inventory		   */
/*											   */
/*---------------------------------------------*/

USTRUCT(BlueprintType)
struct FSInv_InventoryEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	public:
		FSInv_InventoryEntry() {}

	private:

		friend struct FSInv_InventoryFastArray; // foredeclare + friend declaration
		friend USInv_InventoryComponent;
		
		UPROPERTY()
		TObjectPtr<USInv_InventoryItem> Item = nullptr;
};

/*---------------------------------------------*/
/*											   */
/*		List of Inventory Items Entries		   */
/*											   */
/*---------------------------------------------*/

USTRUCT(BlueprintType)
struct FSInv_InventoryFastArray : public FFastArraySerializer
{
	GENERATED_BODY()

	public:
		
		FSInv_InventoryFastArray() : OwnerComponent(nullptr) {}
		FSInv_InventoryFastArray(UActorComponent* InOwnerComponent) : OwnerComponent(InOwnerComponent) {} // Overload

		TArray<USInv_InventoryItem*> GetAllItems();

		/*---------------------------------------*/
		/*		FFastArraySerializer contract	 */
		/*---------------------------------------*/
		
		void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
		void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
		
		/*----------------------------------------------*/
		/*		End of FFastArraySerializer contract	*/
		/*----------------------------------------------*/
		bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
		{
			return FastArrayDeltaSerialize<FSInv_InventoryEntry, FSInv_InventoryFastArray>(ItemEntriesArray,
																							DeltaParams,
																							*this);
		}

		/*---------------------------------------*/
		/*		Items Utility Functions			 */
		/*---------------------------------------*/

		USInv_InventoryItem* AddItemEntry(USInv_ItemComponent* ItemComponent);
		USInv_InventoryItem* AddItemEntry(USInv_InventoryItem* Item); // overload

		void RemoveItemEntry(USInv_InventoryItem* Item);
		
	private:

		friend USInv_InventoryComponent;;
		
		UPROPERTY(NotReplicated)
		TObjectPtr<UActorComponent> OwnerComponent;

		// Replicated list of Items
		UPROPERTY()
		TArray<FSInv_InventoryEntry> ItemEntriesArray;
};

template<>
struct TStructOpsTypeTraits<FSInv_InventoryFastArray> : public TStructOpsTypeTraitsBase2<FSInv_InventoryFastArray>
{
	enum { WithNetDeltaSerializer = true };
	
};
