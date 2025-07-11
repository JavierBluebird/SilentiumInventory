// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Types/SInv_GridTypes.h"
#include "SInv_InventoryGrid.generated.h"


struct FSInv_ImageFragment;
struct FSInv_GridFragment;
class USInv_SlottedItem;
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
	bool MatchesCategory(const USInv_InventoryItem* Item) const;

	FVector2D GetDrawSize(const FSInv_GridFragment* GridFragment) const;
	void SetSlottedItemImage(const USInv_SlottedItem* SlottedItem, const FSInv_GridFragment* GridFragment, const FSInv_ImageFragment* ImageFragment) const;
	void AddItemAtIndex(USInv_InventoryItem* NewItem, const int32 Index, const bool bStackable, const int32 StackAmount) ;

	USInv_SlottedItem* CreateSlottedItem(USInv_InventoryItem* Item,
		const bool bStackable,
		const int32 StackAmount,
		const FSInv_GridFragment* GridFragment,
		const FSInv_ImageFragment* ImageFragment,
		const int32 Index ) const;

	void AddSlottedItemToCanvas(const int32 SlotIndex,
		const FSInv_GridFragment* GridFragment,
		USInv_SlottedItem* SlottedItem) const;

	void UpdateGridSlots(USInv_InventoryItem* NewItem, const int32 SlotIndex, bool bStackableItem, const int32 StackAmount);
	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;

	// Function to check Spatial Grid on Item Adding and Has Room checking.
	bool HasRoomAtIndex(const USInv_GridSlot* GridSlot,
		const FIntPoint& ItemDimensions,
		const TSet<int32>& CheckedIndices,
		TSet<int32>& OutTentativelyClaimed,
		const FGameplayTag& ItemType,
		const int32 MaxStackSize);
	
	bool CheckSlotConstraints(const USInv_GridSlot* GridSlot,
							  const USInv_GridSlot* SubGridSlot, 
							  const TSet<int32>& CheckedIndices,
							  TSet<int32>& OutTentativelyClaimedIndices,
							  const FGameplayTag& ItemType,
							  const int32 MaxStackSize) const;

	bool IsUpperLeftSlot(const USInv_GridSlot* GridSlot,const USInv_GridSlot* SubGridSlot) const;

	bool DoesItemTypeMatch(const USInv_InventoryItem* SubItem ,const FGameplayTag& ItemType) const;

	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;
	
	FIntPoint GetItemDimensions(const FSInv_ItemManifest& ItemManifest) const;

	bool HasValidItem(const USInv_GridSlot* GridSlot) const;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TSubclassOf<USInv_GridSlot> GridSlotClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TSubclassOf<USInv_SlottedItem> SlottedItemClass;

	UPROPERTY()
	TMap<int32, TObjectPtr<USInv_SlottedItem>> SlottedItems;
	
	UPROPERTY(meta = (BindWidget))
	TArray<TObjectPtr<USInv_GridSlot>> GridSlotsArray;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 Rows;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 Columns;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float SlotSize;


	
};
