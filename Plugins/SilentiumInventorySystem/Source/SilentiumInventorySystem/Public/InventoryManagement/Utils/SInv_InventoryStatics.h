// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "Widgets/Inventory/Utils/SInv_WidgetUtils.h"
#include "UObject/Object.h"
#include "SInv_InventoryStatics.generated.h"

class USInv_HoverItem;
class USInv_InventoryComponent;
struct FSInv_ItemManifest;
class USInv_ItemComponent;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryStatics : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "SilentiumInventory")
	static USInv_InventoryComponent* GetInventoryComponent(const APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "SilentiumInventory")
	static ESInv_ItemCategory GetItemCategoryFromItemComp(const USInv_ItemComponent* ItemComp);

	template<typename T, typename FuncT>
	static void ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function);

	UFUNCTION(BlueprintCallable, Category = "SilentiumInventory")
	static void ItemHovered(APlayerController* PC, USInv_InventoryItem* Item);

	UFUNCTION(BlueprintCallable, Category = "SilentiumInventory")
	static void ItemUnhovered(APlayerController* PC);
	
	UFUNCTION(BlueprintCallable, Category = "SilentiumInventory")
	static USInv_HoverItem* GetHoveredItem(APlayerController* PC);
};

/*---------------------------------*/
/*	Template Functions Definitions */
/*---------------------------------*/

template<typename T, typename FuncT>
void USInv_InventoryStatics::ForEach2D(TArray<T>& Array, int32 Index, const FIntPoint& Range2D, int32 GridColumns, const FuncT& Function)
{
	for (int32 j = 0; j < Range2D.Y; ++j) // Vertical, Columns Iterator
	{
		for (int32 i = 0; i < Range2D.X; ++i) // Horizontal, Rows Iterator
		{
			const FIntPoint Coordinates = USInv_WidgetUtils::GetPositionFromIndex(Index,GridColumns) + FIntPoint(i, j);
			const int32 TileIndex = USInv_WidgetUtils::GetIndexFromPosition(Coordinates,GridColumns);
			if (Array.IsValidIndex(TileIndex))
			{
				Function(Array[TileIndex]);
			}
		}
	}
}