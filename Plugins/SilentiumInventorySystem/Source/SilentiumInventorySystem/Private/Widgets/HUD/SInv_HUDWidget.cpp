// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/HUD/SInv_HUDWidget.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Widgets/HUD/SInv_InfoMessage.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"

void USInv_HUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	USInv_InventoryComponent* InventoryComponent = USInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());
	if (IsValid(InventoryComponent))
	{
		InventoryComponent->NoRoomInInventory.AddDynamic(this, &ThisClass::OnNoInventoryRoom);
	}
}

void USInv_HUDWidget::OnNoInventoryRoom()
{
	if (!IsValid(InfoMessageWidget)) return;

	InfoMessageWidget->SetMessage(FText::FromString(TEXT("No Room in Inventory.")));
}
