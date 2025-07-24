// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SInv_PlayerController.generated.h"


class USInv_InventoryComponent;
/**
 * 
 */
class UInputMappingContext;
class UInputAction;
class USInv_HUDWidget;


UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASInv_PlayerController();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:

};
