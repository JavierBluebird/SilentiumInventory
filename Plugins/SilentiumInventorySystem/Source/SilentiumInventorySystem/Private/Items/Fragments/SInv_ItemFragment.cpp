#include "Items/Fragments/SInv_ItemFragment.h"

#include "Widgets/Composite/SInv_CompositeBase.h"
#include "Widgets/Composite/SInv_Leaf_Image.h"
#include "Widgets/Composite/SInv_Leaf_LabeledValue.h"
#include "Widgets/Composite/SInv_Leaf_Text.h"

/*----------------------------------------------*/
/*									     		*/
/*		   Base Fragments Assimilations			*/
/*										    	*/
/*----------------------------------------------*/
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

void FSInv_EquipmentFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	FSInv_InventoryItemFragment::Assimilate(Composite);
	for (const auto& Modifier : EquipModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

/*------------------------------------------------------*/
/*		Base Equipment Fragment Composite Functions		*/
/*------------------------------------------------------*/
void FSInv_EquipmentFragment::OnEquip(APlayerController* PC)
{
	if (bEquipped) return;
	bEquipped = true;
	
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnEquip(PC);
	}
}
void FSInv_EquipmentFragment::OnUnequip(APlayerController* PC)
{
	if (!bEquipped) return;
	bEquipped = false;
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnUnequip(PC);
	}
}

void FSInv_EquipmentFragment::Manifest()
{
	FSInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : EquipModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

/*-----------------------------------------------------------*/
/*		Equip Modifier Fragments Composite Functions		*/
/*--------------------------------------------------....----*/
void FSInv_StrengthModifier::OnEquip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f,
										  FColor::Emerald,
										  FString::Printf(
											TEXT("Strength increased by: %f"),
											GetValue()));
}

void FSInv_StrengthModifier::OnUnequip(APlayerController* PC)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f,
										  FColor::Emerald,
										  FString::Printf(
											TEXT("Item Unequipped. Strength decreased by: %f"),
											GetValue()));
}
/*-----------------------------------------------------------*/
/*		Equip Modifier Fragments Composite End				*/
/*--------------------------------------------------....----*/
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

void FSInv_LabeledNumberFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	FSInv_InventoryItemFragment::Assimilate(Composite);
	if (!MatchesWidgetTag(Composite)) return;

	USInv_Leaf_LabeledValue* LabeledValue = Cast<USInv_Leaf_LabeledValue>(Composite);
	if (!IsValid(LabeledValue)) return;

	LabeledValue->SetText_Label(Text_Label,bCollapseLabel);

	FNumberFormattingOptions NumberOptions;
	
	NumberOptions.MinimumFractionalDigits = MinFractionalDigits;
	NumberOptions.MaximumFractionalDigits = MaxFractionalDigits;
	
	LabeledValue->SetText_Value(FText::AsNumber(Value,&NumberOptions),bCollapseValue);
}

void FSInv_ConsumableFragment::Assimilate(USInv_CompositeBase* Composite) const
{
	FSInv_InventoryItemFragment::Assimilate(Composite);

	for (const auto& Modifier : ConsumeModifiers)
	{
		const auto& ModRef = Modifier.Get();
		ModRef.Assimilate(Composite);
	}
}

bool FSInv_InventoryItemFragment::MatchesWidgetTag(const USInv_CompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

/*----------------------------------------------*/
/*												*/
/*		   Fragments Manifest Functions			*/
/*												*/
/*----------------------------------------------*/

void FSInv_LabeledNumberFragment::Manifest()
{
	FSInv_InventoryItemFragment::Manifest();
	
		if (bRandomizeOnManifest)
		{
			Value = FMath::RandRange(Min,Max);
		}
		bRandomizeOnManifest = false;
}

/*----------------------------------------------*/
/*												*/
/*		   Fragments Specific Functions			*/
/*												*/
/*----------------------------------------------*/
void FSInv_ConsumableFragment::OnConsume(APlayerController* PC)
{
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.OnConsume(PC);
	}
}

void FSInv_ConsumableFragment::Manifest()
{
	FSInv_InventoryItemFragment::Manifest();
	for (auto& Modifier : ConsumeModifiers)
	{
		auto& ModRef = Modifier.GetMutable();
		ModRef.Manifest();
	}
}

void FSInv_HealthPotionFragment::OnConsume(APlayerController* PC)
{
	// Get a Stats Component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an Interface Function for Healing(), etc.

	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f, FColor::Emerald, FString::Printf(
		TEXT("Health Potion Consumed! Healing by: %f"), GetValue()));
}

void FSInv_ManaPotionFragment::OnConsume(APlayerController* PC)
{
	// Get a Stats Component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an Interface Function for HealingMana(), etc.
	GEngine->AddOnScreenDebugMessage(INDEX_NONE,5.f, FColor::Emerald, FString::Printf(TEXT("Mana Potion Consumed! Mana Recovered by: %f"), GetValue()));
}

