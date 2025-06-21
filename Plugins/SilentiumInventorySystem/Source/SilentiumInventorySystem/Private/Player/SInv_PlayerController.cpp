// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SInv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ASInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (IsValid(InputSubsystem))
	{
			for (UInputMappingContext* CurrentContext : DefaultIMCs)
			{
				InputSubsystem->AddMappingContext(CurrentContext, 0);
			}
	}
}

void ASInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ASInv_PlayerController::PrimaryInteract);
}

void ASInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Display, TEXT("Primary Interact"));
}
