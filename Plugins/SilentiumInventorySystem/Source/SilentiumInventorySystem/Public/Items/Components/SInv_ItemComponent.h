// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USInv_ItemComponent();

	FString GetPickupMessage() const {	return PickupMessage;	};

protected:


private:

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FString PickupMessage;
};
