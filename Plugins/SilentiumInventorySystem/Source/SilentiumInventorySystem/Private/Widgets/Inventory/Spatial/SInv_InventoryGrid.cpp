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

/*----------------------------------------------------------------------------------*/
/*						Main Inventory Room Checker Function						*/
/*----------------------------------------------------------------------------------*/

FSInv_SlotAvailabilityResult USInv_InventoryGrid::HasRoomForItem(const FSInv_ItemManifest& Manifest)
{
	FSInv_SlotAvailabilityResult Result;

	// Determine if the Item is Stackable.
	const FSInv_StackableFragment* StackableFragment = Manifest.GetFragmentOfType<FSInv_StackableFragment>();
	Result.bStackable = StackableFragment != nullptr;
		
	// Determine how many stacks to add and Max Stack Size. If Stackable Fragment is null, then it's just 1.
	const int32 MaxStackSize = Result.bStackable ?  StackableFragment->GetMaxStackSize() : 1;
	int32 AmountToFill = Result.bStackable?  StackableFragment->GetStackCount() : 1;

	TSet<int32> CheckedIndices;
	
	// For Each Grid Slot:
	for (const auto& GridSlot : GridSlotsArray)
	{
		// If we don't have anymore to fill, break out of the loop early.
		if (AmountToFill == 0) break;
		
		// Is this Index claimed yet?
		if (IsIndexClaimed(CheckedIndices,GridSlot->GetIndex())) continue;

		// Is Item in Grid Bounds?
		if (!IsInGridBounds(GridSlot->GetSlotIndex(), GetItemDimensions(Manifest))) continue;
		
		/*-------------------------------------*/
		/*		Main Grid Check Section		  */
		/*------------------------------------*/
		
		// Can the item fit here? (Grid Dimensions, out of grid bounds?)
		TSet<int32> TentativelyClaimed;
		if (!HasRoomAtIndex(GridSlot,
							GetItemDimensions(Manifest),
							CheckedIndices,
							TentativelyClaimed,
							Manifest.GetItemType(),
							MaxStackSize))
		{
			continue; // If there's no room there, skip.
		}
		// How much to fill?
		const int32 AmountToFillInSlot = DetermineFillAmountForSlot(Result.bStackable,
																	MaxStackSize,
																	AmountToFill,
																	GridSlot);
		if (AmountToFillInSlot == 0) continue;
		
		CheckedIndices.Append(TentativelyClaimed);
		
		// Update the amount left to fill
		Result.TotalRoomToFill += AmountToFillInSlot;
		Result.SlotAvailabilities.Emplace(
			FSInv_SlotAvailability
			{
				HasValidItem(GridSlot) ? GridSlot->GetUpperLeftSlotIndex() : GridSlot->GetIndex(),
				Result.bStackable ? AmountToFillInSlot : 0,
				HasValidItem(GridSlot)
			}
		);
		AmountToFill -= AmountToFillInSlot;
		
		// How much is the remainder?
		Result.Remainder = AmountToFill;
		
		if (AmountToFill == 0) return Result;
	}
	
	return Result;
}

bool USInv_InventoryGrid::IsIndexClaimed(const TSet<int32>& CheckedIndices, const int32 Index) const
{
	return CheckedIndices.Contains(Index);	
}



FIntPoint USInv_InventoryGrid::GetItemDimensions(const FSInv_ItemManifest& ItemManifest) const
{
	const FSInv_GridFragment* GridFragment = ItemManifest.GetFragmentOfType<FSInv_GridFragment>();
	return GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
}

/*----------------------------------------------------------------------------------*/
/*					Checks all the slots the Item might occupy						*/
/*----------------------------------------------------------------------------------*/

bool USInv_InventoryGrid::HasRoomAtIndex(const USInv_GridSlot* GridSlot,
                                         const FIntPoint& ItemDimensions,
                                         const TSet<int32>& CheckedIndices,
                                         TSet<int32>& OutTentativelyClaimed,
                                         const FGameplayTag& ItemType,
                                         const int32 MaxStackSize)
{
	// Is there room at this index? (i.e. are the other items in the way?)
	bool bHasRoomAtIndex {true};
	
	// Lambda Use
	USInv_InventoryStatics::ForEach2D(GridSlotsArray,GridSlot->GetSlotIndex(),ItemDimensions,Columns,
		[&](const USInv_GridSlot* SubGridSlot)
			{
				if (CheckSlotConstraints(GridSlot,SubGridSlot,CheckedIndices, OutTentativelyClaimed, ItemType, MaxStackSize))
				{
					OutTentativelyClaimed.Add(SubGridSlot->GetIndex()); // This is being added in CheckslotConstraints..must remove
				}
				else
				{
					bHasRoomAtIndex = false;
				}
			}
		);
	return bHasRoomAtIndex;
}

