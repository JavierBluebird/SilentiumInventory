// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_SlottedItem.generated.h"

class USInv_InventoryItem;
class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSlottedItemClicked, int32, GridIndex, const FPointerEvent&, MouseEvent);

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	// Sets whether the item is stackable
	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }

	// Returns the UI image widget used to display the item icon
	UImage* GetImageIcon() const { return ImageIcon; }

	// Sets the index of the grid cell this item occupies
	void SetGridIndex(int32 Index) { GridIndex = Index; }

	// Gets the current grid index of this item
	int32 GetGridIndex() const { return GridIndex; }

	// Sets the size (in grid cells) the item occupies
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }

	// Gets the grid dimensions of this item
	FIntPoint GetGridDimensions() const { return GridDimensions; }

	// Assigns a reference to the underlying inventory item data
	void SetInventoryItem(USInv_InventoryItem* Item);

	// Retrieves the inventory item reference
	USInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }

	// Returns whether this item can stack with others
	bool IsStackable() const { return bIsStackable; }

	// Sets the visual brush of the item's image icon
	void SetImageBrush(const FSlateBrush Brush) const;
	
	void UpdateStackCount(int32 StackCount);

	FSlottedItemClicked OnSlottedItemClicked;
	
private:

	// UI image widget bound via UMG to display the item icon
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	// Index position within the inventory grid
	int32 GridIndex;

	// Dimensions of the item in grid cells (width x height)
	FIntPoint GridDimensions;

	// Weak reference to the logical inventory item backing this widget
	TWeakObjectPtr<USInv_InventoryItem> InventoryItem;

	// Indicates if the item supports stacking
	bool bIsStackable { false };

};
