/**
    Nefertumm and Stevej
    https://github.com/Nefertumm/
    https://github.com/pangolp
 */

#include "loader.h"
#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "NpcPromotion.h"

static bool npcPromotionEnabled, npcPromotionAnnounceEnable;
static int npcPromotionCount, npcPromotionIpCount, npcPromotionMaxLevel,
    npcPromotionMoney, npcPromotionBag, npcPromotionBagAmount,
    NpcPromotionMountReward;
static bool NpcPromotionWarriorTankEnabled, NpcPromotionWarriorDpsEnabled,
    npcPromotionEnableIpLimit, NpcPromotionBagEnable, NpcPromotionEquippedbags,
    NpcPromotionMountEnable;

class NpcPromotionAnnouncer : public PlayerScript
{
    public:
        NpcPromotionAnnouncer() : PlayerScript("NpcPromotionAnnouncer") {}

        void OnLogin(Player* player)
        {
            if (npcPromotionAnnounceEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage(40000);
            }
        }
};

void promotionPlayerTemplate(Player* player)
{
    player->GiveLevel(80);
    player->InitTalentForLevel();
    player->SetUInt32Value(PLAYER_XP, 0);

    player->ModifyMoney(npcPromotionMoney);

    //Bags
    if (NpcPromotionBagEnable) {
        if (NpcPromotionEquippedbags) {
            for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
                if (Item* bag = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                    player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);

            for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
                player->EquipNewItem(slot, npcPromotionBag, true);
        }
        else
        {
            player->AddItem(npcPromotionBag, npcPromotionBagAmount);
        }
    }

    // Riding
    if (NpcPromotionMountEnable) {
        player->learnSpell(SKILL_RIDING_75);
        player->learnSpell(SKILL_RIDING_100);
        player->learnSpell(SKILL_RIDING_FLYING);
        player->learnSpell(SKILL_RIDING_ARTISING);
        player->learnSpell(NpcPromotionMountReward);
    }
    player->UpdateSkillsToMaxSkillsForLevel();
}

void equipmentWarriorTank(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_WARRIOR_TANK_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_WARRIOR_TANK_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_WARRIOR_TANK_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_WARRIOR_TANK_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_WARRIOR_TANK_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_WARRIOR_TANK_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_WARRIOR_TANK_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_WARRIOR_TANK_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_WARRIOR_TANK_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_WARRIOR_TANK_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_WARRIOR_TANK_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_WARRIOR_TANK_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_WARRIOR_TANK_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_WARRIOR_TANK_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_WARRIOR_TANK_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_WARRIOR_TANK_RANGED, true);
}

void equipmentWarriorDps(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_WARRIOR_DPS_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_WARRIOR_DPS_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_WARRIOR_DPS_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_WARRIOR_DPS_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_WARRIOR_DPS_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_WARRIOR_DPS_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_WARRIOR_DPS_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_WARRIOR_DPS_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_WARRIOR_DPS_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_WARRIOR_DPS_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_WARRIOR_DPS_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_WARRIOR_DPS_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_WARRIOR_DPS_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_WARRIOR_DPS_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_WARRIOR_DPS_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_WARRIOR_DPS_RANGED, true);
}

void equipmentPaladinTank(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_PALADIN_TANK_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_PALADIN_TANK_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_PALADIN_TANK_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_PALADIN_TANK_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_PALADIN_TANK_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_PALADIN_TANK_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_PALADIN_TANK_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_PALADIN_TANK_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_PALADIN_TANK_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_PALADIN_TANK_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_PALADIN_TANK_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_PALADIN_TANK_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_PALADIN_TANK_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_PALADIN_TANK_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_PALADIN_TANK_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_PALADIN_TANK_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_PALADIN_TANK_RANGED, true);
}

