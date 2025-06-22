// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Components/SInv_ItemComponent.h"


// Sets default values for this component's properties
USInv_ItemComponent::USInv_ItemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PickupMessage = FString("E - Pick up");
}
