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

	TWeakObjectPtr<USInv_InventoryItem> Item;

	bool bStackable{false};
	int32 TotalRoomToFill{0};
	int32 Remainder{0}; // Amount of items we couldn't fit in the Inventory
	TArray<FSInv_SlotAvailability> SlotAvailabilities;
};

UENUM(BlueprintType)
enum class ESInv_TileQuadrant : uint8
{
	TopLeft,
	TopRight,
	BottomLeft,
	BottomRight,
	None
};

USTRUCT(BlueprintType)
struct FSInv_TileParameters
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Silentium Inventory")
	FIntPoint TileCoordinates{};

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Silentium Inventory")
	int32 TileIndex{INDEX_NONE};

	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Silentium Inventory")
	ESInv_TileQuadrant TileQuadrant{ESInv_TileQuadrant::None};
};

// Operator == Overload
inline bool operator == (const FSInv_TileParameters &A, const FSInv_TileParameters &B)
{
	return  A.TileCoordinates == B.TileCoordinates &&
			A.TileIndex == B.TileIndex &&
			A.TileQuadrant == B.TileQuadrant;
}