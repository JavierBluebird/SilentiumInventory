// Developed by Javier Gonzalez [Lumina Division]


#include "InventoryManagement/Utils/SInv_InventoryStatics.h"


#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Types/SInv_GridTypes.h"
#include "Widgets/Inventory/InventoryBase/SInv_InventoryBase.h"

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

void USInv_InventoryStatics::ItemHovered(APlayerController* PC, USInv_InventoryItem* Item)
{
	USInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	USInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;

	if (InventoryBase->HasHoverItem()) return;
	InventoryBase->OnItemHovered(Item);
}

void USInv_InventoryStatics::ItemUnhovered(APlayerController* PC)
{
	USInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return;

	USInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return;
	InventoryBase->OnItemUnhovered();
}

USInv_HoverItem* USInv_InventoryStatics::GetHoveredItem(APlayerController* PC)
{
	USInv_InventoryComponent* IC = GetInventoryComponent(PC);
	if (!IsValid(IC)) return nullptr;

	USInv_InventoryBase* InventoryBase = IC->GetInventoryMenu();
	if (!IsValid(InventoryBase)) return nullptr;

	return InventoryBase->GetHoverItem();
}
