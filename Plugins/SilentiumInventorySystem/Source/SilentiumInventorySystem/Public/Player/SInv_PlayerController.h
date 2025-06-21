// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SInv_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;
};
