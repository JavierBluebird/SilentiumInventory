// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInv_EquipmentComponent.generated.h"

struct FGameplayTag;
struct FSInv_ItemManifest;
struct FSInv_EquipmentFragment;
class ASInv_EquipActor;
class USInv_InventoryItem;
class APlayerController;
class USInv_InventoryComponent;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent),Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:

protected:
	
	virtual void BeginPlay() override;

private:

	TWeakObjectPtr<USInv_InventoryComponent> InventoryComponent;
	TWeakObjectPtr<APlayerController> OwningPlayerController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	/*------------------------------------------*/
	/*			Equipment Delegates				*/
	/*------------------------------------------*/
	UFUNCTION()
	void OnItemEquipped(USInv_InventoryItem* EquippedItem);
	UFUNCTION()
	void OnItemUnequipped(USInv_InventoryItem* UnequippedItem);
	
	void InitInventoryComponent();
	void InitPlayerController();

	
	ASInv_EquipActor* SpawnEquippedActor(FSInv_EquipmentFragment* EquipmentFragment,
		const FSInv_ItemManifest& Manifest,
		USkeletalMeshComponent* AttachMesh);

	UPROPERTY()
	TArray<TObjectPtr<ASInv_EquipActor>> EquippedActors;

	ASInv_EquipActor* FindEquippedActor(const FGameplayTag& EquipmentTypeTag);
	void RemoveEquippedActor(const FGameplayTag& EquipmentTypeTag);

	UFUNCTION()
	void OnPossessedPawnChange(APawn* OldPawn, APawn* NewPawn);
};

