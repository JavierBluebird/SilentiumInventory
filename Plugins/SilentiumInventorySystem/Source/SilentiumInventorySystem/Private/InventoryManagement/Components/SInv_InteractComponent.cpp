// Developed by Javier Gonzalez [Lumina Division]


#include "InventoryManagement/Components/SInv_InteractComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/SInv_Highlightable.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/HUD/SInv_HUDWidget.h"


// Sets default values for this component's properties
class UEnhancedInputLocalPlayerSubsystem;

USInv_InteractComponent::USInv_InteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	TraceLength = 500.0f;
	ItemTraceChannel = ECC_GameTraceChannel1;
}

void USInv_InteractComponent::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent();
	CreateHUDWidget();
	InventoryComponent = GetOwner()->FindComponentByClass<USInv_InventoryComponent>();
}

// *********************************************************************************************************
//   Actor Components do not have a SetupInputComponent function that can be overriden.
//   Therefore, this is defined manually and called from BeginPlay.
//   Get the PlayerController this is attached to and then bind actions to the PC's Enhanced Input Component
// *********************************************************************************************************

void USInv_InteractComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForItem();
}
void USInv_InteractComponent::SetupInputComponent()
{
	if (const APlayerController* PC = Cast<APlayerController>(GetOwner()))
	{
		if (const ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* EISubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			{
				EISubSystem->AddMappingContext(DefaultIMC, 0);
			}
		}

		// Retrieve the EnhancedInputComponent from the PC
		if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PC->InputComponent))
		{
			// now bind exactly as we did in SetupInputComponent on the PC
			EIC->BindAction(PrimaryInteractAction, ETriggerEvent::Started,this, &ThisClass::PrimaryInteract);
			EIC->BindAction(ToggleInventoryAction, ETriggerEvent::Started,this, &ThisClass::ToggleInventory);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Inv_InteractionComponent: Owning PlayerController has no EnhancedInputComponent!"));
		}
	}
}

void USInv_InteractComponent::ToggleInventory()
{
	if (!InventoryComponent.IsValid()) return;
	InventoryComponent->ToggleInventoryMenu();
	if (InventoryComponent->IsMenuOpen())
	{
		HUDWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		HUDWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void USInv_InteractComponent::PrimaryInteract()
{
	//UE_LOG(LogTemp, Warning, TEXT("PrimaryInteract pressed."));
	if (!CurrentActor.IsValid()) return;

	USInv_ItemComponent* ItemComp = CurrentActor->FindComponentByClass<USInv_ItemComponent>();
	if (!IsValid(ItemComp) || !InventoryComponent.IsValid()) return;

	InventoryComponent->TryAddItem(ItemComp);
}

void USInv_InteractComponent::CreateHUDWidget()
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC->IsLocalController()) return; 
	
	HUDWidget = CreateWidget<USInv_HUDWidget>(PC, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void USInv_InteractComponent::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;

	const APlayerController* PC = Cast<APlayerController>(GetOwner());
	
	FVector2D ViewportSize;
	FVector TraceStart;
	FVector Forward;
	
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize/2;
	
	if (!UGameplayStatics::DeprojectScreenToWorld(PC, ViewportCenter, TraceStart, Forward)) return;

	const FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);

	LastActor = CurrentActor;
	CurrentActor = HitResult.GetActor();

	//if not focusing on a valid Item, then hide the pickup message
	if (!CurrentActor.IsValid())
	{
		if (IsValid(HUDWidget)) HUDWidget->HidePickupMessage();
	}

	if (CurrentActor.IsValid())
	{
		if (UActorComponent* Highlightable = CurrentActor->FindComponentByInterface(USInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			ISInv_Highlightable::Execute_Highlight(Highlightable);
		}
		
		USInv_ItemComponent* ItemComponent = CurrentActor->FindComponentByClass<USInv_ItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(HUDWidget)) HUDWidget->ShowPickupMessage(ItemComponent->GetPickupMessage());		
	}
	
	if (CurrentActor==LastActor) return;

	if (CurrentActor.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Started Tracing: %s"), *CurrentActor->GetName());
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* Highlightable = LastActor->FindComponentByInterface(USInv_Highlightable::StaticClass()); IsValid(Highlightable))
		{
			ISInv_Highlightable::Execute_UnHighlight(Highlightable);
		}
		//UE_LOG(LogTemp, Warning, TEXT("Stopped tracing: %s"), *LastActor->GetName());
	}
}

