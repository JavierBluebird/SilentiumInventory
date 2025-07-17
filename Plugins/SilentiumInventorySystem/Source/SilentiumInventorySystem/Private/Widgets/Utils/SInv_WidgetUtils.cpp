// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Inventory/Utils/SInv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"

FVector2D USInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;

	USlateBlueprintLibrary::LocalToViewport(Widget,Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry),PixelPosition, ViewportPosition);
	return ViewportPosition;
}

FVector2D USInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool USInv_WidgetUtils::IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X >= BoundaryPos.X
			&& MousePos.X <= (BoundaryPos.X + WidgetSize.X)
			&& MousePos.Y >= BoundaryPos.Y
			&& MousePos.Y <= (BoundaryPos.Y + WidgetSize.Y);
}

FVector2D USInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize,
	const FVector2D& MousePos)
{
	FVector2D ClampedPosition = MousePos;

	/*-----------------------------------------------*/
	/*		     Horizontal Boundaries				 */
	/*    Adjust horizontal position to ensure		 */
	/*  that the widget stays within the boundary    */
	/*-----------------------------------------------*/

	if (MousePos.X + WidgetSize.X > Boundary.X) // Widget exceeds the right edge.
	{
		ClampedPosition.X = Boundary.X - WidgetSize.X;
	}
	
	if (MousePos.X < 0.f) // Widget exceeds the left edge.
	{
		ClampedPosition.X = 0.f;
	}
	
	/*-----------------------------------------------*/
	/*		     Vertical Boundaries				 */
	/*     Adjust vertical position to ensure		 */
	/*  that the widget stays within the boundary    */
	/*-----------------------------------------------*/
	if (MousePos.Y + WidgetSize.Y > Boundary.Y) // Widget exceeds the bottom edge.
	{
		ClampedPosition.Y = Boundary.Y - WidgetSize.Y;
	}

	if (MousePos.Y < 0.f) // Widget exceeds the top edge.
	{
		ClampedPosition.Y = 0.f;
	}
	
	return ClampedPosition;
}

int32 USInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint USInv_WidgetUtils::GetPositionFromIndex(const int32 SlotIndex, const int32 Columns)
{
	return FIntPoint(SlotIndex % Columns, SlotIndex / Columns);
}
