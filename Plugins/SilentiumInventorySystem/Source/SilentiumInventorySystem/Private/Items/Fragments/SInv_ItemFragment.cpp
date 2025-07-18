#include "Items/Fragments/SInv_ItemFragment.h"

#include "Widgets/Composite/SInv_CompositeBase.h"
#include "Widgets/Composite/SInv_Leaf_Image.h"
#include "Widgets/Composite/SInv_Leaf_Text.h"

/*------------------------------------------*/
/*											*/
/*		   Fragments Assimilations			*/
/*											*/
/*------------------------------------------*/
void FSInv_InventoryItemFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;
	
	Composite->Expand(); // Shows the Composite
}

void FSInv_TextFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	FSInv_InventoryItemFragment::Assimilate(Composite);

	if (!MatchesWidgetTag(Composite)) return;
	USInv_Leaf_Text* LeafText = Cast<USInv_Leaf_Text>(Composite);
	if (!LeafText) return;

	LeafText->SetText(FragmentText);
}


void FSInv_ImageFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	FSInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	USInv_Leaf_Image* Image = Cast<USInv_Leaf_Image>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(ItemIcon);
	Image->SetBoxSize(IconDimensions);
	Image->SetImageSize(IconDimensions);
}

bool FSInv_InventoryItemFragment::MatchesWidgetTag(const USInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FSInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	// Get a Stats Component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an Interface Function for Healing(), etc.

	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f, FColor::Emerald, FString::Printf(TEXT("Health Potion Consumed! Healing by: %f"), HealAmount));
}

void FSInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	// Get a Stats Component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an Interface Function for HealingMana(), etc.
	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f, FColor::Emerald, FString::Printf(TEXT("Mana Potion Consumed! Mana Recovered by: %f"), ManaAmount));
}

