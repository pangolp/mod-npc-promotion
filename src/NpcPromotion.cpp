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
#include "GossipDef.h"
#include "ScriptedGossip.h"

#if AC_COMPILER == AC_COMPILER_GNU
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

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
    if (NpcPromotionBagEnable)
    {
        if (NpcPromotionEquippedbags)
        {
            for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
            {
                if (Item* bag = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                {
                    player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
                }
                player->EquipNewItem(slot, npcPromotionBag, true);
            }
        }
        else
        {
            player->AddItem(npcPromotionBag, npcPromotionBagAmount);
        }
    }

    // Riding
    if (NpcPromotionMountEnable)
    {
        player->learnSpell(SKILL_RIDING_75);
        player->learnSpell(SKILL_RIDING_100);
        player->learnSpell(SKILL_RIDING_FLYING);
        player->learnSpell(SKILL_RIDING_ARTISING);
        player->learnSpell(NpcPromotionMountReward);
    }
    player->UpdateSkillsToMaxSkillsForLevel();
}

void equipmentPlayer(Player* player, uint8 playerClass, uint8 playerTeam, std::string playerFunction)
{
    QueryResult result = LoginDatabase.Query("SELECT `head`, `neck`, `shoulders`, `body`, `chest`, `waist`, `legs`, `feet`, `wrists`, `hands`, `finger1`, `finger2`, `trinket1`, `trinket2`, `back`, `mainhand`, `offhand`, `ranged`, `tabard` FROM `mod_npc_promotion_items` WHERE `class`={} AND `faction`={} AND `function`='{}';", playerClass, playerTeam, playerFunction);

    if (result)
    {
        Field* fields = result->Fetch();
        for (int slot = EQUIPMENT_SLOT_START; slot < EQUIPMENT_SLOT_END; slot++)
        {
            if (fields[slot].Get<uint32>() == 0)
            {
                continue;
            }
            player->EquipNewItem(slot, fields[slot].Get<uint32>(), true);
        }
    }
}

class npc_promocion : public CreatureScript
{
    public:
        npc_promocion() : CreatureScript("npc_promocion") {}

        uint8 getAccountPromotionCount(uint32 accountId)
        {
            QueryResult result = LoginDatabase.Query("SELECT COUNT(`accountId`) FROM `mod_npc_promotion_log` WHERE `accountId`={}", accountId);

            if (result)
            {
                Field* fields = result->Fetch();
                return fields[0].Get<uint8>();
            }

            return 0;
        }

        uint8 getIpPromotionCount(uint32 accountId)
        {
            QueryResult result = LoginDatabase.Query("SELECT COUNT(`ip`) FROM `mod_npc_promotion_log` WHERE `accountId`={}", accountId);

            if (result)
            {
                Field* fields = result->Fetch();
                return fields[0].Get<uint8>();
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

            QueryResult result = LoginDatabase.Query("INSERT INTO `mod_npc_promotion_log` (`accountId`, `accountName`, `characterName`, `ip`) VALUES ({}, '{}', '{}', '{}')", accountId, accountName.c_str(), characterName.c_str(), ipAccount.c_str());
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            uint8 countAccount = getAccountPromotionCount(player->GetSession()->GetAccountId());
            int8 countIp;

            if (npcPromotionEnableIpLimit)
            {
                countIp = getIpPromotionCount(player->GetSession()->GetAccountId());
            }
            else
            {
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
            {
                AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_TP_DALARAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            }

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
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "tank");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 2:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 3:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "tank");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 4:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "heal");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 5:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 6:
                    {
                        addPromotionLog(player);
                        player->learnSpell(STAVES);
                        promotionPlayerTemplate(player);
                        player->learnSpell(MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                        switch (player->getRace())
                        {
                            case RACE_DRAENEI:
                            case RACE_NIGHTELF:
                            case RACE_ORC:
                            case RACE_TROLL:
                            case RACE_BLOODELF:
                                {
                                    player->AddItem(AMMUNITION_ARROW, AMMUNITION_COUNT);
                                }
                                break;
                            case RACE_DWARF:
                            case RACE_TAUREN:
                                {
                                    player->AddItem(AMMUNITION_BULLETS, AMMUNITION_COUNT);
                                }
                                break;
                        default:
                            break;
                        }
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 7:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 8:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "heal");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 9:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 10:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "tank");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 11:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(PLATE_MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 12:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "caster");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 13:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "melee");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 14:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        player->learnSpell(MAIL);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "heal");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 15:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 16:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "dps");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 17:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "tank");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 18:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "heal");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 19:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "caster");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 20:
                    {
                        addPromotionLog(player);
                        promotionPlayerTemplate(player);
                        equipmentPlayer(player, player->getClass(), player->GetTeamId(), "melee");
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF + 21:
                    break;
                case GOSSIP_ACTION_INFO_DEF + 22:
                    {
                        // Teleport to Dalaran if it is 80.
                        player->TeleportTo(571, 5804.14f, 624.77f, 647.76f, 1.64f);
                    }
                    break;
                default:
                    break;
            }
            CloseGossipMenuFor(player);
            return true;
        }
};

