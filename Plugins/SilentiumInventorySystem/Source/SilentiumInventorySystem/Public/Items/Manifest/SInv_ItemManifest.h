#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/SInv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "SInv_ItemManifest.generated.h"

/*--------------------------------------------------------------------------------------*/
/*																					   */
/* Item Manifest contains all of the necessary data for creating a new Inventory Item */
/*																				     */
/*----------------------------------------------------------------------------------*/
class USInv_InventoryItem;

enum class ESInv_ItemCategory : uint8;

USTRUCT(BlueprintType)
struct SILENTIUMINVENTORYSYSTEM_API FSInv_ItemManifest
{
	GENERATED_BODY()

	TObjectPtr<USInv_InventoryItem> Manifest(UObject* NewOuter);
	
	// Category Getter
	ESInv_ItemCategory GetItemCategory() const {return ItemCategory;}
	// Item Type Getter
	FGameplayTag GetItemType() const { return ItemType; }
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties")
	ESInv_ItemCategory ItemCategory { ESInv_ItemCategory::None };
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties")
	FGameplayTag ItemType;
};


