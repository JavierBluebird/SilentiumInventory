// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Spatial/SInv_InventoryGrid.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetInteractionComponent.h"
#include "InventoryManagement/Components/SInv_InventoryComponent.h"
#include "InventoryManagement/Utils/SInv_InventoryStatics.h"
#include "Widgets/Inventory/GridSlots/SInv_GridSlot.h"
#include "Items/SInv_InventoryItem.h"
#include "Items/Components/SInv_ItemComponent.h"
#include "Items/Fragments/SInv_FragmentTags.h"
#include "Items/Fragments/SInv_ItemFragment.h"
#include "Items/Manifest/SInv_ItemManifest.h"
#include "Widgets/Inventory/HoverItem/SInv_HoverItem.h"
#include "Widgets/Inventory/SlottedItems/SInv_SlottedItem.h"
#include "Widgets/ItemPopUp/SInv_ItemPopUp.h"
#include "Widgets/Inventory/Utils/SInv_WidgetUtils.h"


void USInv_InventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ConstructGrid();
	InventoryComponent = USInv_InventoryStatics::GetInventoryComponent(GetOwningPlayer());

	InventoryComponent->OnItemAdded.AddDynamic(this,&ThisClass::AddItem);
	InventoryComponent->OnStackChange.AddDynamic(this,&ThisClass::AddStacks);
}

void USInv_InventoryGrid::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const FVector2D CanvasPos = USInv_WidgetUtils::GetWidgetPosition(CanvasPanel);
	const FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	if (CursorExitedCanvas(CanvasPos, USInv_WidgetUtils::GetWidgetSize(CanvasPanel),MousePos))
	{
		return;
	}
	UpdateTileParameters(CanvasPos, MousePos);
}

bool USInv_InventoryGrid::CursorExitedCanvas(const FVector2D& BoundaryPos, const FVector2D& BoundarySize,
	const FVector2D& Location)
{
	bLastMouseWithinCanvas = bMouseWithinCanvas;
	bMouseWithinCanvas = USInv_WidgetUtils::IsWithinBounds(BoundaryPos,BoundarySize,Location);

	if (!bMouseWithinCanvas && bLastMouseWithinCanvas) // last frame we were within canvas.
	{
		UnHighlightSlots(LastHighlightedIndex,LastHighlightedDimensions);
		return true;
	}
	return false;
}

void USInv_InventoryGrid::HighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	
	UnHighlightSlots(LastHighlightedIndex, Dimensions);
	if (!bMouseWithinCanvas) return; // If not within canvas, we don't need to highlight.

	USInv_InventoryStatics::ForEach2D(GridSlotsArray, Index, Dimensions,Columns,
		[&](USInv_GridSlot* GridSlot)
		{
			GridSlot->SetOccupiedTexture();
		});
	LastHighlightedDimensions = Dimensions;
	LastHighlightedIndex = Index;
}

void USInv_InventoryGrid::UnHighlightSlots(const int32 Index, const FIntPoint& Dimensions)
{
	USInv_InventoryStatics::ForEach2D(GridSlotsArray, Index, Dimensions,Columns,
		[&](USInv_GridSlot* GridSlot)
		{
			if (GridSlot->IsAvailable())
			{
				GridSlot->SetUnoccupiedTexture();
			}
			else
			{
				GridSlot->SetOccupiedTexture();
			}
		});
}

void USInv_InventoryGrid::UpdateTileParameters(const FVector2D& CanvasPos, const FVector2D& MousePos)
{
	// if Mouse not in canvas panel, return.
	if (!bMouseWithinCanvas) return;
	
	// Calculate the Tile Quadrant, tile index, and coordinates
	const FIntPoint HoveredTileCoordinates = CalculateHoveredCoordinates(CanvasPos, MousePos);

	LastTileParameters = TileParameters;
	TileParameters.TileCoordinates = HoveredTileCoordinates;
	TileParameters.TileIndex = USInv_WidgetUtils::GetIndexFromPosition(HoveredTileCoordinates, Columns);
	
	TileParameters.TileQuadrant = CalculateTileQuadrant(CanvasPos, MousePos);
	
	// Handle Highlight/Unhighlight of the grid slot
	OnTileParametersUpdate(TileParameters);
}

