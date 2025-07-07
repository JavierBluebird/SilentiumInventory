// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_GridSlot.generated.h"

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

	void SetOccupiedTexture();
	void SetUnoccupiedTexture();
	void SetSelectedTexture();
	void SetGrayedOutTexture();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;
	
	int32 SlotIndex;

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
