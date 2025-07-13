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

int32 USInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint USInv_WidgetUtils::GetPositionFromIndex(const int32 SlotIndex, const int32 Columns)
{
	return FIntPoint(SlotIndex % Columns, SlotIndex / Columns);
}
