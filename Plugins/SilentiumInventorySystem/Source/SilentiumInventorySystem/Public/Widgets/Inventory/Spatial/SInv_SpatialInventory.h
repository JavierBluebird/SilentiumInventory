// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Widgets/Inventory/InventoryBase/SInv_InventoryBase.h"
#include "SInv_SpatialInventory.generated.h"

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
	virtual FSInv_SlotAvailabilityResult HasRoomForItem(USInv_ItemComponent* ItemComponent) const override;
	
private:

	// In Case of Extending the inventory system, we must add the Type in SInv_GridTypes.h
	// then create the Grid TObjectPtr, its button and the callback function bound
	// in NativeOnInitialized.
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> Switcher;
	
	// --------------------//
	// Grid objects	      //
	// ------------------//
	
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
	
	// --------------------//
	// Callback Functions //
	// -------------------//
	
	UFUNCTION()
	void ShowEquippables();
	
	UFUNCTION()
	void ShowConsumables();

	UFUNCTION()
	void ShowCraftables();
	
	// --------------------//
	//	 Helper Functions //
	// -------------------//

	void SetActiveGrid(USInv_InventoryGrid* Grid, UButton* Button);
	void DisableButton(UButton* Button);
	
};