void USInv_InventoryGrid::OnTileParametersUpdate(const FSInv_TileParameters& Parameters)
{
	if (!IsValid(HoverItem)) return;

	// Get Hover Item's dimensions
	const FIntPoint Dimensions = HoverItem->GetGridDimensions();
	
	// Calculate the starting coordinate for highlighting
	const FIntPoint StartingCoordinate = CalculateStartingCoordinates(Parameters.TileCoordinates, Dimensions, Parameters.TileQuadrant);
	ItemDropIndex = USInv_WidgetUtils::GetIndexFromPosition(StartingCoordinate, Columns);

	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);
	// Check Hover Position
	CurrentQueryResult = CheckHoverPosition(StartingCoordinate, Dimensions);

	if (CurrentQueryResult.bHasSpace)
	{
		HighlightSlots(ItemDropIndex, Dimensions);
		
		LastHighlightedIndex = ItemDropIndex;
		LastHighlightedDimensions = Dimensions;
		return;
	}
	
	if (CurrentQueryResult.ValidItem.IsValid() && GridSlotsArray.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(CurrentQueryResult.ValidItem.Get(),
																					FragmentTags::GridFragment);
		if (!GridFragment) return;
		ChangeHoverType(CurrentQueryResult.UpperLeftIndex, GridFragment->GetGridSize(),ESInv_GridSlotsState::GrayedOut);
	}
}

FSInv_SpaceQueryResult USInv_InventoryGrid::CheckHoverPosition(const FIntPoint& Position,
	const FIntPoint& Dimensions) 
{
	FSInv_SpaceQueryResult Result;
	
	// Are the dimensions within the grid bounds?
	if (!IsInGridBounds(USInv_WidgetUtils::GetIndexFromPosition(Position, Columns),Dimensions)) return Result;

	Result.bHasSpace = true;
	TSet<int32> OccupiedUpperLeftIndices;
	
	// If more than one of the indices is occupied with the same item, we need to see if they all have the same upper left index.
	USInv_InventoryStatics::ForEach2D(GridSlotsArray,
					USInv_WidgetUtils::GetIndexFromPosition(Position, Columns),
					Dimensions,
					Columns,
					[&](const USInv_GridSlot* GridSlot)
					{
						if (GridSlot->GetInventoryItem().IsValid())
						{
							OccupiedUpperLeftIndices.Add(GridSlot->GetUpperLeftSlotIndex());
							Result.bHasSpace = false;
						}
					});
	
	// if so, is there only one item in the way? (can we swap?)
	if (OccupiedUpperLeftIndices.Num() == 1) // single item at position- it's valid for swapping/combining
	{
		const int32 Index = *OccupiedUpperLeftIndices.CreateConstIterator();
		Result.ValidItem = GridSlotsArray[Index]->GetInventoryItem();
		Result.UpperLeftIndex = GridSlotsArray[Index]->GetUpperLeftSlotIndex();
	}
	return Result;
}

void USInv_InventoryGrid::ChangeHoverType(const int32 Index, const FIntPoint& Dimensions,
	ESInv_GridSlotsState GridSlotState)
{
	UnHighlightSlots(LastHighlightedIndex, LastHighlightedDimensions);

	USInv_InventoryStatics::ForEach2D(GridSlotsArray,Index,Dimensions,Columns,[State = GridSlotState]
		(USInv_GridSlot* GridSlot)
		{
			switch (State)
			{
			case ESInv_GridSlotsState::Occupied:
				GridSlot->SetOccupiedTexture();
				break;
				
			case ESInv_GridSlotsState::Unoccupied:
				GridSlot->SetUnoccupiedTexture();
				break;
				
			case ESInv_GridSlotsState::GrayedOut:
				GridSlot->SetGrayedOutTexture();
				break;

			case ESInv_GridSlotsState::Selected:
				GridSlot->SetSelectedTexture();
				break;
			}
		}
	);
	LastHighlightedIndex = Index;
	LastHighlightedDimensions = Dimensions;
}

