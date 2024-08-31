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
#include "SpellMgr.h"

struct NpcPromotion
{
    bool ENABLED, ANNOUNCE_ENABLE;
    int COUNT, IP_COUNT, LEVEL, MONEY, BAG, BAG_AMOUNT, MOUNT_REWARD;
    bool WARRIOR_TANK, WARRIOR_DPS;
    bool ENABLE_IP_LIMIT, BAG_ENABLE, EQUIPPED_BAGS, MOUNT_ENABLE;
    bool EMBLEMS;
    uint32 EMBLEMS_ID, EMBLEMS_COUNT;
    uint32 ACORE_STRING_MESSAGE;
    bool enableMoney, enableLevel;
    bool ridingArtising;
};

NpcPromotion npcPromotion;

class NpcPromotionAnnouncer : public PlayerScript
{
    public:
        NpcPromotionAnnouncer() : PlayerScript("NpcPromotionAnnouncer") {}

        void OnLogin(Player* player)
        {
            if (npcPromotion.ANNOUNCE_ENABLE)
            {
                ChatHandler(player->GetSession()).SendSysMessage(npcPromotion.ACORE_STRING_MESSAGE);
            }
        }
};

void promotionPlayerTemplate(Player* player)
{
    if (npcPromotion.enableLevel)
    {
        player->GiveLevel(npcPromotion.LEVEL);
        player->InitTalentForLevel();
        player->SetUInt32Value(PLAYER_XP, 0);
    }

    if (npcPromotion.enableMoney)
        player->ModifyMoney(npcPromotion.MONEY);

    if (npcPromotion.BAG_ENABLE)
    {
        if (npcPromotion.EQUIPPED_BAGS)
        {
            for (int slot = INVENTORY_SLOT_BAG_START; slot < INVENTORY_SLOT_BAG_END; slot++)
            {
                if (Item* bag = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot))
                {
                    player->DestroyItem(INVENTORY_SLOT_BAG_0, slot, true);
                }
                player->EquipNewItem(slot, npcPromotion.BAG, true);
            }
        }
        else
        {
            player->AddItem(npcPromotion.BAG, npcPromotion.BAG_AMOUNT);
        }
    }

    if (npcPromotion.MOUNT_ENABLE)
    {
        player->learnSpell(SKILL_RIDING_75);
        player->learnSpell(SKILL_RIDING_100);
        player->learnSpell(SKILL_RIDING_FLYING);
        if (npcPromotion.ridingArtising)
            player->learnSpell(SKILL_RIDING_ARTISING);
        player->learnSpell(SKILL_COLD_WEATHER_FLYING);
        if (npcPromotion.MOUNT_REWARD > 0)
        {
            player->learnSpell(npcPromotion.MOUNT_REWARD);
        }
        else
        {
            switch (player->getRace())
            {
                case RACE_HUMAN:
                    player->learnSpell(23228);
                    player->learnSpell(32239);
                    break;
                case RACE_DWARF:
                    player->learnSpell(23239);
                    player->learnSpell(32239);
                    break;
                case RACE_NIGHTELF:
                    player->learnSpell(23338);
                    player->learnSpell(32239);
                    break;
                case RACE_GNOME:
                    player->learnSpell(23223);
                    player->learnSpell(32239);
                    break;
                case RACE_DRAENEI:
                    player->learnSpell(35713);
                    player->learnSpell(32239);
                    break;
                case RACE_ORC:
                    player->learnSpell(23250);
                    player->learnSpell(32243);
                    break;
                case RACE_UNDEAD_PLAYER:
                    player->learnSpell(66846);
                    player->learnSpell(32243);
                    break;
                case RACE_TAUREN:
                    player->learnSpell(23248);
                    player->learnSpell(32243);
                    break;
                case RACE_TROLL:
                    player->learnSpell(23242);
                    player->learnSpell(32243);
                    break;
                case RACE_BLOODELF:
                    player->learnSpell(33660);
                    player->learnSpell(32243);
                    break;
                default:
                    break;
            }
        }
    }

    player->UpdateSkillsToMaxSkillsForLevel();
}

