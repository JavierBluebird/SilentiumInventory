#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Types/SInv_GridTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "SInv_ItemManifest.generated.h"

struct FSInv_ItemFragment;
/*--------------------------------------------------------------------------------------*/
/*																					   */
/* Item Manifest contains all of the necessary data for creating a new Inventory Item */
/*																				     */
/*----------------------------------------------------------------------------------*/
class USInv_InventoryItem;
struct FSInv_ItemFragment;

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

	template<typename T> requires std::derived_from<T, FSInv_ItemFragment>
	const T* GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const;

	template<typename T> requires std::derived_from<T, FSInv_ItemFragment>
	const T* GetFragmentOfType() const;

	template<typename T> requires std::derived_from<T, FSInv_ItemFragment>
	T* GetFragmentOfTypeMutable();

	void SpawnPickUpActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation);
	
private:

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties", meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FSInv_ItemFragment>> Fragments;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties")
	ESInv_ItemCategory ItemCategory { ESInv_ItemCategory::None };
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties")
	FGameplayTag ItemType;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory|Item Properties")
	TSubclassOf<AActor> PickupActorClass;
};

/*--------------------------------------------*/
/*	Fragment Type Searching with Tag Checking */
/*--------------------------------------------*/
template<typename T>
requires std::derived_from<T, FSInv_ItemFragment>
const T* FSInv_ItemManifest::GetFragmentOfTypeWithTag(const FGameplayTag& FragmentTag) const
{
	for (const TInstancedStruct<FSInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			// If the fragment doesnt match the tag, skips this index
			if (!FragmentPtr->GetFragmentTag().MatchesTagExact(FragmentTag)) continue;
			return FragmentPtr; // Returns detected valid Fragment const pointer
		}
	}
	return nullptr;
}
/*-----------------------------------------------*/
/*	Fragment Type Searching without Tag Checking */
/*-----------------------------------------------*/
template<typename T>
requires std::derived_from<T, FSInv_ItemFragment>
const T* FSInv_ItemManifest::GetFragmentOfType() const
{
	for (const TInstancedStruct<FSInv_ItemFragment>& Fragment : Fragments)
	{
		if (const T* FragmentPtr = Fragment.GetPtr<T>())
		{
			return FragmentPtr; // Returns detected valid Fragment const pointer
		}
	}
	return nullptr;
}

template <typename T>
requires std::derived_from<T, FSInv_ItemFragment>
T* FSInv_ItemManifest::GetFragmentOfTypeMutable()
{
	for (TInstancedStruct<FSInv_ItemFragment>& Fragment : Fragments)
	{
		if (T* FragmentPtr = Fragment.GetMutablePtr<T>())
		{
			return FragmentPtr; // Returns detected valid Fragment mutable pointer
		}
	}
	return nullptr;
}
