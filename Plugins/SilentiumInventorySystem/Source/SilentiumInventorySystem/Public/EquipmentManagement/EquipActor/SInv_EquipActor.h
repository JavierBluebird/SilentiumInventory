// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "SInv_EquipActor.generated.h"

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_EquipActor : public AActor
{
	GENERATED_BODY()

public:
	ASInv_EquipActor();

	FGameplayTag GetEquipmentType() const { return EquipmentType; }
	void SetEquipmentType(FGameplayTag Type) { EquipmentType = Type; }
	
private:
	
	UPROPERTY(EditAnywhere,Category = "Silentium Inventory")
	FGameplayTag EquipmentType;
};
