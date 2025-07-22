// Developed by Javier Gonzalez [Lumina Division]


#include "EquipmentManagement/Components/SInv_EquipmentComponent.h"

#include "GameFramework/Character.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Fragments/SInv_ItemFragment.h"


void USInv_EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningPlayerController = Cast<APlayerController>(GetOwner());

	// Only if our owner is a Player controller we proceed.
	if (OwningPlayerController.IsValid())
	{
		ACharacter* OwnerCharacter = Cast<ACharacter>(OwningPlayerController->GetPawn());
		if (OwnerCharacter)
		{
			OwningSkeletalMesh = OwnerCharacter->GetMesh();
		}
		InitInventoryComponent();
	}
}

void USInv_EquipmentComponent::InitInventoryComponent()
{
	InventoryComponent = USInv_InventoryStatics::GetInventoryComponent(OwningPlayerController.Get());
	
	if (!InventoryComponent.IsValid()) return;

	/*------------------------------------------*/
	/*			Equipment Delegates				*/
	/*	Check if Delegates are already bound.   */
	/*------------------------------------------*/
	
	if (!InventoryComponent->OnItemEquipped.IsAlreadyBound(this,&ThisClass::OnItemEquipped))
	{
		InventoryComponent->OnItemEquipped.AddDynamic(this,&ThisClass::OnItemEquipped);
	}
	if (!InventoryComponent->OnItemUnequipped.IsAlreadyBound(this,&ThisClass::OnItemUnequipped))
	{
		InventoryComponent->OnItemUnequipped.AddDynamic(this,&ThisClass::OnItemUnequipped);
	}
}

void USInv_EquipmentComponent::OnItemEquipped(USInv_InventoryItem* EquippedItem)
{
	if (!EquippedItem) return;
	if (!OwningPlayerController->HasAuthority()) return; // Only happens on server.

	FSInv_ItemManifest& ItemManifest = EquippedItem->GetItemManifestMutable();
	FSInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FSInv_EquipmentFragment>();

	if (!EquipmentFragment) return;
	
	EquipmentFragment->OnEquip(OwningPlayerController.Get());
}

void USInv_EquipmentComponent::OnItemUnequipped(USInv_InventoryItem* UnequippedItem)
{
	if (!UnequippedItem) return;
	if (!OwningPlayerController->HasAuthority()) return; // Only happens on server.

	FSInv_ItemManifest& ItemManifest = UnequippedItem->GetItemManifestMutable();
	FSInv_EquipmentFragment* EquipmentFragment = ItemManifest.GetFragmentOfTypeMutable<FSInv_EquipmentFragment>();

	if (!EquipmentFragment) return;
	
	EquipmentFragment->OnUnequip(OwningPlayerController.Get());
}

