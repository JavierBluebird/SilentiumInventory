// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SInv_PlayerController.h"
#include "SilentiumInventorySystem.h"

void ASInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogSilentiumInventory, Log, TEXT("Begin play for Player Controller"));
}
