// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SInv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/SInv_HUDWidget.h"

ASInv_PlayerController::ASInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.f;
}

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
	CreateHUDWidget();
}

void ASInv_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForItem();
}


void ASInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(PrimaryInteractAction, ETriggerEvent::Started, this, &ASInv_PlayerController::PrimaryInteract);
}


void ASInv_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController()) return;

	HUDWidget = CreateWidget<USInv_HUDWidget>(this,HUDWidgetClass);

	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void ASInv_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2;

	// Transform to World Space
	FVector TraceStart, Forward;
	
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter,TraceStart, Forward)) return; // If fail, return.

	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,TraceStart,TraceEnd,ItemTraceChannel);

	LastActor = CurrentActor; // Value CurrentActor was last frame. Can be nullptr.
	CurrentActor = HitResult.GetActor();

	if (CurrentActor == LastActor) return;

	if (CurrentActor.IsValid()) // Weak Pointer wrapper function.
	{
		UE_LOG(LogTemp,Warning,TEXT("Started tracing a new Actor."));
	}

	if (LastActor.IsValid())
	{
		UE_LOG(LogTemp,Warning,TEXT("Stopped tracing last actor."));
	}
	
}

void ASInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogTemp, Display, TEXT("Primary Interact"));
}
