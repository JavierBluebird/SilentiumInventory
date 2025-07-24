// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SInv_PlayerController.h"


ASInv_PlayerController::ASInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASInv_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ASInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}
