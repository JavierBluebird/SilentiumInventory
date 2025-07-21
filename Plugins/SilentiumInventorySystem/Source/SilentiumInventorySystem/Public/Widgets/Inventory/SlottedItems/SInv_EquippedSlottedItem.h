// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SInv_SlottedItem.h"
#include "SInv_EquippedSlottedItem.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquippedSlottedItemClicked, class USInv_EquippedSlottedItem*, SlottedItem);

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_EquippedSlottedItem : public USInv_SlottedItem
{
	GENERATED_BODY()

public:

	void SetEquipmentTypeTag(const FGameplayTag& Tag) { EquipmentTypeTag = Tag ;}
	FGameplayTag GetEquipmentTypeTag() const { return EquipmentTypeTag; }

	virtual FReply NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FEquippedSlottedItemClicked OnEquippedSlottedItemClicked;
	
private:

	UPROPERTY()
	FGameplayTag EquipmentTypeTag;
};