void equipmentPlayer(Player* player, uint8 playerClass, uint8 playerTeam, std::string playerFunction)
{
    if (npcPromotion.EMBLEMS)
    {
        player->AddItem(npcPromotion.EMBLEMS_ID, npcPromotion.EMBLEMS_COUNT);
    }
    else
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
            if (npcPromotion.ENABLE_IP_LIMIT)
            {
                uint32 accountId = player->GetSession()->GetAccountId();
                std::string accountName;
                AccountMgr::GetName(accountId, accountName);
                std::string characterName = player->GetSession()->GetPlayerName();
                std::string ipAccount = player->GetSession()->GetRemoteAddress();

                QueryResult result = LoginDatabase.Query("INSERT INTO `mod_npc_promotion_log` (`accountId`, `accountName`, `characterName`, `ip`) VALUES ({}, '{}', '{}', '{}')", accountId, accountName.c_str(), characterName.c_str(), ipAccount.c_str());
            }
            return true;
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            int8 countIp;
            int8 countAccount;

            if (npcPromotion.ENABLE_IP_LIMIT)
            {
                countAccount = getAccountPromotionCount(player->GetSession()->GetAccountId());
                countIp = getIpPromotionCount(player->GetSession()->GetAccountId());
            }
            else
            {
                countIp = -1;
                countAccount = -1;
            }

            if ((player->GetLevel() < npcPromotion.LEVEL) && (((countAccount < npcPromotion.COUNT)) || (countIp < npcPromotion.IP_COUNT)))
            {
                switch (player->getClass())
                {
                    case CLASS_WARRIOR:
                        if (npcPromotion.WARRIOR_TANK)
                            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_WARRIOR_TANK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                        if (npcPromotion.WARRIOR_DPS)
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

            AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_CLOSE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);

            if (player->GetLevel() >= npcPromotion.LEVEL)
            {
                AddGossipItemFor(player, GOSSIP_MENU_PROMOTION, GOSSIP_MENU_TP_DALARAN, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);
            }

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
            { "view", HandleViewNpcPromotionCommand,  SEC_MODERATOR,  Console::No }
        };

        static ChatCommandTable commandTable =
        {
            { "promotion", promotionSetCommandTable }
        };

        return commandTable;
    }

    static void getTargetAccountIdByName(std::string& name, uint32& accountId)
    {
        QueryResult result = CharacterDatabase.Query("SELECT `account` FROM `characters` WHERE `name`='{}';", name);
        accountId = (*result)[0].Get<int32>();
    }

    static bool HandleViewNpcPromotionCommand(ChatHandler* handler, std::string playerName)
    {
        if (!normalizePlayerName(playerName))
        {
            return false;
        }

        ObjectGuid targetGuid = sCharacterCache->GetCharacterGuidByName(playerName);
        uint32 targetAccountId = sCharacterCache->GetCharacterAccountIdByGuid(targetGuid);

        if (!targetGuid)
        {
            return false;
        }

        QueryResult result = LoginDatabase.Query("SELECT * FROM `account` WHERE `id`={};", targetAccountId);
        QueryResult resultPromotion = LoginDatabase.Query("SELECT * FROM `mod_npc_promotion_log` WHERE `accountId`={};", targetAccountId);

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

            npcPromotion.ACORE_STRING_MESSAGE = sConfigMgr->GetOption<uint32>("npc.promotion.acore.string.message", 40000);

            npcPromotion.ENABLED = sConfigMgr->GetOption<bool>("npc.promotion.enable", true);
            npcPromotion.ANNOUNCE_ENABLE = sConfigMgr->GetOption<bool>("npc.promotion.announce.enable", true);
            npcPromotion.COUNT = sConfigMgr->GetOption<uint8>("npc.promotion.count", 1);
            npcPromotion.ENABLE_IP_LIMIT = sConfigMgr->GetOption<bool>("npc.promotion.enable.ip.limit", true);
            npcPromotion.IP_COUNT = sConfigMgr->GetOption<uint8>("npc.promotion.count.ip", 1);
            npcPromotion.enableLevel = sConfigMgr->GetOption<bool>("npc.promotion.enable.level", true);
            npcPromotion.LEVEL = sConfigMgr->GetOption<uint8>("npc.promotion.level", 80);
            npcPromotion.enableMoney = sConfigMgr->GetOption<bool>("npc.promotion.enable.money", true);
            npcPromotion.MONEY = sConfigMgr->GetOption<uint32>("npc.promotion.money", 25000000);

            npcPromotion.BAG_ENABLE = sConfigMgr->GetOption<bool>("npc.promotion.bag.enable", true);
            npcPromotion.EQUIPPED_BAGS = sConfigMgr->GetOption<bool>("npc.promotion.equipped.bags", true);
            npcPromotion.BAG = sConfigMgr->GetOption<uint16>("npc.promotion.bag", 20400);
            npcPromotion.BAG_AMOUNT = sConfigMgr->GetOption<uint8>("npc.promotion.bag.amount", 4);

            npcPromotion.MOUNT_ENABLE = sConfigMgr->GetOption<bool>("npc.promotion.mount.enable", true);
            npcPromotion.MOUNT_REWARD = sConfigMgr->GetOption<uint32>("npc.promotion.mount.reward", 74856);
            npcPromotion.ridingArtising = sConfigMgr->GetOption<bool>("npc.promotion.riding.artising", true);

            npcPromotion.EMBLEMS = sConfigMgr->GetOption<bool>("npc.promotion.emblems.enable", true);
            npcPromotion.EMBLEMS_ID = sConfigMgr->GetOption<uint32>("npc.promotion.emblems.id", 49426);
            npcPromotion.EMBLEMS_COUNT = sConfigMgr->GetOption<uint32>("npc.promotion.emblems.count", 2000);

            npcPromotion.WARRIOR_TANK = sConfigMgr->GetOption<bool>("npc.promotion.warrior.tank.enable", true);
            npcPromotion.WARRIOR_DPS = sConfigMgr->GetOption<bool>("npc.promotion.warrior.dps.enable", true);
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
