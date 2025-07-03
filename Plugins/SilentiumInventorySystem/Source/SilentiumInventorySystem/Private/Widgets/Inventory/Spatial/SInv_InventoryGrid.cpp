// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Widgets/Inventory/GridSlots/SInv_GridSlot.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "Widgets/Utils/SInv_WidgetUtils.h"


void USInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ConstructGrid();
	InventoryComponent = USInv_InventoryStatics::
						GetInventoryComponent(GetOwningPlayer());

	InventoryComponent->OnItemAdded.AddDynamic(this,&ThisClass::AddItem);
}

void USInv_InventoryGrid::ConstructGrid()
{
	GridSlotsArray.Reserve(Rows * Columns); // Reserves memory beforehand.

	for (int32 j = 0; j < Rows; ++j)
	{
		for (int32 i = 0; i < Columns; ++i)
		{
			// Slot Widget Object Creation
			USInv_GridSlot* GridSlot = CreateWidget<USInv_GridSlot>(this,GridSlotClass);
			CanvasPanel->AddChildToCanvas(GridSlot);
			
			// Slot Index Calculation
			FIntPoint SlotPosition = FIntPoint(i, j);
			GridSlot->SetSlotIndex(USInv_WidgetUtils::GetIndexFromPosition(SlotPosition, Columns));

			// Slot Size Calculation within the Canvas Panel
			UCanvasPanelSlot* GridCanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(GridSlot);
			GridCanvasPanelSlot->SetSize(FVector2D(SlotSize)); // FVector Constructor initializes XY to the parameter value.
			
			// Slot Position Calculation within the Canvas Panel
			GridCanvasPanelSlot->SetPosition(SlotPosition * SlotSize);

			// Stores the created Slot in GridSlot array
			GridSlotsArray.Add(GridSlot);
		}
	}
}

bool USInv_InventoryGrid::MatchesCategory(const USInv_InventoryItem* Item) const
{
	return Item->GetItemManifest().GetItemCategory() == ItemCategory;
}

/*------------------------------------------*/
/*		HasRoomForItem overloads			*/
/*------------------------------------------*/
FSInv_SlotAvailabilityResult USInv_InventoryGrid::HasRoomForItem(const USInv_ItemComponent* ItemComponent)
{
	return HasRoomForItem(ItemComponent->GetItemManifest());
}

FSInv_SlotAvailabilityResult USInv_InventoryGrid::HasRoomForItem(const USInv_InventoryItem* Item)
{
	return HasRoomForItem(Item->GetItemManifest());
}

FSInv_SlotAvailabilityResult USInv_InventoryGrid::HasRoomForItem(const FSInv_ItemManifest& Manifest)
{
	FSInv_SlotAvailabilityResult Result;
	Result.TotalRoomToFill = 1;

	FSInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 1;
	SlotAvailability.SlotIndex = 0;
	
	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability)); // Moves the struct Slot Availability into Result's SlotAvailability struct array
	
	return Result;
}

/*------------------------------------------*/
/*			Adding Items to Grid			*/
/*------------------------------------------*/

// Called from InventoryComponent::AddNewItem OnItemAdded.Broadcast()

void USInv_InventoryGrid::AddItem(USInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	FSInv_SlotAvailabilityResult Result = HasRoomForItem(Item);

	AddItemToIndices(Result, Item);
}

void USInv_InventoryGrid::AddItemToIndices(const FSInv_SlotAvailabilityResult& Result, USInv_InventoryItem* NewItem)
{
	// Get Grid Fragment so we know how many grid spaces the item takes.
	// Get Image Fragment, so we have the Icon to display.

	// Create a Widget to add to the grid

	// Store the new Widget in a container.
}
