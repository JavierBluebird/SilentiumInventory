// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "SInv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USInv_ItemComponent();

	FString GetPickupMessage() const {	return PickupMessage;	};

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FSInv_ItemManifest GetItemManifest() const {return ItemManifest;};
private:

	UPROPERTY(EditAnywhere,Category="Silentium Inventory", Replicated)
	FSInv_ItemManifest ItemManifest;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FString PickupMessage;
};
