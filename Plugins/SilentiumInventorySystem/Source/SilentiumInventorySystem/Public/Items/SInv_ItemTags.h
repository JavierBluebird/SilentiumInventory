#pragma once

#include "NativeGameplayTags.h"


/*----------------------------------------*/
/*										  */
/*	Equippable Weapon Tags Declarations	  */
/*										  */
/*----------------------------------------*/

namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Axe)
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(Sword)
		}
		namespace Cloaks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(RedCloak)
		}
		namespace Masks
		{
			UE_DECLARE_GAMEPLAY_TAG_EXTERN(SteelMask)
		}
	}
	namespace Consumables
	{
		namespace Potions
		{
			namespace RedPotions
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(SmallPotion)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(LargePotion)
			}
			namespace BluePotions
			{
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(SmallPotion)
				UE_DECLARE_GAMEPLAY_TAG_EXTERN(LargePotion)
			}
		}
	}
	namespace Craftables
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FireFernFruit)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(LuminDaisy)
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ScorchPetalBlossom)
	}
}


