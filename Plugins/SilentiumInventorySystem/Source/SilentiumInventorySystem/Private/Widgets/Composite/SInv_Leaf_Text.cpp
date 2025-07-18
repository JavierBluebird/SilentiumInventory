// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/Composite/SInv_Leaf_Text.h"

#include "Components/TextBlock.h"

void USInv_Leaf_Text::NativePreConstruct()
{
	Super::NativePreConstruct();

	FSlateFontInfo FontInfo = Text_LeafText->GetFont();
	FontInfo.Size = FontSize;

	Text_LeafText->SetFont(FontInfo);
}

void USInv_Leaf_Text::SetText(const FText& Text) const
{
	Text_LeafText->SetText(Text);
}

