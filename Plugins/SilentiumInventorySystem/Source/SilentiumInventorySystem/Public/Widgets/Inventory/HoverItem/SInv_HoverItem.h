// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "SInv_HoverItem.generated.h"

/**
 * The HoverItem is the item that will appear and follow the mouse
 * when an Inventory Item on the grid has been clicked
 */

class USInv_InventoryItem;
class UImage;
class UTextBlock;

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_HoverItem : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	 /*-------------------------------------*/
	 /*			Utility Functions		   */
	 /*-----------------------------------*/
	
	/** Sets the item icon image brush dynamically. */
	void SetImageBrush(const FSlateBrush& Brush) const;

	/** Updates the displayed stack count number. */
	void UpdateStackCount(int32 Count) const;

	// ----------------------------
	//		Getters & Setters
	// ----------------------------

	/** Returns the item type (as GameplayTag). */
	FGameplayTag GetItemType() const;

	/** Returns how many items are stacked in this slot. */
	int32 GetStackCount() const { return StackCount; }

	/** Returns whether the item in this slot is stackable. */
	bool IsStackable() const { return bIsStackable; }

	/** Sets whether the item is stackable. */
	void SetIsStackable(bool bStacks);

	/** Returns the previous grid index before relocation. */
	int32 GetPreviousGridIndex() const { return PreviousGridIndex; }

	/** Stores the previous grid index. */
	void SetPreviousGridIndex(const int32 Index) { PreviousGridIndex = Index; }

	/** Gets the item dimensions (width × height in grid units). */
	FIntPoint GetGridDimensions() const { return GridDimensions; }

	/** Sets the item dimensions (grid width × height). */
	void SetGridDimensions(const FIntPoint Dimensions) { GridDimensions = Dimensions; }

	/** Returns a raw pointer to the inventory item associated with this slot. */
	USInv_InventoryItem* GetInventoryItem() const;

	/** Sets the associated inventory item reference. */
	void SetInventoryItem(USInv_InventoryItem* Item);


// ================================
//		Private Members
// ================================
private:

	/** Icon image widget representing the item. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	/** Text block showing the number of stacked items. */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StackCount;

	/** Grid index this slot previously occupied. Used for undo or drag logic. */
	int32 PreviousGridIndex;

	/** Dimensions of the item in this slot (in grid units). */
	FIntPoint GridDimensions;

	/** Weak reference to the inventory item stored in this slot. */
	TWeakObjectPtr<USInv_InventoryItem> InventoryItem;

	/** Whether the item can stack with similar items. */
	bool bIsStackable {false};

	/** Current number of items stacked in this slot. */
	int32 StackCount {0};
	
};
