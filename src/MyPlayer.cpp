/**
    Nefertumm and Stevej
    https://github.com/Nefertumm/
    https://github.com/pangolp
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Config.h"
#include "Chat.h"

class NpcPromotionAnnouncer : public PlayerScript
{
    public:
        NpcPromotionAnnouncer() : PlayerScript("NpcPromotionAnnouncer") {}

        void OnLogin(Player* player)
        {
            if (announceEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Npc Promotion |rmodule.");
            }
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

            recruitEnabled = sConfigMgr->GetBoolDefault("NpcPromotion.enable", false);
            recruitAnnounceEnable = sConfigMgr->GetBoolDefault("NpcPromotion.announceEnable", false);

        }
    }
};

void AddNpcPromotionScripts() {
    new NpcPromotionAnnouncer();
}
