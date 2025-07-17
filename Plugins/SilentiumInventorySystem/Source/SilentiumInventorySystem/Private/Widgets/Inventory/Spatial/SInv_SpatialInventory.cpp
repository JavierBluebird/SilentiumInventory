// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_SpatialInventory.h"

#include "SilentiumInventorySystem.h"
#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Widgets/ItemDescription/SInv_ItemDescription.h"


void USInv_SpatialInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// ------------------------------//
	// Button callbacks Binding      //
	// -----------------------------//
	Button_Consumables->OnClicked.AddDynamic(this,&ThisClass::ShowConsumables);
	Button_Craftables->OnClicked.AddDynamic(this,&ThisClass::ShowCraftables);
	Button_Equippables->OnClicked.AddDynamic(this,&ThisClass::ShowEquippables);

	Grid_Equippables->SetOwningCanvas(CanvasPanel);
	Grid_Consumables->SetOwningCanvas(CanvasPanel);
	Grid_Craftables->SetOwningCanvas(CanvasPanel);
	
	ShowEquippables();
}

FReply USInv_SpatialInventory::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ActiveGrid->DropItem();
	return FReply::Handled();
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

void USInv_SpatialInventory::OnItemHovered(USInv_InventoryItem* InventoryItem)
{
	USInv_ItemDescription* DescriptionWidget = GetItemDescription();
	DescriptionWidget->SetVisibility(ESlateVisibility::Collapsed);

	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer); // If it's active, deactivate first.
	
	FTimerDelegate DescriptionTimerDelegate;
	DescriptionTimerDelegate.BindLambda([this]() // Defines the delegate functionality
		{
			GetItemDescription()->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
	);
	GetOwningPlayer()->GetWorldTimerManager().SetTimer(DescriptionTimer,  // Actually Starts timer
														  DescriptionTimerDelegate,
														  DescriptionTimerDelay,
														  false);
}

void USInv_SpatialInventory::OnItemUnhovered()
{
	GetItemDescription()->SetVisibility(ESlateVisibility::Collapsed);
	GetOwningPlayer()->GetWorldTimerManager().ClearTimer(DescriptionTimer);
}

bool USInv_SpatialInventory::HasHoverItem() const
{
	if (Grid_Equippables->HasHoverItem()) return true;
	if (Grid_Craftables->HasHoverItem()) return true;
	if (Grid_Consumables->HasHoverItem()) return true;

	return false;
}

USInv_ItemDescription* USInv_SpatialInventory::GetItemDescription()
{
	if (!IsValid(ItemDescription))
	{
		ItemDescription = CreateWidget<USInv_ItemDescription>(GetOwningPlayer(),ItemDescriptionClass);
		CanvasPanel->AddChild(ItemDescription);
	}
	return ItemDescription;
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

	if (ActiveGrid.IsValid()) ActiveGrid->HideCursor(); // Hide, then update and show cursor

	ActiveGrid = Grid;
	if (ActiveGrid.IsValid()) ActiveGrid->ShowCursor();
	
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