void equipmentPaladinHeal(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_PALADIN_HEAL_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_PALADIN_HEAL_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_PALADIN_HEAL_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_PALADIN_HEAL_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_PALADIN_HEAL_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_PALADIN_HEAL_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_PALADIN_HEAL_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_PALADIN_HEAL_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_PALADIN_HEAL_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_PALADIN_HEAL_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_PALADIN_HEAL_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_PALADIN_HEAL_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_PALADIN_HEAL_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_PALADIN_HEAL_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_PALADIN_HEAL_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_PALADIN_HEAL_RANGED, true);
}

void equipmentPaladinDps(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_PALADIN_DPS_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_PALADIN_DPS_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_PALADIN_DPS_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_PALADIN_DPS_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_PALADIN_DPS_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_PALADIN_DPS_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_PALADIN_DPS_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_PALADIN_DPS_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_PALADIN_DPS_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_PALADIN_DPS_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_PALADIN_DPS_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_PALADIN_DPS_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_PALADIN_DPS_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_PALADIN_DPS_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_PALADIN_DPS_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_PALADIN_DPS_RANGED, true);
}

void equipmentHunter(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_HUNTER_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_HUNTER_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_HUNTER_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_HUNTER_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_HUNTER_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_HUNTER_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_HUNTER_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_HUNTER_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_HUNTER_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_HUNTER_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_HUNTER_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_HUNTER_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_HUNTER_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_HUNTER_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_HUNTER_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_HUNTER_RANGED, true);
}

void equipmentRogue(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_ROGUE_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_ROGUE_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_ROGUE_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_ROGUE_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_ROGUE_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_ROGUE_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_ROGUE_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_ROGUE_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_ROGUE_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_ROGUE_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_ROGUE_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_ROGUE_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_ROGUE_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_ROGUE_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_ROGUE_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_ROGUE_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_ROGUE_RANGED, true);
}

void equipmentPriestHeal(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_PRIEST_HEAL_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_PRIEST_HEAL_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_PRIEST_HEAL_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_PRIEST_HEAL_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_PRIEST_HEAL_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_PRIEST_HEAL_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_PRIEST_HEAL_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_PRIEST_HEAL_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_PRIEST_HEAL_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_PRIEST_HEAL_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_PRIEST_HEAL_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_PRIEST_HEAL_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_PRIEST_HEAL_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_PRIEST_HEAL_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_PRIEST_HEAL_RANGED, true);
}

void equipmentPriestShadow(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_PRIEST_SHADOW_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_PRIEST_SHADOW_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_PRIEST_SHADOW_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_PRIEST_SHADOW_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_PRIEST_SHADOW_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_PRIEST_SHADOW_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_PRIEST_SHADOW_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_PRIEST_SHADOW_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_PRIEST_SHADOW_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_PRIEST_SHADOW_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_PRIEST_SHADOW_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_PRIEST_SHADOW_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_PRIEST_SHADOW_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_PRIEST_SHADOW_RANGED, true);
}

void equipmentDkTank(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DK_TANK_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DK_TANK_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DK_TANK_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DK_TANK_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DK_TANK_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DK_TANK_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DK_TANK_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DK_TANK_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DK_TANK_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DK_TANK_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DK_TANK_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DK_TANK_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DK_TANK_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DK_TANK_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DK_TANK_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DK_TANK_RANGED, true);
}

void equipmentDkDps(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DK_DPS_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DK_DPS_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DK_DPS_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DK_DPS_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DK_DPS_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DK_DPS_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DK_DPS_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DK_DPS_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DK_DPS_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DK_DPS_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DK_DPS_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DK_DPS_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DK_DPS_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DK_DPS_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DK_DPS_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DK_DPS_RANGED, true);
}

void equipmentShamanCaster(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_SHAMAN_CASTER_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_SHAMAN_CASTER_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_SHAMAN_CASTER_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_SHAMAN_CASTER_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_SHAMAN_CASTER_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_SHAMAN_CASTER_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_SHAMAN_CASTER_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_SHAMAN_CASTER_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_SHAMAN_CASTER_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_SHAMAN_CASTER_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_SHAMAN_CASTER_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_SHAMAN_CASTER_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_SHAMAN_CASTER_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_SHAMAN_CASTER_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_SHAMAN_CASTER_RANGED, true);
}

