#include "Items/Fragments/SInv_FragmentTags.h"

namespace FragmentTags
{
	UE_DEFINE_GAMEPLAY_TAG(GridFragment, "FragmentTags.GridFragment")
	UE_DEFINE_GAMEPLAY_TAG(ImageFragment, "FragmentTags.ImageFragment")
	UE_DEFINE_GAMEPLAY_TAG(StackableFragment, "FragmentTags.StackableFragment")
	UE_DEFINE_GAMEPLAY_TAG(ConsumableFragment, "FragmentTags.ConsumableFragment")
	UE_DEFINE_GAMEPLAY_TAG(EquipmentFragment, "FragmentTags.EquipmentFragment")
	
	namespace ItemCharacteristics
	{	UE_DEFINE_GAMEPLAY_TAG(ItemNameFragment, "FragmentTags.ItemCharacteristics.ItemNameFragment")
		UE_DEFINE_GAMEPLAY_TAG(PrimaryStatFragment, "FragmentTags.ItemCharacteristics.PrimaryStatFragment")
		UE_DEFINE_GAMEPLAY_TAG(ItemTypeFragment, "FragmentTags.ItemCharacteristics.ItemTypeFragment")
		UE_DEFINE_GAMEPLAY_TAG(FlavorTextFragment, "FragmentTags.ItemCharacteristics.FlavorTextFragment")
		UE_DEFINE_GAMEPLAY_TAG(SellValueFragment, "FragmentTags.ItemCharacteristics.SellValueFragment")
		UE_DEFINE_GAMEPLAY_TAG(RequiredLevelFragment, "FragmentTags.ItemCharacteristics.RequiredLevelFragment")
	}
	namespace StatModifier
	{
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_1, "FragmentTags.StatModifier.1")
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_2, "FragmentTags.StatModifier.2")
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_3, "FragmentTags.StatModifier.3")
	}
}