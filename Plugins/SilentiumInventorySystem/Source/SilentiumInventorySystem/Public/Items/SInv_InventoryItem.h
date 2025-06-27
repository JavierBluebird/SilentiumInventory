// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "SInv_InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryItem : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/SilentiumInventorySystem.SInv_Manifest"))
	FInstancedStruct ItemManifest;
};