/*-----------------------------------------------------------------------------*/
/*					Individual Grid Slot Checker						   	   */
/*	This is called by HasRoomAtIndex ForEach2D for loop for each Grid Slot     */
/*						the Item might occupy							      */
/*----------------------------------------------------------------------------*/

bool USInv_InventoryGrid::CheckSlotConstraints(
						  const USInv_GridSlot* GridSlot,
						  const USInv_GridSlot* SubGridSlot,
						  const TSet<int32>& CheckedIndices,
						  TSet<int32>& OutTentativelyClaimed,
						  const FGameplayTag& ItemType,
						  const int32 MaxStackSize) const
{
	// Index claimed?
	if (IsIndexClaimed(CheckedIndices,SubGridSlot->GetIndex())) return false;
	
	// Has valid item? If not, this index might fit our needs.
	if (!HasValidItem(SubGridSlot))
	{
		OutTentativelyClaimed.Add(SubGridSlot->GetIndex());
		return true;
	}

	// Is this Grid Slot an Upper left slot?
	if (!IsUpperLeftSlot(GridSlot,SubGridSlot)) return false;
	
	// If so, is this a stackable item?
	const USInv_InventoryItem* SubItem = SubGridSlot->GetInventoryItem().Get();
	if (!SubItem->IsStackable()) return false;
	
	// Is this Item the same type as the item we're trying to add?
	if (!DoesItemTypeMatch(SubItem,ItemType)) return false;
	
	// If stackable, is this slot at the Max Stack Size already?
	if (GridSlot->GetStackCount() >= MaxStackSize) return false;
	
	return true; // If we made it to this point, we can say "yes, it's a valid Slot"
}

bool USInv_InventoryGrid::IsUpperLeftSlot(const USInv_GridSlot* GridSlot, const USInv_GridSlot* SubGridSlot) const
{
	return SubGridSlot->GetUpperLeftSlotIndex() == GridSlot->GetIndex();
}

bool USInv_InventoryGrid::DoesItemTypeMatch(const USInv_InventoryItem* SubItem, const FGameplayTag& ItemType) const
{
	return SubItem->GetItemManifest().GetItemType().MatchesTagExact(ItemType);
}

bool USInv_InventoryGrid::IsInGridBounds(const int32 StartIndex, const FIntPoint& ItemDimensions) const
{
	// First check if we are out of bounds
	if (StartIndex < 0 || StartIndex >= GridSlotsArray.Num()) return false;
	
	const int32 EndColumn = (StartIndex % Columns) + ItemDimensions.X;
	const int32 EndRow = (StartIndex / Columns) + ItemDimensions.Y;
	return EndColumn <= Columns && EndRow <= Rows;
}

int32 USInv_InventoryGrid::DetermineFillAmountForSlot(const bool bStackable, const int32 MaxStackSize,
	const int32 AmountToFill, const USInv_GridSlot* GridSlot) const
{
	// Calculate Room in the Slot
	const int32 RoomInSlot = MaxStackSize - GetStackAmount(GridSlot);
	
	// if stackable, we need the minimum between AmountToFill and RoomInSlot
	return bStackable ? FMath::Min(AmountToFill, RoomInSlot) : 1;
}

int32 USInv_InventoryGrid::GetStackAmount(const USInv_GridSlot* GridSlot) const
{
	int32 CurrentSlotStackCount = GridSlot->GetStackCount();
	
	// If we are at a slot that doesn't hold the stack count, we must get the actual stack count.
	// that means it's not the upper left slot.
	if (const int32 UpperLeftSlotIndex = GridSlot->GetUpperLeftSlotIndex();
		UpperLeftSlotIndex != INDEX_NONE)
	{
		USInv_GridSlot* UpperLeftGridSlot = GridSlotsArray[UpperLeftSlotIndex];
		CurrentSlotStackCount = UpperLeftGridSlot->GetStackCount();
	}
	return CurrentSlotStackCount;
}

bool USInv_InventoryGrid::HasValidItem(const USInv_GridSlot* GridSlot) const
{
	return GridSlot->GetInventoryItem().IsValid(); // Need .IsValid as it's a WeakObjectPtr
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
