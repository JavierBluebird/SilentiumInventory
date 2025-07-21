// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SInv_GridTypes.h"
#include "SInv_InventoryBase.generated.h"


class USInv_HoverItem;
class USInv_ItemComponent;
class USInv_InventoryItem;

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FSInv_SlotAvailabilityResult HasRoomForItem(USInv_ItemComponent* ItemComponent) const { return FSInv_SlotAvailabilityResult(); };

	virtual void OnItemHovered(USInv_InventoryItem* InventoryItem) { }
	virtual void OnItemUnhovered() { }
	virtual bool HasHoverItem() const { return false; }
	virtual USInv_HoverItem* GetHoverItem() const { return nullptr; }
	virtual float GetSlotSize() const { return 0.0f; }
};
