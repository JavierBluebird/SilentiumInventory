// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SInv_PlayerController.generated.h"




/**
 * 
 */
class UInputMappingContext;
class UInputAction;

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	void PrimaryInteract();
	

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TObjectPtr<UInputAction> PrimaryInteractAction;
};
