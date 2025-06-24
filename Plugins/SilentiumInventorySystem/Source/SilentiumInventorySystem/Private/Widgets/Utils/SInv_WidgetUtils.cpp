// Developed by Javier Gonzalez [Lumina Division]


#include "SInv_WidgetUtils.h"

int32 USInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}
