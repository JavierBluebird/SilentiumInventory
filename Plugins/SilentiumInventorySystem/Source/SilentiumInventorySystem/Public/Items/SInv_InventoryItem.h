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

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool IsSupportedForNetworking() const override { return true; }

	void SetItemManifest(const FSInv_ItemManifest& Manifest);
	
	// Non-mutable reference Getter 
	const FSInv_ItemManifest& GetItemManifest() const { return ItemManifest.Get<FSInv_ItemManifest>(); }

	// Mutable Reference Getter
	FSInv_ItemManifest& GetItemManifestMutable() { return ItemManifest.GetMutable<FSInv_ItemManifest>(); }

	bool IsStackable() const;
	bool IsConsumable() const;

	int32 GetTotalStackCount() const { return TotalStackCount; };
	void SetTotalStackCount(const int32 StackCount) { TotalStackCount = StackCount; };

private:
	UPROPERTY(VisibleAnywhere, meta = (BaseStruct = "/Script/SilentiumInventorySystem.SInv_Manifest"), Replicated)
	FInstancedStruct ItemManifest;
	
	UPROPERTY(Replicated)
	int32 TotalStackCount {0};
};

template <typename FragmentType>
const FragmentType* GetFragment(const USInv_InventoryItem* Item, const FGameplayTag& Tag)
{
	if (!IsValid(Item)) return nullptr;
	
	const FSInv_ItemManifest& Manifest = Item->GetItemManifest();
	
	return Manifest.GetFragmentOfTypeWithTag<FragmentType>(Tag);
};