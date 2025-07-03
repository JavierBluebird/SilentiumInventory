// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SInv_InventoryStatics.generated.h"

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
	
};
