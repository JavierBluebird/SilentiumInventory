// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_SpatialInventory.h"

#include "SilentiumInventorySystem.h"
#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"


void USInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// ------------------------------//
	// Button callbacks Binding      //
	// -----------------------------//
	Button_Consumables->OnClicked.AddDynamic(this,&ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this,&ThisClass::ShowCraftables);
	Button_Equippables->OnClicked.AddDynamic(this,&ThisClass::ShowEquippables);

	ShowEquippables();
}

FSInv_SlotAvailabilityResult USInv_SpatialInventory::HasRoomForItem(USInv_ItemComponent* ItemComponent) const
{
	switch (USInv_InventoryStatics::GetItemCategoryFromItemComp(ItemComponent))
	{
		case ESInv_ItemCategory::Equippable :
			return Grid_Equippables->HasRoomForItem(ItemComponent);
		
		case ESInv_ItemCategory::Consumable :
			return Grid_Consumables->HasRoomForItem(ItemComponent);

		case ESInv_ItemCategory::Craftable :
			return Grid_Craftables->HasRoomForItem(ItemComponent);

		default:
			 // This will be an error, hence we will log it in our custom Log Channel
			UE_LOG(LogSilentiumInventory, Error, TEXT("SInv_SpatialInventory: ItemComponent doesn't have a valid Item Category "));
			return FSInv_SlotAvailabilityResult();
	}
}


void USInv_SpatialInventory::ShowEquippables()
{
	SetActiveGrid(Grid_Equippables, Button_Equippables);
}

void USInv_SpatialInventory::ShowConsumables()
{
	SetActiveGrid(Grid_Consumables, Button_Consumables);
}

void USInv_SpatialInventory::ShowCraftables()
{
	SetActiveGrid(Grid_Craftables, Button_Craftables);
}

void USInv_SpatialInventory::SetActiveGrid(USInv_InventoryGrid* Grid, UButton* Button)
{
	DisableButton(Button);

	Switcher->SetActiveWidget(Grid);
}

void USInv_SpatialInventory::DisableButton(UButton* Button)
{
	Button_Equippables->SetIsEnabled(true);
	Button_Consumables->SetIsEnabled(true);
	Button_Craftables->SetIsEnabled(true);
	Button->SetIsEnabled(false);
}