FIntPoint USInv_InventoryGrid::CalculateStartingCoordinates(const FIntPoint& Coordinate, const FIntPoint& Dimensions, const ESInv_TileQuadrant Quadrant) const
{
	const int32 HasEvenWidth = Dimensions.X % 2 == 0 ? 1 : 0;
	const int32 HasEvenHeight= Dimensions.Y % 2 == 0 ? 1 : 0;

	FIntPoint StartingCoord;
	switch (Quadrant)
	{
		case ESInv_TileQuadrant::TopLeft:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;

		case ESInv_TileQuadrant::TopRight:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y);
		break;

		case ESInv_TileQuadrant::BottomLeft:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X);
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;

		case ESInv_TileQuadrant::BottomRight:
			StartingCoord.X = Coordinate.X - FMath::FloorToInt(0.5f * Dimensions.X) + HasEvenWidth;
			StartingCoord.Y = Coordinate.Y - FMath::FloorToInt(0.5f * Dimensions.Y) + HasEvenHeight;
		break;

		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid Quadrant"));
			return FIntPoint(-1,-1);
	}
	return StartingCoord;
}

FIntPoint USInv_InventoryGrid::CalculateHoveredCoordinates(const FVector2D& CanvasPos, const FVector2D& MousePos) const
{
	return FIntPoint
	{
		static_cast<int32>(FMath::FloorToInt((MousePos.X - CanvasPos.X) / SlotSize)),
		static_cast<int32>(FMath::FloorToInt((MousePos.Y - CanvasPos.Y) / SlotSize))
	};
}

ESInv_TileQuadrant USInv_InventoryGrid::CalculateTileQuadrant(const FVector2D& CanvasPos, const FVector2D& MousePos) const
{
	// Calculate relative position within the current tile
	const float TileLocalX = FMath::Fmod(MousePos.X - CanvasPos.X,SlotSize);
	const float TileLocalY = FMath::Fmod(MousePos.Y - CanvasPos.Y,SlotSize);

	// Determine which quadrant the mouse is in
	const bool bIsTop = TileLocalY < SlotSize / 2.f; // Top if Y is in the upper half.
	const bool bIsLeft = TileLocalX < SlotSize / 2.f; // Left if X is in the left half.

	ESInv_TileQuadrant HoveredTileQuadrant{ ESInv_TileQuadrant::None };
	if (bIsTop && bIsLeft) HoveredTileQuadrant = ESInv_TileQuadrant::TopLeft;
	else if (bIsTop && !bIsLeft) HoveredTileQuadrant = ESInv_TileQuadrant::TopRight;
	else if (!bIsTop && bIsLeft) HoveredTileQuadrant = ESInv_TileQuadrant::TopLeft;
	else if (!bIsTop && !bIsLeft) HoveredTileQuadrant = ESInv_TileQuadrant::BottomRight;

	return HoveredTileQuadrant;
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

			// Bind to Slot Events
			GridSlot->GridSlotClicked.AddDynamic(this, &ThisClass::OnGridSlotClicked);
			GridSlot->GridSlotHovered.AddDynamic(this, &ThisClass::OnGridSlotHovered);
			GridSlot->GridSlotUnhovered.AddDynamic(this, &ThisClass::OnGridSlotUnhovered);
		}
	}
}

/*-----------------------------------------------*/
/*		Grid Slot Events Callback Functions		*/
/*-----------------------------------------------*/

void USInv_InventoryGrid::OnGridSlotClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (!IsValid(HoverItem)) return;
	if (!GridSlotsArray.IsValidIndex(ItemDropIndex)) return;

	if (CurrentQueryResult.ValidItem.IsValid() && GridSlotsArray.IsValidIndex(CurrentQueryResult.UpperLeftIndex))
	{
		OnSlottedItemClicked(CurrentQueryResult.UpperLeftIndex,MouseEvent);
		return;
	}
	
	auto GridSlot = GridSlotsArray[ItemDropIndex];
	if (!GridSlot->GetInventoryItem().IsValid())
	{
		// Put Item Down at this index.
		PutDownOnIndex(ItemDropIndex);
	}
}

void USInv_InventoryGrid::PutDownOnIndex(const int32 Index)
{
	AddItemAtIndex(HoverItem->GetInventoryItem(),Index,HoverItem->IsStackable(),HoverItem->GetStackCount());
	UpdateGridSlots(HoverItem->GetInventoryItem(),Index,HoverItem->IsStackable(),HoverItem->GetStackCount());
	ClearHoveredItem();
}

