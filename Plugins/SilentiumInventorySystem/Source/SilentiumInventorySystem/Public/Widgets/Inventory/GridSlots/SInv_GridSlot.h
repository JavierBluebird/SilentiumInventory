// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_GridSlot.generated.h"

class USInv_InventoryItem;
class UImage;

UENUM(BlueprintType)
enum class ESInv_GridSlotsState: uint8
{
	Unoccupied,
	Occupied,
	Selected,
	GrayedOut
};

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_GridSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSlotIndex(int32 Index) {SlotIndex = Index; }
	int32 GetSlotIndex() const {return SlotIndex; }

	ESInv_GridSlotsState GetGridSlotState() const {return GridSlotState; }

	/*--------------------------*/
	/*	 Slot Texture Setters	*/
	/*--------------------------*/
	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();

	/*----------------------------------*/
	/*	 Item Data Setters & Getters	*/
	/*----------------------------------*/
	TWeakObjectPtr<USInv_InventoryItem> GetInventoryItem() const { return InventoryItem; }
	void SetInventoryItem(TWeakObjectPtr<USInv_InventoryItem> Item);

	/*----------------------------------*/
	/*	 Stack CountSetters & Getters	*/
	/*----------------------------------*/
	int32 GetStackCount() const {return StackCount; }
	void SetStackCount(int32 Count) {StackCount = Count; }

	/*----------------------------------*/
	/*	 Slot Index Setters & Getters	*/
	/*----------------------------------*/
	int32 GetIndex() const { return SlotIndex; }
	void SetIndex(int32 Index) {SlotIndex = Index; }

	/*--------------------------------------*/
	/*	 Upper Left Slot Setters & Getters	*/
	/*--------------------------------------*/
	int32 GetUpperLeftSlotIndex() const {return UpperLeftSlotIndex; }
	void SetUpperLeftSlotIndex(int32 Index) {UpperLeftSlotIndex = Index; }

	bool IsAvailable() const {return bAvailable; }
	void SetAvailable(bool bIsAvailable) {bAvailable = bIsAvailable; }
	
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
	
	int32 SlotIndex;
	int32 StackCount;
	int32 UpperLeftSlotIndex {INDEX_NONE};
	TWeakObjectPtr<USInv_InventoryItem> InventoryItem;
	bool bAvailable;

	ESInv_GridSlotsState GridSlotState;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_GrayedOut;
	
};
