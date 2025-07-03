// Developed by Javier Gonzalez [Lumina Division]


#include "InventoryManagement/Utils/SInv_InventoryStatics.h"


#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Types/SInv_GridTypes.h"

USInv_InventoryComponent* USInv_InventoryStatics::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!PlayerController) return nullptr;

	USInv_InventoryComponent* InventoryComponent = PlayerController->FindComponentByClass<USInv_InventoryComponent>();
	return InventoryComponent;
}

ESInv_ItemCategory USInv_InventoryStatics::GetItemCategoryFromItemComp(const USInv_ItemComponent* ItemComp)
{
	if (!IsValid(ItemComp)) return ESInv_ItemCategory::None;
	
	return ItemComp->GetItemManifest().GetItemCategory();
}
