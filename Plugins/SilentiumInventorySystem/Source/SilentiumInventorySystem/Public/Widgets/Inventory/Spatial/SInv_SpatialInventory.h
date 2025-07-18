// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Widgets/Inventory/InventoryBase/SInv_InventoryBase.h"
#include "SInv_SpatialInventory.generated.h"

struct FGameplayTag;
class USInv_EquippedGridSlot;
class USInv_ItemDescription;
class UCanvasPanel;
class UButton;
class UWidgetSwitcher;
class USInv_InventoryGrid;

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_SpatialInventory : public USInv_InventoryBase
{
	GENERATED_BODY()

public:
	
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FSInv_SlotAvailabilityResult HasRoomForItem(USInv_ItemComponent* ItemComponent) const override;

	virtual void OnItemHovered(USInv_InventoryItem* InventoryItem) override;
	virtual bool HasHoverItem() const override;
	virtual void OnItemUnhovered() override;

	virtual USInv_HoverItem* GetHoverItem() const override;
	
private:

	UPROPERTY()
	TArray<TObjectPtr<USInv_EquippedGridSlot>> EquippedGridSlots;
	
	// In Case of Extending the inventory system, we must add the Type in SInv_GridTypes.h
	// then create the Grid TObjectPtr, its button and the callback function bound
	// in NativeOnInitialized.
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	// --------------------//
	// Grid objects	      //
	// ------------------//
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USInv_InventoryGrid> Grid_Equippables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USInv_InventoryGrid> Grid_Consumables;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USInv_InventoryGrid> Grid_Craftables;

	// --------------------//
	// Button objects	  //
	// -------------------//
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Equippables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Consumables;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Craftables;

	// ----------------------------//
	// Item Description objects	  //
	// --------------------------//

	UPROPERTY(EditAnywhere)
	TSubclassOf<USInv_ItemDescription> ItemDescriptionClass;

	UPROPERTY()
	TObjectPtr<USInv_ItemDescription> ItemDescription;

	USInv_ItemDescription* GetItemDescription();

	FTimerHandle DescriptionTimer;

	UPROPERTY(EditAnywhere,Category="Silentium Inventory", meta =(ToolTip = "How much time will it take for Item Description Widget to Show up"))
	float DescriptionTimerDelay {0.5f};

	void SetItemDescriptionSizeAndPosition(USInv_ItemDescription* Description, UCanvasPanel* Canvas) const;
	// --------------------//
	// Callback Functions //
	// -------------------//
	
	UFUNCTION()
	void ShowEquippables();
	
	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();

	UFUNCTION()
	void EquippedGridSlotClicked(USInv_EquippedGridSlot* EquippedGridSlot, const FGameplayTag& EquipmentTypeTag);
	
	// --------------------//
	//	 Helper Functions //
	// -------------------//

	void SetActiveGrid(USInv_InventoryGrid* Grid, UButton* Button);
	void DisableButton(UButton* Button);

	TWeakObjectPtr<USInv_InventoryGrid> ActiveGrid;
};
