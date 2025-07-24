// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInv_InteractComponent.generated.h"


class USInv_InventoryComponent;
class USInv_HUDWidget;
class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_InteractComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	
	USInv_InteractComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Silentium Inventory")
	void ToggleInventory();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent();

private:
	void PrimaryInteract();
	void CreateHUDWidget();
	void TraceForItem(); // Item tracing function

	TWeakObjectPtr<USInv_InventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TObjectPtr<UInputMappingContext> DefaultIMC;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TObjectPtr<UInputAction> PrimaryInteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Input Settings")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	//assign the class for our HUD Widget in the editor
	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|HUD Settings")
	TSubclassOf<USInv_HUDWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<USInv_HUDWidget> HUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Trace Settings")
	double TraceLength;

	// Cannot expose an ENum directly for changing in the editor with UPROPERTY. It muSt be wrapped in a
	// TEnumAsByte<> for it to appear as a list.
	UPROPERTY(EditDefaultsOnly, Category = "Silentium Inventory|Trace Settings")
	TEnumAsByte<ECollisionChannel> ItemTraceChannel;

	TWeakObjectPtr<AActor> CurrentActor; // Actor hit this frame
	TWeakObjectPtr<AActor> LastActor; // Actor hit last frame
};
