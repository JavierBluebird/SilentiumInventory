// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Composite/SInv_Composite.h"

#include "Blueprint/WidgetTree.h"

void USInv_Composite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
		{	// We are only interested in those inside a Composite hierarchy.
			if (USInv_CompositeBase* Composite = Cast<USInv_CompositeBase>(Widget); IsValid(Composite))
			{
				Children.Add(Composite);
				Composite->Collapse();
			}
		});
}

void USInv_Composite::ApplyFunction(FuncType Function)
{
	for (auto& Child : Children)
	{
		Child->ApplyFunction(Function);
	}
}

void USInv_Composite::Collapse()
{
	for (auto& Child : Children)
	{
		Child->Collapse();
	}
}
