// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInv_InventoryComponent.generated.h"


class USInv_InventoryItem;
class USInv_InventoryBase;

// Delegate of Item Added or Removed on FastArray.cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, USInv_InventoryItem*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USInv_InventoryComponent();
	
	void ToggleInventoryMenu();
	
	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;

protected:
	virtual void BeginPlay() override;

private:
	
	void ConstructInventory();

	TWeakObjectPtr<APlayerController> OwningController;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System|HUD ")
	TSubclassOf<USInv_InventoryBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<USInv_InventoryBase> InventoryMenuReference;

	// Inventory State Related variables
	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();


};
