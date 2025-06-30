// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Widgets/Inventory/GridSlots/SInv_GridSlot.h"
#include "Items/SInv_InventoryItem.h"
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

// Called from InventoryComponent::AddNewItem OnItemAdded.Broadcast()
void USInv_InventoryGrid::AddItem(USInv_InventoryItem* Item)
{
	if (!MatchesCategory(Item)) return;

	UE_LOG(LogTemp, Warning, TEXT("InventoryGrid::AddItem"));
}