void equipmentShamanMelee(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_SHAMAN_MELEE_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_SHAMAN_MELEE_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_SHAMAN_MELEE_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_SHAMAN_MELEE_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_SHAMAN_MELEE_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_SHAMAN_MELEE_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_SHAMAN_MELEE_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_SHAMAN_MELEE_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_SHAMAN_MELEE_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_SHAMAN_MELEE_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_SHAMAN_MELEE_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_SHAMAN_MELEE_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_SHAMAN_MELEE_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_SHAMAN_MELEE_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_SHAMAN_MELEE_RANGED, true);
}

void equipmentShamanHeal(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_SHAMAN_HEAL_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_SHAMAN_HEAL_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_SHAMAN_HEAL_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_SHAMAN_HEAL_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_SHAMAN_HEAL_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_SHAMAN_HEAL_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_SHAMAN_HEAL_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_SHAMAN_HEAL_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_SHAMAN_HEAL_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_SHAMAN_HEAL_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_SHAMAN_HEAL_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_SHAMAN_HEAL_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_SHAMAN_HEAL_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_OFFHAND, EQUIPMENT_SLOT_SHAMAN_HEAL_OFFHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_SHAMAN_HEAL_RANGED, true);
}

void equipmentMage(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_MAGE_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_MAGE_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_MAGE_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_MAGE_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_MAGE_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_MAGE_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_MAGE_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_MAGE_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_MAGE_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_MAGE_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_MAGE_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_MAGE_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_MAGE_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_MAGE_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_MAGE_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_MAGE_RANGED, true);
}

void equipmentWarlock(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_WARLOCK_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_WARLOCK_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_WARLOCK_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_WARLOCK_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_WARLOCK_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_WARLOCK_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_WARLOCK_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_WARLOCK_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_WARLOCK_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_WARLOCK_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_WARLOCK_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_WARLOCK_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_WARLOCK_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_WARLOCK_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_WARLOCK_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_WARLOCK_RANGED, true);
}

void equipmentDruidTank(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DRUID_TANK_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DRUID_TANK_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DRUID_TANK_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DRUID_TANK_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DRUID_TANK_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DRUID_TANK_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DRUID_TANK_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DRUID_TANK_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DRUID_TANK_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DRUID_TANK_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DRUID_TANK_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DRUID_TANK_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DRUID_TANK_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DRUID_TANK_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DRUID_TANK_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DRUID_TANK_RANGED, true);
}

void equipmentDruidHeal(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DRUID_HEAL_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DRUID_HEAL_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DRUID_HEAL_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DRUID_HEAL_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DRUID_HEAL_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DRUID_HEAL_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DRUID_HEAL_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DRUID_HEAL_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DRUID_HEAL_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DRUID_HEAL_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DRUID_HEAL_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DRUID_HEAL_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DRUID_HEAL_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DRUID_HEAL_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DRUID_HEAL_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DRUID_HEAL_RANGED, true);
}

void equipmentDruidMelee(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DRUID_MELEE_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DRUID_MELEE_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DRUID_MELEE_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DRUID_MELEE_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DRUID_MELEE_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DRUID_MELEE_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DRUID_MELEE_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DRUID_MELEE_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DRUID_MELEE_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DRUID_MELEE_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DRUID_MELEE_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DRUID_MELEE_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DRUID_MELEE_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DRUID_MELEE_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DRUID_MELEE_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DRUID_MELEE_RANGED, true);
}

