// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_GridSlot.generated.h"

class UImage;
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
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_GridSlot;

	int32 SlotIndex;
};
