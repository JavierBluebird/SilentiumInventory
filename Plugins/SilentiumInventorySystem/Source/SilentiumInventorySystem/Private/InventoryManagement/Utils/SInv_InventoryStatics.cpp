// Developed by Javier Gonzalez [Lumina Division]


#include "InventoryManagement/Utils/SInv_InventoryStatics.h"

#include "InventoryManagement/Components/SInv_InventoryComponent.h"

USInv_InventoryComponent* USInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!PlayerController) return nullptr;

	USInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<USInv_InventoryComponent>();
	return InventoryComponent;
}