void equipmentDruidCaster(Player* player)
{
    player->EquipNewItem(EQUIPMENT_SLOT_HEAD, EQUIPMENT_SLOT_DRUID_CASTER_HEAD, true);
    player->EquipNewItem(EQUIPMENT_SLOT_NECK, EQUIPMENT_SLOT_DRUID_CASTER_NECK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_SHOULDERS, EQUIPMENT_SLOT_DRUID_CASTER_SHOULDERS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_CHEST, EQUIPMENT_SLOT_DRUID_CASTER_CHEST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WAIST, EQUIPMENT_SLOT_DRUID_CASTER_WAIST, true);
    player->EquipNewItem(EQUIPMENT_SLOT_LEGS, EQUIPMENT_SLOT_DRUID_CASTER_LEGS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FEET, EQUIPMENT_SLOT_DRUID_CASTER_FEET, true);
    player->EquipNewItem(EQUIPMENT_SLOT_WRISTS, EQUIPMENT_SLOT_DRUID_CASTER_WRISTS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_HANDS, EQUIPMENT_SLOT_DRUID_CASTER_HANDS, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER1, EQUIPMENT_SLOT_DRUID_CASTER_FINGER1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_FINGER2, EQUIPMENT_SLOT_DRUID_CASTER_FINGER2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET1, EQUIPMENT_SLOT_DRUID_CASTER_TRINKET1, true);
    player->EquipNewItem(EQUIPMENT_SLOT_TRINKET2, EQUIPMENT_SLOT_DRUID_CASTER_TRINKET2, true);
    player->EquipNewItem(EQUIPMENT_SLOT_BACK, EQUIPMENT_SLOT_DRUID_CASTER_BACK, true);
    player->EquipNewItem(EQUIPMENT_SLOT_MAINHAND, EQUIPMENT_SLOT_DRUID_CASTER_MAINHAND, true);
    player->EquipNewItem(EQUIPMENT_SLOT_RANGED, EQUIPMENT_SLOT_DRUID_CASTER_RANGED, true);
}

class npc_promocion : public CreatureScript
{
    public:
        npc_promocion() : CreatureScript("npc_promocion") {}

        uint8 getAccountPromotionCount(uint32 accountId)
        {
            QueryResult result = LoginDatabase.PQuery("SELECT COUNT(`accountId`) FROM `promotion` WHERE `accountId` = %u", accountId);

            if (result)
            {
                Field* fields = result->Fetch();
                return fields[0].GetUInt8();
            }

            return 0;
        }

        uint8 getIpPromotionCount(uint32 accountId)
        {
            QueryResult result = LoginDatabase.PQuery("SELECT COUNT(`ip`) FROM `promotion` WHERE `accountId` = %u", accountId);

            if (result)
            {
                Field* fields = result->Fetch();
                return fields[0].GetUInt8();
            }

            return 0;

        }

