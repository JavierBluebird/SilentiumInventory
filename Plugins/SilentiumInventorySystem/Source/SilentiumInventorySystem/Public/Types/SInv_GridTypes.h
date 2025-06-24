#pragma once

#include "SInv_GridTypes.generated.h"

UENUM(BlueprintType)
enum class ESInv_ItemCategory : uint8
{
	Equippable,
	Consumable,
	Craftable,
	None
};