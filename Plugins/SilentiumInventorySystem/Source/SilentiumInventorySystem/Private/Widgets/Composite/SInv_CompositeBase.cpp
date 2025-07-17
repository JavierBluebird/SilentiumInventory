// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Composite/SInv_CompositeBase.h"

void USInv_CompositeBase::Collapse()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void USInv_CompositeBase::Expand()
{
	SetVisibility(ESlateVisibility::Visible);
}