        bool addPromotionLog(Player* player)
        {
            uint32 accountId = player->GetSession()->GetAccountId();
            std::string accountName;
            AccountMgr::GetName(accountId, accountName);
            std::string characterName = player->GetSession()->GetPlayerName();
            std::string ipAccount = player->GetSession()->GetRemoteAddress();

            QueryResult result = LoginDatabase.PQuery("INSERT INTO `promotion` (`accountId`, `accountName`, `characterName`, `ip`) VALUES (%u, '%s', '%s', '%s')",
                accountId, accountName.c_str(), characterName.c_str(), ipAccount.c_str());
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            uint8 countAccount = getAccountPromotionCount(player->GetSession()->GetAccountId());
            int8 countIp;
            if (npcPromotionEnableIpLimit) {
                countIp = getIpPromotionCount(player->GetSession()->GetAccountId());
            } else {
                countIp = -1;
            }

            if ((player->getLevel() < npcPromotionMaxLevel) && (((countAccount < npcPromotionCount)) || (countIp < npcPromotionIpCount)))
            {
                switch (player->getClass())
                {
                    case CLASS_WARRIOR:
                        if (NpcPromotionWarriorTankEnabled)
                            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_WARRIOR_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                        if (NpcPromotionWarriorDpsEnabled)
                            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_WARRIOR_DPS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                        break;
                    case CLASS_PALADIN:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_PALADIN_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_PALADIN_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_PALADIN_DPS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                        break;
                    case CLASS_HUNTER:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_HUNTER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                        break;
                    case CLASS_ROGUE:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_ROGUE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
                        break;
                    case CLASS_PRIEST:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_PRIEST_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_PRIEST_DPS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);
                        break;
                    case CLASS_DEATH_KNIGHT:
                        if ((player->GetQuestStatus(QUEST_WHERE_KINGS_WALK) == QUEST_STATUS_REWARDED) || (player->GetQuestStatus(QUEST_WARCHIEFS_BLESSING) == QUEST_STATUS_REWARDED))
                        {
                            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DEATH_KNIGHT_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);
                            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DEATH_KNIGHT_DPS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);
                        }
                        break;
                    case CLASS_SHAMAN:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_SHAMAN_CASTER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_SHAMAN_MELEE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_SHAMAN_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                        break;
                    case CLASS_MAGE:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_MAGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                        break;
                    case CLASS_WARLOCK:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_WARLOCK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                        break;
                    case CLASS_DRUID:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_CASTER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_MELEE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
                        break;
                    default:
                        break;
                }
            }

            if (player->getLevel() >= npcPromotionMaxLevel)
                AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_TP_DALARAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);

            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_CLOSE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);
            SendGossipMenuFor(player, NPC_TEXT_ID, creature);
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* /* creature */, uint32 /*sender*/, uint32 action)
        {
            ClearGossipMenuFor(player);

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentWarriorTank(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentWarriorDps(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentPaladinTank(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentPaladinHeal(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentPaladinDps(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    addPromotionLog(player);
                    player->learnSpell(STAVES);
                    promotionPlayerTemplate(player);
                    player->learnSpell(MAIL);
                    equipmentHunter(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 7:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentRogue(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 8:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentPriestHeal(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 9:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentPriestShadow(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 10:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentDkTank(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 11:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(PLATE_MAIL);
                    equipmentDkDps(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 12:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(MAIL);
                    equipmentShamanCaster(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 13:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(MAIL);
                    equipmentShamanMelee(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 14:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    player->learnSpell(MAIL);
                    equipmentShamanHeal(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 15:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentMage(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 16:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentWarlock(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 17:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentDruidTank(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 18:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentDruidHeal(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 19:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentDruidMelee(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 20:
                    addPromotionLog(player);
                    promotionPlayerTemplate(player);
                    equipmentDruidCaster(player);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 21:
                    break;
                case GOSSIP_ACTION_INFO_DEF + 22:
                    /* Teleport a Dalaran si es 80. */
                    player->TeleportTo(571, 5804.14f, 624.77f, 647.76f, 1.64f);
                    break;
                default:
                    break;
            }
            CloseGossipMenuFor(player);
            return true;
        }
};

class NpcPromotionCommand : public CommandScript
{
    public:

        NpcPromotionCommand() : CommandScript("NpcPromotionCommand") { }

        std::vector<ChatCommand> GetCommands() const override
        {
            static std::vector<ChatCommand> commandTable
            {
                { "view", SEC_MODERATOR, false, &HandleViewNpcPromotionCommand, "" }
            };

            static std::vector<ChatCommand> promotionCommandTable =
            {
                { "promotion", SEC_MODERATOR, false, nullptr, "", commandTable}
            };

            return promotionCommandTable;
        }

        static void getTargetAccountIdByName(std::string& name, uint32& accountId)
        {
            QueryResult result = CharacterDatabase.PQuery("SELECT `account` FROM `characters` WHERE `name` = '%s';", name);
            accountId = (*result)[0].GetInt32();
        }

        static bool HandleViewNpcPromotionCommand(ChatHandler* handler, const char* args)
        {
            if (!*args)
                return false;

            Player* target = nullptr;
            std::string playerName;

            if (!handler->extractPlayerTarget((char*)args, &target, nullptr, &playerName))
                return false;

            uint32 playerAccountId;

            if (target)
                playerAccountId = target->GetSession()->GetAccountId();
            else
                getTargetAccountIdByName(playerName, playerAccountId);

            QueryResult result = LoginDatabase.PQuery("SELECT * FROM `account` WHERE `id` = %u;", playerAccountId);
            QueryResult resultPromotion = LoginDatabase.PQuery("SELECT * FROM `promotion` WHERE `accountId` = %u;", playerAccountId);

            if (result)
            {
                Field* fields = result->Fetch();
                std::string accountName = fields[1].GetString();
                std::string email = fields[7].GetString();
                std::string ip = fields[10].GetString();
                uint8 online = fields[16].GetUInt8();
                uint32 recruiter = fields[23].GetUInt32();

                handler->PSendSysMessage("                                                      ");
                handler->PSendSysMessage(" ------------------------------------------------     ");
                handler->PSendSysMessage(" Username : %s", accountName.c_str());
                handler->PSendSysMessage(" Email : %s", email.c_str());
                handler->PSendSysMessage(" Last IP : %s", ip.c_str());
                handler->PSendSysMessage(" Online : %s", online ? " [ONLINE]" : "[OFFLINE]");
                handler->PSendSysMessage(" Recruiter : %u", recruiter);
                handler->PSendSysMessage(" ------------------------------------------------     ");
                handler->PSendSysMessage("      --- LISTA DE PERSONAJES ---                     ");

                if (resultPromotion)
                {
                    do
                    {
                        Field* promotion = resultPromotion->Fetch();
                        std::string characterName = promotion[3].GetString();
                        std::string ip = promotion[4].GetString();
                        std::string date = promotion[5].GetString();
                        handler->PSendSysMessage("Character: %s, ip: %s, date: %s", characterName.c_str(), ip.c_str(), date.c_str());
                    } while (resultPromotion->NextRow());
                }
                return true;
            }
            else
            {
                (ChatHandler(handler->GetSession())).PSendSysMessage(SMSG_CHAT_PLAYER_NOT_FOUND);
            }

            return true;
        }
};

class NpcPromotionWorld : public WorldScript
{
public:
    NpcPromotionWorld() : WorldScript("NpcPromotionWorld") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload)
        {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/mod_npc_promotion.conf";
#ifdef WIN32
            cfg_file = "mod_npc_promotion.conf";
#endif
            std::string cfg_def_file = cfg_file + ".dist";

            sConfigMgr->LoadModulesConfigs();

            npcPromotionEnabled = sConfigMgr->GetBoolDefault("NpcPromotion.enable", true);
            npcPromotionAnnounceEnable = sConfigMgr->GetBoolDefault("NpcPromotion.announceEnable", true);
            npcPromotionCount = sConfigMgr->GetIntDefault("NpcPromotion.count", 1);
            npcPromotionEnableIpLimit = sConfigMgr->GetBoolDefault("NpcPromotion.enableIpLimit", true);
            npcPromotionIpCount = sConfigMgr->GetIntDefault("NpcPromotion.countIp", 1);
            npcPromotionMaxLevel = sConfigMgr->GetIntDefault("NpcPromotion.maxLevel", 80);
            npcPromotionMoney = sConfigMgr->GetIntDefault("NpcPromotion.money", 25000000);

            NpcPromotionBagEnable = sConfigMgr->GetBoolDefault("NpcPromotion.bagEnable", true);
            NpcPromotionEquippedbags = sConfigMgr->GetBoolDefault("NpcPromotion.equippedbags", true);
            npcPromotionBag = sConfigMgr->GetIntDefault("NpcPromotion.bag", 20400);
            npcPromotionBagAmount = sConfigMgr->GetIntDefault("NpcPromotion.bagAmount", 4);

            NpcPromotionMountEnable = sConfigMgr->GetBoolDefault("NpcPromotion.mountEnable", true);
            NpcPromotionMountReward = sConfigMgr->GetIntDefault("NpcPromotion.mountReward", 74856);

            NpcPromotionWarriorTankEnabled = sConfigMgr->GetBoolDefault("NpcPromotionWarriorTank.enable", true);
            NpcPromotionWarriorDpsEnabled = sConfigMgr->GetBoolDefault("NpcPromotionWarriordps.enable", true);
        }
    }
};

void AddNpcPromotionScripts()
{
    new NpcPromotionAnnouncer();
    new NpcPromotionWorld();
    new npc_promocion();
    new NpcPromotionCommand();
}