void USInv_InventoryGrid::ClearHoveredItem()
{
	if (!IsValid(HoverItem)) return;

	HoverItem->SetInventoryItem(nullptr);
	HoverItem->SetIsStackable(false);
	HoverItem->SetPreviousGridIndex(INDEX_NONE);
	HoverItem->UpdateStackCount(0);
	HoverItem->SetImageBrush(FSlateNoResource());

	HoverItem->RemoveFromParent();
	HoverItem = nullptr;
	
	ShowCursor();
}

UUserWidget* USInv_InventoryGrid::GetVisibleCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;

	if (!IsValid(VisibleCursorWidget))
	{
		VisibleCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), VisibleCursorWidgetClass);
	}
	return VisibleCursorWidget;
}

UUserWidget* USInv_InventoryGrid::GetHiddenCursorWidget()
{
	if (!IsValid(GetOwningPlayer())) return nullptr;

	if (!IsValid(HiddenCursorWidget))
	{
		HiddenCursorWidget = CreateWidget<UUserWidget>(GetOwningPlayer(), HiddenCursorWidgetClass);
	}
	return HiddenCursorWidget;
}

bool USInv_InventoryGrid::IsSameStackable(const USInv_InventoryItem* ClickedInventoryItem) const
{
	const bool bIsSameItem = ClickedInventoryItem == HoverItem->GetInventoryItem();
	const bool bIsStackable = ClickedInventoryItem->IsStackable();
	return bIsSameItem &&
		   bIsStackable &&
		   	HoverItem->GetItemType().MatchesTagExact(ClickedInventoryItem->GetItemManifest().GetItemType());
}

void USInv_InventoryGrid::SwapWithHoverItem(USInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	if (!IsValid(HoverItem)) return;

	USInv_InventoryItem* TempInventoryItem = HoverItem->GetInventoryItem();
	const int32 TempStackCount = HoverItem->GetStackCount();
	const bool bTempIsStackable = HoverItem->IsStackable();

	// Keep the same previous grid index
	AssignHoverItem(ClickedInventoryItem, GridIndex, HoverItem->GetPreviousGridIndex());
	RemoveItemFromGrid(ClickedInventoryItem, GridIndex);
	AddItemAtIndex(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
	UpdateGridSlots(TempInventoryItem, ItemDropIndex, bTempIsStackable, TempStackCount);
}

bool USInv_InventoryGrid::ShouldSwapStackCounts(const int32 RoomInClickedSlot, const int32 HoveredStackCount,
	const int32 MaxStackSize)
{
	return RoomInClickedSlot == 0 && HoveredStackCount < MaxStackSize;
}

void USInv_InventoryGrid::SwapStackCounts(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	USInv_GridSlot* GridSlot = GridSlotsArray[Index];
	GridSlot->SetStackCount(HoveredStackCount);

	USInv_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(HoveredStackCount);

	HoverItem->UpdateStackCount(ClickedStackCount);
}

bool USInv_InventoryGrid::ShouldConsumeHoverItemStacks(const int32 HoveredStackCount,
	const int32 RoomInClickedSlot) const
{
	return RoomInClickedSlot >= HoveredStackCount;
}

void USInv_InventoryGrid::ConsumeHoverItemStacks(const int32 ClickedStackCount, const int32 HoveredStackCount,
	const int32 Index)
{
	const int32 AmountToTransfer = HoveredStackCount;
	const int32 NewClickedStackCount = ClickedStackCount + AmountToTransfer;

	GridSlotsArray[Index]->SetStackCount(NewClickedStackCount);
	SlottedItems.FindChecked(Index)->UpdateStackCount(NewClickedStackCount);
	ClearHoveredItem();
	ShowCursor();

	const FSInv_GridFragment* GridFragment = GridSlotsArray[Index]->GetInventoryItem()->GetItemManifest().GetFragmentOfType<FSInv_GridFragment>();
	const FIntPoint Dimensions = GridFragment ? GridFragment->GetGridSize() : FIntPoint(1,1);
	HighlightSlots(Index,Dimensions);
}

bool USInv_InventoryGrid::ShouldFillInStack(const int32 RoomInClickedSlot, const int32 HoveredStackCount) const
{
	return RoomInClickedSlot < HoveredStackCount;
}

void USInv_InventoryGrid::FillInStack(const int32 FillAmount, const int32 Remainder, const int32 Index)
{
	USInv_GridSlot* GridSlot = GridSlotsArray[Index];
	const int32 NewStackCount = GridSlot->GetStackCount() + FillAmount;

	GridSlot->SetStackCount(NewStackCount);

	USInv_SlottedItem* ClickedSlottedItem = SlottedItems.FindChecked(Index);
	ClickedSlottedItem->UpdateStackCount(NewStackCount);

	HoverItem->UpdateStackCount(Remainder);
}

void USInv_InventoryGrid::ShowCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetVisibleCursorWidget());
}

