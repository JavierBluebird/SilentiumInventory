#include "Items/Manifest/SInv_ItemManifest.h"

#include "Items/SInv_InventoryItem.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Items/Fragments/SInv_ItemFragment.h"
#include "Widgets/Composite/SInv_Composite.h"

TObjectPtr<USInv_InventoryItem> FSInv_ItemManifest::Manifest(UObject* NewOuter)
{
	USInv_InventoryItem* Item = NewObject<USInv_InventoryItem>(NewOuter,
		USInv_InventoryItem::StaticClass());

	Item->SetItemManifest(*this);
	return Item;
}

void FSInv_ItemManifest::SpawnPickUpActor(const UObject* WorldContextObject, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	if (!PickupActorClass || !IsValid(WorldContextObject)) return;

	AActor* SpawnedActor = WorldContextObject->GetWorld()->SpawnActor<AActor>(PickupActorClass, SpawnLocation, SpawnRotation);

	if (!IsValid(SpawnedActor)) return;

	// Set the Item Manifest, Item Category, Item Type, etc.
	USInv_ItemComponent* ItemComponent = SpawnedActor->FindComponentByClass<USInv_ItemComponent>();
	check(ItemComponent);

	ItemComponent->InitItemManifest(*this); // Pass by Value to copy this manifest into the newly spawned item's one
}

void FSInv_ItemManifest::AssimilateInventoryFragments(USInv_CompositeBase* Composite) const
{
	const auto& InventoryItemFragments = GetAllFragmentsOfType<FSInv_InventoryItemFragment>();

	for (const auto* Fragment : InventoryItemFragments)
	{
		Composite->ApplyFunction([Fragment](USInv_CompositeBase* Widget)
		{
			Fragment->Assimilate(Widget);
		});
	}
}

