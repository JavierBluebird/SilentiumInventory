// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Composite/SInv_Leaf.h"

void USInv_Leaf::ApplyFunction(FuncType Function)
{
	Function(this);
}
