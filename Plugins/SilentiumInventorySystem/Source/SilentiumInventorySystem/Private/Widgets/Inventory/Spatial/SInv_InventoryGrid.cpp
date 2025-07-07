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
#include "Items/Fragments/SInv_FragmentTags.h"
#include "Items/Fragments/SInv_ItemFragment.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "Widgets/Inventory/SlottedItems/SInv_SlottedItem.h"
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
	Result.bStackable = true;
	

	FSInv_SlotAvailability SlotAvailability;
	SlotAvailability.AmountToFill = 2;
	SlotAvailability.SlotIndex = 0;

	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability)); // Moves the struct Slot Availability into Result's SlotAvailability struct array

	FSInv_SlotAvailability SlotAvailability2;
	SlotAvailability2.AmountToFill = 5;
	SlotAvailability2.SlotIndex = 1;

	Result.SlotAvailabilities.Add(MoveTemp(SlotAvailability2)); // Moves the struct Slot Availability into Result's SlotAvailability struct array

	
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
	for (const auto& Availability : Result.SlotAvailabilities)
	{
		AddItemAtIndex(NewItem,Availability.SlotIndex,Result.bStackable, Availability.AmountToFill);
		UpdateGridSlots(NewItem,Availability.SlotIndex,Result.bStackable, Availability.AmountToFill);
	}
}

/*------------------------------------------*/
/*			Creates Slotted Item			*/
/*------------------------------------------*/
void USInv_InventoryGrid::AddItemAtIndex(USInv_InventoryItem* NewItem, const int32 Index, const bool bStackable, const int32 StackAmount)
{
	// Get Grid Fragment so we know how many grid spaces the item takes.
	const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	// Get Image Fragment, so we have the Icon to display.
	const FSInv_ImageFragment* ImageFragment = GetFragment<FSInv_ImageFragment>(NewItem, FragmentTags::ImageFragment);

	if (!GridFragment || !ImageFragment) return; // Both are necessary, return if not valid.
	
	USInv_SlottedItem* SlottedItem = CreateSlottedItem(NewItem,bStackable,StackAmount,GridFragment,ImageFragment,Index); // Creates Slotted Item

	// Add Slotted Item to the canvas panel.
	AddSlottedItemToCanvas(Index, GridFragment, SlottedItem);
	// Store the new Widget in a container.
	SlottedItems.Add(Index, SlottedItem);
}

USInv_SlottedItem* USInv_InventoryGrid::CreateSlottedItem(USInv_InventoryItem* Item, const bool bStackable, const int32 StackAmount,
	const FSInv_GridFragment* GridFragment, const FSInv_ImageFragment* ImageFragment, const int32 Index) const
{
	USInv_SlottedItem* SlottedItem = CreateWidget<USInv_SlottedItem>(GetOwningPlayer(), SlottedItemClass);
	SlottedItem->SetInventoryItem(Item);
	SetSlottedItemImage(SlottedItem,GridFragment,ImageFragment);
	SlottedItem->SetGridIndex(Index);

	// Stackable Settings
	SlottedItem->SetIsStackable(bStackable);
	const int32 StackUpdateAmount = bStackable ? StackAmount : 0;
	SlottedItem->UpdateStackCount(StackUpdateAmount);
	
	return SlottedItem;
}

/*------------------------------------------*/
/*		Adds Slotted Item to Canvas			*/
/*------------------------------------------*/
void USInv_InventoryGrid::AddSlottedItemToCanvas(const int32 SlotIndex, const FSInv_GridFragment* GridFragment,
	USInv_SlottedItem* SlottedItem) const
{
	CanvasPanel->AddChild(SlottedItem);
	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(SlottedItem);
	CanvasSlot->SetSize(GetDrawSize(GridFragment));

	const FVector2D DrawPos = USInv_WidgetUtils::GetPositionFromIndex(SlotIndex, Columns) * SlotSize;
	const FVector2D DrawPosWithPadding = DrawPos + FVector2D(GridFragment->GetGridPadding());
	
	CanvasSlot->SetPosition(DrawPosWithPadding);
}

void USInv_InventoryGrid::UpdateGridSlots(USInv_InventoryItem* NewItem, const int32 SlotIndex, bool bStackableItem, const int32 StackAmount)
{
	check(GridSlotsArray.IsValidIndex(SlotIndex)); // Safety Check

	if (bStackableItem)
	{
		GridSlotsArray[SlotIndex]->SetStackCount(StackAmount); // Will be Upper Left Index
	}
	
	
	const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(NewItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	// If we don't have a Grid Fragment, we assume it's 1x1 size.
	const FIntPoint GridDimensions =  GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);

	USInv_InventoryStatics::ForEach2D(GridSlotsArray,SlotIndex,GridDimensions,Columns,
	[&](USInv_GridSlot* GridSlot)
				{
					GridSlot->SetInventoryItem(NewItem);
					GridSlot->SetUpperLeftSlotIndex(SlotIndex);
					GridSlot->SetOccupiedTexture();
					GridSlot->SetAvailable(false);
				}
	);
}

void USInv_InventoryGrid::SetSlottedItemImage(const USInv_SlottedItem* SlottedItem,
                                              const FSInv_GridFragment* GridFragment,
                                              const FSInv_ImageFragment* ImageFragment) const
{
	FSlateBrush Brush;
	Brush.SetResourceObject(ImageFragment->GetItemIcon());
	Brush.DrawAs = ESlateBrushDrawType::Image;
	Brush.ImageSize = GetDrawSize(GridFragment);
	SlottedItem->SetImageBrush(Brush);
}

FVector2D USInv_InventoryGrid::GetDrawSize(const FSInv_GridFragment* GridFragment) const
{
	const float IconTileWidth = SlotSize - GridFragment->GetGridPadding() * 2; // Left and right Padding
	FVector2D IconSize = GridFragment->GetGridSize() * IconTileWidth;

	return IconSize;
}
