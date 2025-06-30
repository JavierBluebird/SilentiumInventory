// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/SInv_ItemComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USInv_ItemComponent::USInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PickupMessage = FString("E - Pick up");
}

void USInv_ItemComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemManifest);
}
