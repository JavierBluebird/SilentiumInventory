// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "Types/SInv_GridTypes.h"
#include "Widgets/Inventory/GridSlots/SInv_GridSlot.h"
#include "SInv_InventoryGrid.generated.h"


class USInv_HoverItem;
struct FSInv_ImageFragment;
struct FSInv_GridFragment;
class USInv_SlottedItem;
struct FSInv_ItemManifest;
class USInv_ItemComponent;
class USInv_InventoryComponent;
class UCanvasPanel;
class USInv_GridSlot;
class USInv_ItemManifest;
enum class ESInv_GridSlotsState : uint8;

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryGrid : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
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

	/*---------------------------------------------------*/
	/*													 */
	/*	  Item Adding Checker & Utility Functions		 */
	/*													 */
	/*---------------------------------------------------*/
	
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

	bool IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const;
	
	bool IsUpperLeftSlot(const USInv_GridSlot* GridSlot,const USInv_GridSlot* SubGridSlot) const;

	bool DoesItemTypeMatch(const USInv_InventoryItem* SubItem ,const FGameplayTag& ItemType) const;

	bool IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const;

	int32 DetermineFillAmountForSlot(const bool bStackable,
									 const int32 MaxStackSize,
									 const int32 AmountToFill,
									 const USInv_GridSlot* GridSlot) const;

	int32 GetStackAmount(const USInv_GridSlot* GridSlot) const;
	
	FIntPoint GetItemDimensions(const FSInv_ItemManifest& ItemManifest) const;

	bool HasValidItem(const USInv_GridSlot* GridSlot) const;

	UFUNCTION()
	void AddStacks(const FSInv_SlotAvailabilityResult& Result);

	UFUNCTION()
	void OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent);

	/*----------------------------------*/
	/*		Hover Item Mouse Events		*/
	/*----------------------------------*/
	
	bool IsRightClick(const FPointerEvent& MouseEvent) const;
	bool IsLeftClick(const FPointerEvent& MouseEvent) const;
	void PickUp(USInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex);
	
	void AssignHoverItem(USInv_InventoryItem* InventoryItem);
	// Overload with more Info
	void AssignHoverItem(USInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex);

	void RemoveItemFromGrid(const USInv_InventoryItem* InventoryItem, const int32 GridIndex);

	void UpdateTileParameters(const FVector2D& CanvasPos, const FVector2D& MousePos);
	FIntPoint CalculateHoveredCoordinates(const FVector2D& CanvasPos, const FVector2D& MousePos) const;
	ESInv_TileQuadrant CalculateTileQuadrant(const FVector2D& CanvasPos, const FVector2D& MousePos) const;
	void OnTileParametersUpdate(const FSInv_TileParameters& Parameters);
	FIntPoint CalculateStartingCoordinates(const FIntPoint& Coordinate, const FIntPoint& Dimensions,  const ESInv_TileQuadrant Quadrant) const;
	FSInv_SpaceQueryResult CheckHoverPosition(const FIntPoint& Position, const FIntPoint& Dimensions);

	bool CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize, const FVector2D& Location);
	void HighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions);
	void ChangeHoverType(const int32 Index, const FIntPoint& Dimensions, ESInv_GridSlotsState GridSlotState);
	/*---------------------------------------------------*/
	/*													 */
	/*			Member Variables Section				 */
	/*													 */
	/*---------------------------------------------------*/
	
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

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TSubclassOf<USInv_HoverItem> HoverItemClass;

	UPROPERTY()
	TObjectPtr<USInv_HoverItem> HoverItem;

	FSInv_TileParameters TileParameters;
	FSInv_TileParameters LastTileParameters;
	FSInv_SpaceQueryResult CurrentQueryResult;
	bool bMouseWithinCanvas;
	bool bLastMouseWithinCanvas;

	// Index where an Item would be placed if we click on the grid at a valid location.
	int32 ItemDropIndex{INDEX_NONE};
	
	int32 LastHighlightedIndex;
	FIntPoint LastHighlightedDimensions;
};


