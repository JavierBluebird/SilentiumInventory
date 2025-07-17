// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/ItemDescription/SInv_ItemDescription.h"

#include "Components/SizeBox.h"

FVector2D USInv_ItemDescription::GetBoxSize() const
{
	return SizeBox->GetDesiredSize();
}
