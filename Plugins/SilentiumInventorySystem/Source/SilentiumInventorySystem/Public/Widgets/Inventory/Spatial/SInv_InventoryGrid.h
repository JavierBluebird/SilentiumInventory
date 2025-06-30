// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Items/SInv_InventoryItem.h"
#include "Types/SInv_GridTypes.h"
#include "SInv_InventoryGrid.generated.h"

class USInv_InventoryComponent;
class UCanvasPanel;
class USInv_GridSlot;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	ESInv_ItemCategory GetItemCategory() const { return ItemCategory; };

	UFUNCTION()
	void AddItem(USInv_InventoryItem* Item);
	
private:

	TWeakObjectPtr<USInv_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "Silentium Inventory")
	ESInv_ItemCategory ItemCategory;

	void ConstructGrid();

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TSubclassOf<USInv_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;
	
	UPROPERTY(meta = (BindWidget))
	TArray<TObjectPtr<USInv_GridSlot>> GridSlotsArray;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 Rows;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 Columns;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float SlotSize;

	bool MatchesCategory(const USInv_InventoryItem* Item) const;
	
};
