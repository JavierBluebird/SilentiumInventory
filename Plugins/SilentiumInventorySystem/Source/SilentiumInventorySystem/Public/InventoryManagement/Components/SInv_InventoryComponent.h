// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryManagement/FastArray/SInv_FastArray.h"
#include "SInv_InventoryComponent.generated.h"


struct FSInv_SlotAvailabilityResult;
class USInv_ItemComponent;
class USInv_InventoryItem;
class USInv_InventoryBase;

// Delegate of Item Added or Removed on FastArray.cpp
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemChanged, USInv_InventoryItem*, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoRoomInInventory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStackChange, const FSInv_SlotAvailabilityResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemEquipStatusChanged, USInv_InventoryItem*, Item);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class SILENTIUMINVENTORYSYSTEM_API USInv_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	USInv_InventoryComponent();

	UFUNCTION(BlueprintCallable,BlueprintAuthorityOnly, Category = "Silentium Inventory")
	void TryAddItem(USInv_ItemComponent* ItemComponent);
	
	void AddRepSubObj(UObject* SubObj);
	void ToggleInventoryMenu();
	void SpawnDroppedItem(USInv_InventoryItem* Item, const int32 StackCount);
	USInv_InventoryBase* GetInventoryMenu() const;
	
	/*-------------------------------*/
	/*			Server RPCs			 */
	/*-------------------------------*/
	UFUNCTION(Server,Reliable)
	void Server_AddNewItem(USInv_ItemComponent* ItemComponent, int32 StackCount);

	UFUNCTION(Server,Reliable)
	void Server_AddStacksToItem(USInv_ItemComponent* ItemComponent, int32 StackCount, int32 Remainder);

	UFUNCTION(Server,Reliable)
	void Server_DropItem(USInv_InventoryItem* Item, int32 StackCount);

	UFUNCTION(Server,Reliable)
	void Server_ConsumeItem(USInv_InventoryItem* Item);

	UFUNCTION(Server,Reliable)
	void Server_EquipSlotClicked(USInv_InventoryItem* ItemToEquip, USInv_InventoryItem* ItemToUnequip);

	UFUNCTION(NetMulticast,Reliable)
	void Multicast_EquipSlotClicked(USInv_InventoryItem* ItemToEquip, USInv_InventoryItem* ItemToUnequip);
	
	/*-------------------------------*/
	/*		Inventory Delegates		 */
	/*-------------------------------*/
	FInventoryItemChanged OnItemAdded;
	FInventoryItemChanged OnItemRemoved;
	FNoRoomInInventory NoRoomInInventory;
	FStackChange OnStackChange;
	FItemEquipStatusChanged OnItemEquipped;
	FItemEquipStatusChanged OnItemUnequipped;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

private:
	
	void ConstructInventory();

	//-----------------------
	// FAST ARRAY SERIALIZER
	//-----------------------
	UPROPERTY(Replicated)
	FSInv_InventoryFastArray InventoryList;
	
	TWeakObjectPtr<APlayerController> OwningController;
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | HUD ")
	TSubclassOf<USInv_InventoryBase> InventoryMenuClass;

	UPROPERTY()
	TObjectPtr<USInv_InventoryBase> InventoryMenuReference;

	// Inventory State Related variables
	bool bInventoryMenuOpen;
	void OpenInventoryMenu();
	void CloseInventoryMenu();

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | Drop Item Settings")
	float DropSpawnAngleMin = -85.f;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | Drop Item Settings")
	float DropSpawnAngleMax = 85.f;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | Drop Item Settings")
	float DropSpawnDistanceMin = -10.f;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | Drop Item Settings")
	float DropSpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory System | Drop Item Settings")
	float RelativeSpawnElevation = 70.f;
};
