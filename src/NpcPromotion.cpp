/**
    Nefertumm and Stevej
    https://github.com/Nefertumm/
    https://github.com/pangolp
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"
#include "NpcPromotion.h"

static bool npcPromotionEnabled, npcPromotionAnnounceEnable;
static int npcPromotionCount, npcPromotionIpCount;
static bool NpcPromotionWarriorTankEnabled, NpcPromotionWarriorDpsEnabled;

class NpcPromotionAnnouncer : public PlayerScript
{
    public:
        NpcPromotionAnnouncer() : PlayerScript("NpcPromotionAnnouncer") {}

        void OnLogin(Player* player)
        {
            if (npcPromotionAnnounceEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Npc Promotion |rmodule.");
            }
        }
};

void equiparWarriorTank(Player* player)
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
            uint8 countIp = getIpPromotionCount(player->GetSession()->GetAccountId());

            if ((player->getLevel() < 80) && (((countAccount < npcPromotionCount)) || (countIp < npcPromotionIpCount)))
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
                        break;
                    case CLASS_MAGE:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_MAGE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);
                        break;
                    case CLASS_WARLOCK:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_WARLOCK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 15);
                        break;
                    case CLASS_DRUID:
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 16);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_HEAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 17);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_CASTER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 18);
                        AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_DRUID_MELEE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 19);
                        break;
                    default:
                        break;
                }
            }

            if (player->getLevel() == 80)
                AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_TP_DALARAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);

            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_CLOSE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);
            SendGossipMenuFor(player, NPC_TEXT_ID, creature);
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
        {
            ClearGossipMenuFor(player);

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    addPromotionLog(player);
                    player->GiveLevel(80);
                    player->InitTalentForLevel();
                    player->SetUInt32Value(PLAYER_XP, 0);
                    player->AddItem(20400, 3);
                    player->EquipNewItem(INVENTORY_SLOT_BAG_START, 20400, true);
                    player->learnSpell(PLATE_MAIL);
                    equiparWarriorTank(player);
                    /* 2000 de oro */
                    player->ModifyMoney(20000000);
                    player->UpdateSkillsToMaxSkillsForLevel();
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    addPromotionLog(player);
                    player->GiveLevel(80);
                    player->AddItem(20400, 4);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 20:
                    break;
                case GOSSIP_ACTION_INFO_DEF + 21:
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

            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());

            npcPromotionEnabled = sConfigMgr->GetBoolDefault("NpcPromotion.enable", true);
            npcPromotionAnnounceEnable = sConfigMgr->GetBoolDefault("NpcPromotion.announceEnable", true);
            npcPromotionCount = sConfigMgr->GetIntDefault("NpcPromotion.count", 1);
            npcPromotionIpCount = sConfigMgr->GetIntDefault("NpcPromotion.countIp", 1);

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
}
