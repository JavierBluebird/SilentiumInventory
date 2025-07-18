#pragma once

#include "NativeGameplayTags.h"

namespace FragmentTags
{
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(GridFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ImageFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(StackableFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConsumableFragment)
	
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemNameFragment)
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(PrimaryStatFragment)

	namespace StatModifier
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatModifier_1)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatModifier_2)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatModifier_3)
	}
}