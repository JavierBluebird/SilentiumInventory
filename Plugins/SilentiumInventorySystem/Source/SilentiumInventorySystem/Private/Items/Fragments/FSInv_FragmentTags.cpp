#include "Items/Fragments/SInv_FragmentTags.h"

namespace FragmentTags
{
	UE_DEFINE_GAMEPLAY_TAG(GridFragment, "FragmentTags.GridFragment")
	UE_DEFINE_GAMEPLAY_TAG(ImageFragment, "FragmentTags.ImageFragment")
	UE_DEFINE_GAMEPLAY_TAG(StackableFragment, "FragmentTags.StackableFragment")
	UE_DEFINE_GAMEPLAY_TAG(ConsumableFragment, "FragmentTags.ConsumableFragment")
	UE_DEFINE_GAMEPLAY_TAG(ItemNameFragment, "FragmentTags.ItemNameFragment")
	UE_DEFINE_GAMEPLAY_TAG(PrimaryStatFragment, "FragmentTags.PrimaryStatFragment")

	namespace StatModifier
	{
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_1, "FragmentTags.StatModifier.1")
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_2, "FragmentTags.StatModifier.2")
		UE_DEFINE_GAMEPLAY_TAG(StatModifier_3, "FragmentTags.StatModifier.3")
	}
}