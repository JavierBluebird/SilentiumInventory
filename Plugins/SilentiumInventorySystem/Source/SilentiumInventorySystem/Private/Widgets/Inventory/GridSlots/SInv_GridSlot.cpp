// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/GridSlots/SInv_GridSlot.h"

#include "Components/Image.h"

void USInv_GridSlot::SetOccupiedTexture()
{
	GridSlotState = ESInv_GridSlotsState::Occupied;
	Image_GridSlot->SetBrush(Brush_Occupied);
}

void USInv_GridSlot::SetUnoccupiedTexture()
{
	GridSlotState = ESInv_GridSlotsState::Unoccupied;
	Image_GridSlot->SetBrush(Brush_Unoccupied);
}

void USInv_GridSlot::SetSelectedTexture()
{
	GridSlotState = ESInv_GridSlotsState::Selected;
	Image_GridSlot->SetBrush(Brush_Selected);
}

void USInv_GridSlot::SetGrayedOutTexture()
{
	GridSlotState = ESInv_GridSlotsState::GrayedOut;
	Image_GridSlot->SetBrush(Brush_GrayedOut);
}
