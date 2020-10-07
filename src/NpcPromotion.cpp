/**
    Nefertumm and Stevej
    https://github.com/Nefertumm/
    https://github.com/pangolp
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

static bool npcPromotionEnabled, npcPromotionAnnounceEnable;
static int npcPromotionCount;
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

enum NpcPromotionGossip
{
    GOSSIP_MENU_PROMOTION = 62000,
    GOSSIP_MENU_WARRIOR_TANK = 0,
    GOSSIP_MENU_WARRIOR_DPS = 1,
    GOSSIP_MENU_PALADIN_TANK = 2,
    GOSSIP_MENU_PALADIN_HEAL = 3,
    GOSSIP_MENU_PALADIN_DPS = 4,
    GOSSIP_MENU_HUNTER = 5,
    GOSSIP_MENU_ROGUE = 6,
    GOSSIP_MENU_PRIEST_HEAL = 7,
    GOSSIP_MENU_DEATH_KNIGHT_TANK = 8,
    GOSSIP_MENU_DEATH_KNIGHT_DPS = 9,
    GOSSIP_MENU_PRIEST_DPS = 10,
    GOSSIP_MENU_SHAMAN_CASTER = 11,
    GOSSIP_MENU_SHAMAN_MELEE = 12,
    GOSSIP_MENU_MAGE = 13,
    GOSSIP_MENU_WARLOCK = 14,
    GOSSIP_MENU_DRUID_TANK = 15,
    GOSSIP_MENU_DRUID_HEAL = 16,
    GOSSIP_MENU_DRUID_CASTER = 17,
    GOSSIP_MENU_DRUID_MELEE = 18,
    GOSSIP_MENU_CLOSE = 19,
};

enum NpcPromotionQuest
{
    QUEST_WHERE_KINGS_WALK = 13188,
    QUEST_WARCHIEFS_BLESSING = 13189,
};

enum NpcTextId
{
    NPC_TEXT_ID = 90000
};

enum NpcItem
{
    BAG_WARRIOR_TANK = 29886
};

class npc_promocion : public CreatureScript
{
    public:
        npc_promocion() : CreatureScript("npc_promocion") {}

        uint8 getAccountPromotionCount(uint32 accountId)
        {
            QueryResult result = CharacterDatabase.PQuery("SELECT COUNT(`accountId`) FROM `promotion` WHERE `accountId` = %u", accountId);

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

            QueryResult result = CharacterDatabase.PQuery("INSERT INTO `promotion` (`accountId`, `accountName`, `characterName`) VALUES (%u, '%s', '%s')", accountId, accountName.c_str(), characterName.c_str());
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            uint8 count = getAccountPromotionCount(player->GetSession()->GetAccountId());

            if ((player->getLevel() < 80) && ((count < npcPromotionCount)))
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
                    player->AddItem(20400, 4);
                    player->AddItem(BAG_WARRIOR_TANK, 1);
                    player->UpdateSkillsToMaxSkillsForLevel();
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    addPromotionLog(player);
                    player->GiveLevel(80);
                    player->AddItem(20400, 4);
                    break;
                case GOSSIP_ACTION_INFO_DEF + 20:
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
