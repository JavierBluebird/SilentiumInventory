// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SInv_GridTypes.h"
#include "SInv_InventoryBase.generated.h"


class USInv_ItemComponent;

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FSInv_SlotAvailabilityResult HasRoomForItem(USInv_ItemComponent* ItemComponent) const { return FSInv_SlotAvailabilityResult(); };
};