void USInv_InventoryGrid::HideCursor()
{
	if (!IsValid(GetOwningPlayer())) return;
	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default, GetHiddenCursorWidget());
}

void USInv_InventoryGrid::SetOwningCanvas(UCanvasPanel* OwningCanvas)
{
	OwningCanvasPanel = OwningCanvas;
}

void USInv_InventoryGrid::OnGridSlotHovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	USInv_GridSlot* GridSlot = GridSlotsArray[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetOccupiedTexture();
	}
}

void USInv_InventoryGrid::OnGridSlotUnhovered(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	if (IsValid(HoverItem)) return;

	USInv_GridSlot* GridSlot = GridSlotsArray[GridIndex];
	if (GridSlot->IsAvailable())
	{
		GridSlot->SetUnoccupiedTexture();
	}
}

void USInv_InventoryGrid::OnPopUpMenuSplit(int32 SplitAmount, int32 Index)
{
	USInv_InventoryItem* RightClickedItem = GridSlotsArray[Index]->GetInventoryItem().Get();
	
	if (!IsValid(RightClickedItem)) return;
	if (!RightClickedItem->IsStackable()) return;
	
	const int32 UpperLeftIndex = GridSlotsArray[Index]->GetUpperLeftSlotIndex();
	USInv_GridSlot* UpperLeftGridSlot = GridSlotsArray[UpperLeftIndex];
	const int32 StackCount = UpperLeftGridSlot->GetStackCount();
	const int32 NewStackCount = StackCount - SplitAmount;

	UpperLeftGridSlot->SetStackCount(NewStackCount); // Updates item new stack count
	SlottedItems.FindChecked(UpperLeftIndex)->UpdateStackCount(NewStackCount);

	AssignHoverItem(RightClickedItem, UpperLeftIndex, UpperLeftIndex);
	HoverItem->UpdateStackCount(SplitAmount);
}

void USInv_InventoryGrid::OnPopUpMenuDrop(int32 Index)
{
	USInv_InventoryItem* RightClickedItem = GridSlotsArray[Index]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;

	PickUp(RightClickedItem, Index);
	DropItem();
}

void USInv_InventoryGrid::OnPopUpMenuConsume(int32 Index)
{
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
	SlottedItem->OnSlottedItemClicked.AddDynamic(this,&ThisClass::OnSlottedItemClicked);
	
	return SlottedItem;
}

