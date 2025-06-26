// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SInv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/SInv_Highlightable.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/SInv_HUDWidget.h"

ASInv_PlayerController::ASInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	TraceLength = 500.f;
	ItemTraceChannel = ECC_GameTraceChannel1;
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
	InventoryComponent = FindComponentByClass<USInv_InventoryComponent>(); // Even if its in BP, it will be found.
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
	EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ASInv_PlayerController::ToggleInventory);
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

void ASInv_PlayerController::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return; // weak pointer, needs to be validated.
	InventoryComponent->ToggleInventoryMenu();
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

	if (!CurrentActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage(); // Hide message if we are not tracking any actor.
	}
	
	if (CurrentActor == LastActor) return;

	if (CurrentActor.IsValid()) // Weak Pointer wrapper function.
	{
		if (UActorComponent* Highlightable = CurrentActor->FindComponentByInterface(USInv_Highlightable::StaticClass()))
		{
			ISInv_Highlightable::Execute_Highlight(Highlightable); // Executes interface function on Highlightable AC
		}
		
		UE_LOG(LogTemp,Warning,TEXT("Started tracing a new Actor."));
		USInv_ItemComponent* ItemComponent = CurrentActor->FindComponentByClass<USInv_ItemComponent>();
		
		if (!ItemComponent) return; // if its not valid, return
		
		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(USInv_Highlightable::StaticClass()))
		{
			ISInv_Highlightable::Execute_UnHighlight(Highlightable); // Executes interface function on Highlightable AC
		}
		UE_LOG(LogTemp,Warning,TEXT("Stopped tracing last actor."));
	}
	
}

void ASInv_PlayerController::PrimaryInteract()
{
	if (!CurrentActor.IsValid()) return;

	USInv_ItemComponent* ItemComponent = CurrentActor->FindComponentByClass<USInv_ItemComponent>();
	
	if (!IsValid(ItemComponent) || !InventoryComponent.IsValid()) return;
	InventoryComponent->TryAddItem(ItemComponent);
}
