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
class USInv_HUDWidget;


UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ASInv_PlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
private:

	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem(); // Item tracing function
	
	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TArray<TObjectPtr<UInputMappingContext>> DefaultIMCs;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|HUD Settings")
	TSubclassOf<USInv_HUDWidget> HUDWidgetClass;
	
	UPROPERTY()
	TObjectPtr<USInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Trace Settings")
	double TraceLength;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Trace Settings")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> CurrentActor; // Actor hit this frame
	TWeakObjectPtr<AActor> LastActor; // Actor hit last frame

};
