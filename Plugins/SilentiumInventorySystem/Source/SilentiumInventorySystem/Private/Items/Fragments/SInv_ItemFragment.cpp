#include "Items/Fragments/SInv_ItemFragment.h"

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
