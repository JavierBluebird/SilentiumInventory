// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SInv_GridSlot.h"
#include "SInv_EquippedGridSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquippedGridSlotClicked,
											 USInv_EquippedGridSlot*, GridSlot,
											 const FGameplayTag&, EquipmentTypeTag);
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_EquippedGridSlot : public USInv_GridSlot
{
	GENERATED_BODY()

public:
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	FEquippedGridSlotClicked EquippedGridSlotClicked;
private:

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (Categories = "GameItems.Equipment", ToolTip = "Tag that indicates to what Equipment type this Slot belongs to."))
	FGameplayTag EquipmentTypeTag;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GrayedOutIcon;
};