using namespace Acore::ChatCommands;

class NpcPromotionCommand : public CommandScript
{
public:
    NpcPromotionCommand() : CommandScript("NpcPromotionCommand") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable promotionSetCommandTable =
        {
            { "view", SEC_MODERATOR, false, &HandleViewNpcPromotionCommand, "" }
        };

        static ChatCommandTable commandTable =
        {
            { "promotion", SEC_MODERATOR, false, nullptr, "", promotionSetCommandTable}
        };

        return commandTable;
    }

    static void getTargetAccountIdByName(std::string& name, uint32& accountId)
    {
        QueryResult result = CharacterDatabase.Query("SELECT `account` FROM `characters` WHERE `name`='{}';", name);
        accountId = (*result)[0].Get<int32>();
    }

    static bool HandleViewNpcPromotionCommand(ChatHandler* handler, const char* args)
    {
        if (!*args)
        {
            return false;
        }

        Player* target = nullptr;
        std::string playerName;

        if (!handler->extractPlayerTarget((char*)args, &target, nullptr, &playerName))
        {
            return false;
        }

        uint32 playerAccountId;

        if (target)
        {
            playerAccountId = target->GetSession()->GetAccountId();
        }
        else
        {
            getTargetAccountIdByName(playerName, playerAccountId);
        }

        QueryResult result = LoginDatabase.Query("SELECT * FROM `account` WHERE `id`={};", playerAccountId);
        QueryResult resultPromotion = LoginDatabase.Query("SELECT * FROM `mod_npc_promotion_log` WHERE `accountId`={};", playerAccountId);

        if (result)
        {
            Field* fields = result->Fetch();
            std::string accountName = fields[1].Get<std::string>();
            std::string email = fields[7].Get<std::string>();
            std::string ip = fields[10].Get<std::string>();
            uint8 online = fields[16].Get<uint8>();
            uint32 recruiter = fields[23].Get<uint32>();

            handler->PSendSysMessage("                                                      ");
            handler->PSendSysMessage(" ------------------------------------------------     ");
            handler->PSendSysMessage(" Username : {}", accountName.c_str());
            handler->PSendSysMessage(" Email : {}", email.c_str());
            handler->PSendSysMessage(" Last IP : {}", ip.c_str());
            handler->PSendSysMessage(" Online : {}", online ? " [ONLINE]" : "[OFFLINE]");
            handler->PSendSysMessage(" Recruiter : {}", recruiter);
            handler->PSendSysMessage(" ------------------------------------------------     ");
            handler->PSendSysMessage("      --- LISTA DE PERSONAJES ---                     ");

            if (resultPromotion)
            {
                do
                {
                    Field* promotion = resultPromotion->Fetch();
                    std::string characterName = promotion[3].Get<std::string>();
                    std::string ip = promotion[4].Get<std::string>();
                    std::string date = promotion[5].Get<std::string>();
                    handler->PSendSysMessage("Character: {}, {}: {}, date: {}", characterName.c_str(), ip.c_str(), date.c_str());
                }
                while (resultPromotion->NextRow());
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
            sConfigMgr->LoadModulesConfigs();

            npcPromotionEnabled = sConfigMgr->GetOption<bool>("NpcPromotion.enable", true);
            npcPromotionAnnounceEnable = sConfigMgr->GetOption<bool>("NpcPromotion.announceEnable", true);
            npcPromotionCount = sConfigMgr->GetOption<uint8>("NpcPromotion.count", 1);
            npcPromotionEnableIpLimit = sConfigMgr->GetOption<bool>("NpcPromotion.enableIpLimit", true);
            npcPromotionIpCount = sConfigMgr->GetOption<uint8>("NpcPromotion.countIp", 1);
            npcPromotionMaxLevel = sConfigMgr->GetOption<uint8>("NpcPromotion.maxLevel", 80);
            npcPromotionMoney = sConfigMgr->GetOption<uint16>("NpcPromotion.money", 25000000);

            NpcPromotionBagEnable = sConfigMgr->GetOption<bool>("NpcPromotion.bagEnable", true);
            NpcPromotionEquippedbags = sConfigMgr->GetOption<bool>("NpcPromotion.equippedbags", true);
            npcPromotionBag = sConfigMgr->GetOption<uint8>("NpcPromotion.bag", 20400);
            npcPromotionBagAmount = sConfigMgr->GetOption<uint8>("NpcPromotion.bagAmount", 4);

            NpcPromotionMountEnable = sConfigMgr->GetOption<bool>("NpcPromotion.mountEnable", true);
            NpcPromotionMountReward = sConfigMgr->GetOption<uint8>("NpcPromotion.mountReward", 74856);

            NpcPromotionWarriorTankEnabled = sConfigMgr->GetOption<bool>("NpcPromotionWarriorTank.enable", true);
            NpcPromotionWarriorDpsEnabled = sConfigMgr->GetOption<bool>("NpcPromotionWarriordps.enable", true);
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
