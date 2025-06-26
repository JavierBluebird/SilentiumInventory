#pragma once

#include "SInv_GridTypes.generated.h"

class USInv_InventoryItem;

UENUM(BlueprintType)
enum class ESInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};

USTRUCT()
struct FSInv_SlotAvailability
{
	GENERATED_BODY()

	FSInv_SlotAvailability(){}
	FSInv_SlotAvailability(int32 SlotIndex, int32 ItemAmountToFill, bool bItemAtIndexDetected)
		: SlotIndex(SlotIndex),
	      AmountToFill(ItemAmountToFill),
		  bItemAtIndex(bItemAtIndexDetected){}
	
	int32 SlotIndex{INDEX_NONE};
	int32 AmountToFill{0};
	bool bItemAtIndex{false}; // Nos dira si hay un item en ese Grid Slot
						      // De esta manera no hay que crear un nuevo Widget, solo updatear el Stack Count.
};

USTRUCT()
struct FSInv_SlotAvailabilityResult
{
	GENERATED_BODY()

	FSInv_SlotAvailabilityResult() {}

	TWeakPtr<USInv_InventoryItem> Item;

	bool bStackable{false};
	int32 TotalRoomToFill{0};
	int32 Remainder{0}; // Amount of items we couldn't fit in the Inventory
	TArray<FSInv_SlotAvailability> SlotAvailabilities;
};
