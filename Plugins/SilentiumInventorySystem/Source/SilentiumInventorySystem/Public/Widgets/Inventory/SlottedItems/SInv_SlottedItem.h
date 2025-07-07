// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_SlottedItem.generated.h"

class USInv_InventoryItem;
class UImage;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_SlottedItem : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetIsStackable(bool bStackable) { bIsStackable = bStackable; }
	UImage* GetImageIcon() const { return ImageIcon; }
	void SetGridIndex(int32 Index) { GridIndex = Index; }
	int32 GetGridIndex() const { return GridIndex; }
	void SetGridDimensions(const FIntPoint& Dimensions) { GridDimensions = Dimensions; }
	FIntPoint GetGridDimensions() const { return GridDimensions; }
	void SetInventoryItem(USInv_InventoryItem* Item);
	USInv_InventoryItem* GetInventoryItem() const { return InventoryItem.Get(); }
	bool IsStackable() const { return bIsStackable; }
	void SetImageBrush(const FSlateBrush Brush) const;
	
private:
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UImage> ImageIcon;

	int32 GridIndex;
	FIntPoint GridDimensions;
	TWeakObjectPtr<USInv_InventoryItem> InventoryItem;
	bool bIsStackable{false};
};