void USInv_InventoryGrid::OnSlottedItemClicked(int32 GridIndex, const FPointerEvent& MouseEvent)
{
	check(GridSlotsArray.IsValidIndex(GridIndex));
	USInv_InventoryItem* ClickedInventoryItem = GridSlotsArray[GridIndex]->GetInventoryItem().Get();

	if (!IsValid(HoverItem) && IsLeftClick(MouseEvent))
	{
		PickUp(ClickedInventoryItem,GridIndex);
		return;
	}

	if (IsRightClick(MouseEvent))
	{
		CreateItemPopUp(GridIndex);
		return;
	}
	
	// Do the hovered item and the clicked inventory item share a type, and are they stackable?
	if (IsSameStackable(ClickedInventoryItem))
	{
		const int32 ClickedStackCount = GridSlotsArray[GridIndex]->GetStackCount();
		const FSInv_StackableFragment* StackableFragment = ClickedInventoryItem->GetItemManifest().GetFragmentOfType<FSInv_StackableFragment>();
		const int32 MaxStackSize = StackableFragment->GetMaxStackSize();
		const int32 RoomInClickedSlot = MaxStackSize - ClickedStackCount;
		const int32 HoveredStackCount = HoverItem->GetStackCount();
		
		// Should we swap their stack counts? (Room in the clicked slot == 0 && HoveredStackCount < MaxStackSize)
		if (ShouldSwapStackCounts(RoomInClickedSlot,HoveredStackCount,MaxStackSize))
		{
			SwapStackCounts(ClickedStackCount,HoveredStackCount,GridIndex);
			return;
		}
		// Should we consume the hover item's stacks? (Room in the clicked slot >= HoveredStackCount)
		if (ShouldConsumeHoverItemStacks(HoveredStackCount,RoomInClickedSlot))
		{
			ConsumeHoverItemStacks(ClickedStackCount,HoveredStackCount,GridIndex);
			return;
		}
		// Should we fill in the stacks of the clicked item? (adn not consume the hovered item)
		if (ShouldFillInStack(RoomInClickedSlot,HoveredStackCount))
		{
			FillInStack(RoomInClickedSlot,HoveredStackCount - RoomInClickedSlot ,GridIndex);
			return;
		}
		
		// Clicked slot is already full - do nothing (maybe play a sound?)
		if (RoomInClickedSlot == 0)
		{
			return;
		}
	}
	// Swap with the hovered item.
	SwapWithHoverItem(ClickedInventoryItem,GridIndex);
}

void USInv_InventoryGrid::CreateItemPopUp(const int32 GridIndex)
{
	USInv_InventoryItem* RightClickedItem = GridSlotsArray[GridIndex]->GetInventoryItem().Get();
	if (!IsValid(RightClickedItem)) return;

	if (IsValid(GridSlotsArray[GridIndex]->GetItemPopUp())) return;
	
	PopUpItem = CreateWidget<USInv_ItemPopUp>(this,PopUpItemClass);
	GridSlotsArray[GridIndex]->SetItemPopUp(PopUpItem); // Sets weak object reference on Grid Slot.
	
	OwningCanvasPanel->AddChild(PopUpItem);

	UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(PopUpItem);
	const FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	CanvasSlot->SetPosition(MousePosition - ItemPopUpOffset);
	CanvasSlot->SetSize(PopUpItem->GetBoxSize());
	/*---------------------------------------------------------------------*/
	/*					Split Functionality Binding						   */
	/*		Check if Splittable, then calculate slider values			   */
	/*				or hide Split functionalities						   */
	/*---------------------------------------------------------------------*/
	
	const int32 SliderMax = GridSlotsArray[GridIndex]->GetStackCount() - 1;
	if (RightClickedItem->IsStackable() && SliderMax > 0)
	{
		PopUpItem->OnSplit.BindDynamic(this,&ThisClass::OnPopUpMenuSplit);
		PopUpItem->SetSliderParams(SliderMax,FMath::Max(1,GridSlotsArray[GridIndex]->GetStackCount() / 2)); // we make sure it's never 0 and in the middle
	}
	else
	{
		PopUpItem->CollapseSplitButton();
	}
	/*---------------------------------------------------------------------*/
	/*					 Drop Functionality Binding					       */
	/*---------------------------------------------------------------------*/
	
	PopUpItem->OnDrop.BindDynamic(this,&ThisClass::OnPopUpMenuDrop);
	
	/*---------------------------------------------------------------------*/
	/*					 Consume Functionality Binding					   */
	/*---------------------------------------------------------------------*/

	if (RightClickedItem->IsConsumable())
	{
		PopUpItem->OnConsume.BindDynamic(this,&ThisClass::OnPopUpMenuConsume);
	}
	else
	{
		PopUpItem->CollapseConsumeButton();
	}
}

void USInv_InventoryGrid::DropItem()
{
	if (!IsValid(HoverItem)) return;
	if (!IsValid(HoverItem->GetInventoryItem())) return;

	InventoryComponent->Server_DropItem(HoverItem->GetInventoryItem(), HoverItem->GetStackCount());

	ClearHoveredItem();
	ShowCursor();
}

bool USInv_InventoryGrid::IsRightClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool USInv_InventoryGrid::IsLeftClick(const FPointerEvent& MouseEvent) const
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}

