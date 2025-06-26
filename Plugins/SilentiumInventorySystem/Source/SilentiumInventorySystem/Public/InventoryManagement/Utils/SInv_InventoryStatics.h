// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SInv_InventoryStatics.generated.h"

class USInv_InventoryComponent;
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
};
