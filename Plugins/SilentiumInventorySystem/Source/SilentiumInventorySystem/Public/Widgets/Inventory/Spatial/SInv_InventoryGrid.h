// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/SInv_GridTypes.h"
#include "SInv_InventoryGrid.generated.h"

struct FSInv_ItemManifest;
class USInv_ItemComponent;
class USInv_InventoryComponent;
class UCanvasPanel;
class USInv_GridSlot;
class USInv_ItemManifest;
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

	FSInv_SlotAvailabilityResult HasRoomForItem(const USInv_ItemComponent* ItemComponent);

	UFUNCTION()
	void AddItem(USInv_InventoryItem* Item);
	
private:

	TWeakObjectPtr<USInv_InventoryComponent> InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"),Category = "Silentium Inventory")
	ESInv_ItemCategory ItemCategory;
	
	void ConstructGrid();

	// Overload with Inventory Item
	FSInv_SlotAvailabilityResult HasRoomForItem(const USInv_InventoryItem* Item);

	// Overload with Manifest
	FSInv_SlotAvailabilityResult HasRoomForItem(const FSInv_ItemManifest& Manifest);

	void AddItemToIndices(const FSInv_SlotAvailabilityResult& Result, USInv_InventoryItem* NewItem);

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