void USInv_InventoryGrid::PickUp(USInv_InventoryItem* ClickedInventoryItem, const int32 GridIndex)
{
	// Assign the Hover Item
	AssignHoverItem(ClickedInventoryItem,GridIndex,GridIndex);
	
	// Remove the clicked item from the grid.
	RemoveItemFromGrid(ClickedInventoryItem,GridIndex);
}

void USInv_InventoryGrid::AssignHoverItem(USInv_InventoryItem* InventoryItem)
{
	if (!IsValid(HoverItem))
	{
		HoverItem = CreateWidget<USInv_HoverItem>(GetOwningPlayer(), HoverItemClass);
	}
	
	const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	const FSInv_ImageFragment* ImageFragment = GetFragment<FSInv_ImageFragment>(InventoryItem, FragmentTags::ImageFragment);
	if (!GridFragment || !ImageFragment) return;

	const FVector2D DrawSize = GetDrawSize(GridFragment);

	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ImageFragment->GetItemIcon());
	IconBrush.DrawAs = ESlateBrushDrawType::Image;
	IconBrush.ImageSize = DrawSize * UWidgetLayoutLibrary::GetViewportScale(this);

	HoverItem->SetImageBrush(IconBrush);
	HoverItem->SetGridDimensions(GridFragment->GetGridSize());
	HoverItem->SetInventoryItem(InventoryItem);
	HoverItem->SetIsStackable(InventoryItem->IsStackable());

	GetOwningPlayer()->SetMouseCursorWidget(EMouseCursor::Default,HoverItem);
}

void USInv_InventoryGrid::AssignHoverItem(USInv_InventoryItem* InventoryItem, const int32 GridIndex, const int32 PreviousGridIndex)
{
	AssignHoverItem(InventoryItem);

	HoverItem->SetPreviousGridIndex(PreviousGridIndex);
	HoverItem->UpdateStackCount(InventoryItem->IsStackable() ? GridSlotsArray[GridIndex]->GetStackCount() : 0);
}

void USInv_InventoryGrid::RemoveItemFromGrid(const USInv_InventoryItem* InventoryItem, const int32 GridIndex)
{
	const FSInv_GridFragment* GridFragment = GetFragment<FSInv_GridFragment>(InventoryItem, FragmentTags::GridFragment);
	if (!GridFragment) return;

	USInv_InventoryStatics::ForEach2D(GridSlotsArray,GridIndex,GridFragment->GetGridSize(),Columns,
		[&](USInv_GridSlot* GridSlot)
		{
			GridSlot->SetInventoryItem(nullptr);
			GridSlot->SetUpperLeftSlotIndex(INDEX_NONE);
			GridSlot->SetUnoccupiedTexture();
			GridSlot->SetAvailable(true);
			GridSlot->SetStackCount(0);
		});

	if (SlottedItems.Contains(GridIndex))
	{
		TObjectPtr<USInv_SlottedItem> FoundSlottedItem;
		SlottedItems.RemoveAndCopyValue(GridIndex, FoundSlottedItem);
		FoundSlottedItem->RemoveFromParent();
	}
}

/*--------------------------------------------------*/
/*		Adds Stacks on StackChange Delegate			*/
/*--------------------------------------------------*/
void USInv_InventoryGrid::AddStacks(const FSInv_SlotAvailabilityResult& Result)
{
	if (!MatchesCategory(Result.Item.Get())) return;

	for (const auto& Availability : Result.SlotAvailabilities)
	{
		if (Availability.bItemAtIndex) // We need to make sure there's an item there to add the stacks to.
		{
			const auto& GridSlot = GridSlotsArray[Availability.SlotIndex];
			const auto& SlottedItem = SlottedItems.FindChecked(Availability.SlotIndex);
			SlottedItem->UpdateStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
			GridSlot->SetStackCount(GridSlot->GetStackCount() + Availability.AmountToFill);
		}
		else
		{
			AddItemAtIndex(Result.Item.Get(),Availability.SlotIndex,Result.bStackable,Availability.AmountToFill);
			UpdateGridSlots(Result.Item.Get(),Availability.SlotIndex,Result.bStackable, Availability.AmountToFill);
		}
	}
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
