#include "Items/SInv_ItemTags.h"

/*----------------------------------------*/
/*										  */
/*	Equippable Weapon Tags Definitions	  */
/*										  */
/*----------------------------------------*/

namespace GameItems
{
	namespace Equipment
	{
		namespace Weapons
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Axe, "GameItems.Equipment.Weapons.Axe", "Axe");
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Sword, "GameItems.Equipment.Weapons.Sword", "Sword");
		}
		namespace Cloaks
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(RedCloak, "GameItems.Equipment.Cloaks.RedCloak", "RedCloak");
		}
		namespace Masks
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(RedCloak, "GameItems.Equipment.Masks.SteelMask", "SteelMask");
		}
	}
	namespace Consumables
	{
		namespace Potions
		{
			namespace RedPotions
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(SmallPotion, "GameItems.Consumables.RedPotions.SmallPotion", "SmallPotion");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(LargePotion, "GameItems.Consumables.RedPotions.LargePotion", "LargePotion");
			}
			namespace BluePotions
			{
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(SmallPotion, "GameItems.Consumables.BluePotions.SmallPotion", "SmallPotion");
				UE_DEFINE_GAMEPLAY_TAG_COMMENT(LargePotion, "GameItems.Consumables.BluePotions.LargePotion", "LargePotion");
			}
		}
	}
	namespace Craftables
	{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(FireFernFruit,"GameItems.Craftables.FireFernFruit","FireFernFruit")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(LuminDaisy,"GameItems.Craftables.LuminDaisy","LuminDaisy")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(ScorchPetalBlossom,"GameItems.Craftables.ScorchPetalBlossom","ScorchPetalBlossom")
	}
}

