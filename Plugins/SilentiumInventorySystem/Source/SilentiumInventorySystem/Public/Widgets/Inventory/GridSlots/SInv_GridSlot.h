// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_GridSlot.generated.h"

class USInv_ItemPopUp;
class USInv_InventoryItem;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGridSlotEvent, int32, GridIndex, const FPointerEvent&, MouseEvent);

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

	/*-------------------------------*/
	/*	Virtual Functions Overrides	*/
	/*------------------------------*/
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
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

	/*--------------------------------------*/
	/*		Grid Slot Event Delegates    	*/
	/*--------------------------------------*/
	FGridSlotEvent GridSlotClicked;
	FGridSlotEvent GridSlotHovered;
	FGridSlotEvent GridSlotUnhovered;

	void SetItemPopUp(USInv_ItemPopUp* PopUp);
	USInv_ItemPopUp* GetItemPopUp() const;
	
private:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
	
	int32 StackCount{0};
	bool bAvailable {true};
	int32 SlotIndex{INDEX_NONE};
	int32 UpperLeftSlotIndex {INDEX_NONE};

	/*--------------------------------------*/
	/*		  Weak Object Pointers    		*/
	/*--------------------------------------*/
	TWeakObjectPtr<USInv_InventoryItem> InventoryItem;
	TWeakObjectPtr<USInv_ItemPopUp> ItemPopUp;

	ESInv_GridSlotsState GridSlotState;

	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Unoccupied;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Occupied;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_Selected;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FSlateBrush Brush_GrayedOut;

	UFUNCTION()
	void OnItemPopUpDestruct(UUserWidget* Menu);
};
