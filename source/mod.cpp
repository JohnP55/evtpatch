#include "mod.h"
#include "cutscene_helpers.h"
#include "evt_cmd.h"
#include "evtpatch.h"
#include "exception.h"
#include "patch.h"
#include "romfontexpand.h"

#include <common.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_cam.h>
#include <spm/evt_door.h>
#include <spm/evt_eff.h>
#include <spm/evt_fade.h>
#include <spm/evt_fairy.h>
#include <spm/evt_guide.h>
#include <spm/evt_mario.h>
#include <spm/evt_mobj.h>
#include <spm/evt_msg.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/evt_snd.h>
#include <spm/fadedrv.h>
#include <spm/fontmgr.h>
#include <spm/map_data.h>
#include <spm/msgdrv.h>
#include <spm/npcdrv.h>
#include <spm/seqdef.h>
#include <spm/seqdrv.h>
#include <msl/stdio.h>
#include <msl/string.h>
#include <wii/ipc.h>
#include <wii/os.h>

namespace mod {

#define EVT_MSG_FLAG_DIRECT 1
bool gIsDolphin;

/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::gx::GXColor green {0, 255, 0, 255};
    f32 scale = 0.8f;
    const char * msg = "SPM Rel Loader";
    spm::fontmgr::FontDrawStart();
    spm::fontmgr::FontDrawEdge();
    spm::fontmgr::FontDrawColor(&green);
    spm::fontmgr::FontDrawScale(scale);
    spm::fontmgr::FontDrawNoiseOff();
    spm::fontmgr::FontDrawRainbowColorOff();
    f32 x = -((spm::fontmgr::FontGetMessageWidth(msg) * scale) / 2);
    spm::fontmgr::FontDrawString(x, 200.0f, msg);
    seq_titleMainReal(wp);
}
static void titleScreenCustomTextPatch()
{
    seq_titleMainReal = spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main;
    spm::seqdef::seq_data[spm::seqdrv::SEQ_TITLE].main = &seq_titleMainOverride;
}

/*
    Platform detection to adjust certain settings
*/

static void checkForDolphin()
{
    // Thanks to TheLordScruffy for telling me about this
    gIsDolphin = wii::ipc::IOS_Open("/sys", 1) == -106;

    // If they ever fix that, it'll be in a version that's definitely new enough to have /dev/dolphin
    if (!gIsDolphin)
    {
        int ret = wii::ipc::IOS_Open("/dev/dolphin", 0);
        if (ret >= 0)
        {
            gIsDolphin = true;
            wii::ipc::IOS_Close(ret);
        }
    }
}

/*
    General mod functions
*/

spm::npcdrv::NPCTribeAnimDef animsJaydesNormal[] = {
    {0, "S_1"}, // Standing (idle)
    {1, "W_1"}, // Walking
    {2, "R_1"}, // Running
    {3, "T_1"}, // Talking
    {-1, nullptr}
};
spm::npcdrv::NPCTribeAnimDef animsJaydesSad[] = {
    {0, "S_3b"}, // Standing (idle)
    {25, "S_3a"}, // To sad
    {2, "S_3c"}, // sad back to normal
    {3, "T_3"}, // Talking
    {-1, nullptr}
};
spm::npcdrv::NPCTribeAnimDef animsJaydesAngry[] = {
    {0, "S_2b"}, // Standing (idle)
    {25, "S_2a"}, // To angry
    {2, "S_2c"}, // angry back to normal
    {3, "T_2"}, // Talking
    {-1, nullptr}
};
spm::npcdrv::NPCTribeAnimDef animsJaydesShocked[] = {
    {0, "S_5"}, // Standing (idle)
    {3, "T_4"}, // Talking
    {-1, nullptr}
};
spm::npcdrv::NPCTribeAnimDef animsJaydesThinking[] = { // don't wanna see a talking animation for "..."
    {0, "S_1"}, // Standing (idle)
    {3, "S_1"}, // Talking
    {-1, nullptr}
};
spm::npcdrv::NPCTribeAnimDef animsJaydesThinkingAngry[] = {
    {0, "S_2b"}, // Standing (idle)
    {1, "S_2a"}, // To angry
    {2, "S_2c"}, // angry back to normal
    {3, "S_2b"}, // Talking
    {-1, nullptr}
};

spm::npcdrv::NPCTribeAnimDef animsLuvbiAngry[] = {
    {0, "S_2b"}, // Standing (idle)
    {1, "W_1"}, // Walking
    {2, "R_1"}, // Running
    {25, "S_2a"}, // To angry
    {26, "S_2c"}, // angry back to normal
    {3, "T_2"}, // Talking
    {-1, nullptr}
};

EVT_BEGIN(luvbiAsyncWalking)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("angel"), 1100, 0, 0, FLOAT(80.0), 1, 0, 0)
    RETURN()
EVT_END()

EVT_BEGIN(luvbiAsyncBombs)
    DO(6)
        USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_FLAME_BOMB1"), PTR("angel"))
        WAIT_MSEC(400)
    WHILE()
    RETURN()
EVT_END()

const char diaryScreenEnding[] = "<se 2>"
"<diary>"
"<wpos -280 100 560 185>"
"And so, the Queen of the\n"
"Underwhere bestowed upon\n"
"Mario the last Pure Heart.\n"
"<k>"
"<p>"
"What happened back there?\n"
"That we may never know.\n"
"<k>"
"<p>"
"Alas, the question was of little\n"
"importance to our crew as the\n"
"void grew larger and larger.\n"
"<k>"
"<p>"
"And let's be real,\n"
"she deserved it."
"<k>";

EVT_BEGIN(funnyCutscene)
    //GOTO(0)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>I do, indeed, have a Pure\nHeart to offer to you, heroes.\n<k><p>However, I cannot give it\nto you as promised.<k>"), 0, PTR("enma"))
    WAIT_MSEC(250)
    //LBL(0)
    MARIO_SPAWN_QUESTION_MARK()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_10"), 0)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("S_1"), 0)
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<fairy><p>What seems to be the issue,\nQueen Jaydes?<k>"), 0, PTR("__guide__"))
    WAIT_MSEC(300)
    NPC_SET_ANIMDEF_AND_ANIM("enma", animsJaydesSad, 1)
    NPC_FACE_DIR("enma", NPC_FACEDIR_LEFT)
    NPC_SET_ANIMDEF_AND_ANIM_LOOP("enma", animsJaydesSad, 1, 0)
    WAIT_MSEC(300)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>A very long time ago, the\nPure Heart was hidden away\ninto a form so inconspicuous,\n<k><p>that it would never be\nrecognized as what it truly is\nif one seeked it.\n<k><p>However, I don't know how\nto undo that transformation\nby myself.<k>"), 0, PTR("enma"))
    WAIT_MSEC(500)
    USER_FUNC(spm::evt_npc::evt_npc_set_position, PTR("angel"), 1100, 100, 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, 0, PTR("stg7_1_123"), 0, PTR("angel")) // original fits already
    WAIT_MSEC(100)
    NPC_SPAWN_EXCLAMATION_MARK("enma")
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 2, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("enma"), 1)
    NPC_SET_ANIMDEF_AND_ANIM("enma", animsJaydesShocked, 0)
    WAIT_MSEC(1000)
    NPC_SET_ANIMDEF_AND_ANIM_LOOP("enma", animsJaydesNormal, 1, 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>Oh Grambi, not that brat again...<k>"), 0, PTR("enma"))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 850, 175, 484, 850, 175, -16, 500, 11)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("angel"), 1) // func_80108194 is at 801077b4 in us0, no clue what this function does but it was called in the og script
    NPC_SET_ANIMDEF_AND_ANIM("angel", animsLuvbiAngry, 0)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("angel"), 925, 0, 0, FLOAT(80.0), 1, 0, 0)
    WAIT_FRM(1)
    USER_FUNC(spm::evt_npc::func_80108194, PTR("angel"), 0)
    NPC_SET_ANIMDEF_AND_ANIM_LOOP("enma", animsJaydesThinkingAngry, 1, 0)
    WAIT_MSEC(25)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>Mother!\n<k><p>I shan't meet mine prince hither!\nThy place fucking blows!<k>"), 0, PTR("angel"))
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p><small><scale 0.67>Urgh...<wait 750>"), 0, PTR("enma"))
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>There hath been yet ZERO\ncharming gentlemen hither, only\nSINNERS and old fatty types!\n<k><p>I hath decided for myself!\nI shall go live with Father,\nwhere my prince must reside!<k>"), 0, PTR("angel"))
    WAIT_MSEC(250)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p><small><scale 0.67>.<wait 250>.<wait 250>.<wait 250>"), 0, PTR("enma"))
    NPC_SPAWN_EXCLAMATION_MARK("enma")
    WAIT_MSEC(2000)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 2, 1)
    USER_FUNC(spm::evt_npc::evt_npc_wait_anim_end, PTR("enma"), 1)
    NPC_SET_ANIMDEF_AND_ANIM("enma", animsJaydesNormal, 0) // go back to actually talking
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>An idea strikes...<k>"), 0, PTR("enma"))
    WAIT_MSEC(250)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>Stay right there heroes, I\nwill be back in just a moment.<k>"), 0, PTR("enma"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 1, 1) // Walking
    NPC_FACE_DIR("enma", NPC_FACEDIR_RIGHT)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("enma"), 975, 0, 0, FLOAT(80.0), 1, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 0, 1) // Idle
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>Luvbi, you're coming with me.<k>"), 0, PTR("enma"))
    RUN_EVT_ID(luvbiAsyncWalking, LW(0))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 1, 1) // Walking
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("enma"), 1100, 0, 0, FLOAT(80.0), 1, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 0, 1) // Idle
    WAIT_FOR_EVT(LW(0), LW(1))
    WAIT_MSEC(200)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_DOKAN_MAKE1"), PTR("angel"))
    WAIT_MSEC(1000)
    MARIO_SPAWN_QUESTION_MARK()
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_10"), 0)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_P_V_PEACH_PANIC1"), PTR("angel"))
    WAIT_MSEC(250)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_11"), 0)
    WAIT_MSEC(750)
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_BS_DMN_FLAME_BOMB1"), PTR("angel"))
    WAIT_MSEC(400)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("E_5"), 0)
    RUN_EVT_ID(luvbiAsyncBombs, LW(0))
    USER_FUNC(spm::evt_mario::evt_mario_wait_anim)
    USER_FUNC(spm::evt_mario::evt_mario_set_pose, PTR("T_7"), 0)
    WAIT_FOR_EVT(LW(0), LW(1))
    USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 0, 3000)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<fairy><shake><p>What...<wait 250> did she just....<wait 250>?<k>"), 0, PTR("__guide__"))
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 1, 1) // Walking
    NPC_FACE_DIR("enma", NPC_FACEDIR_LEFT)
    USER_FUNC(spm::evt_npc::evt_npc_walk_to, PTR("enma"), 900, 0, 0, FLOAT(80.0), 1, 0, 0)
    USER_FUNC(spm::evt_npc::evt_npc_set_anim, PTR("enma"), 0, 1) // Idle
    USER_FUNC(spm::evt_npc::evt_npc_get_position, PTR("enma"), LW(5), LW(6), LW(7))
    SUB(LW(5), 40)
    ADD(LW(6), 50)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>I'm back. I've got your pure\nheart right here.<k>"), 0, PTR("enma"))
    USER_FUNC(spm::evt_snd::evt_snd_sfxon_npc, PTR("SFX_EVT_PUREHEART1"), PTR("angel"))
    USER_FUNC(spm::evt_eff::evt_eff, PTR("heart"), PTR("pure_heart"), 7, LW(5), LW(6), LW(7), 0, 0, 0, 0, 0, 0, 0, 0)
    USER_FUNC(spm::evt_msg::evt_msg_print, EVT_MSG_FLAG_DIRECT, PTR("<p>Just...\n<k><p><shake><scale 0.8>don't ask what happened back there...<k>"), 0, PTR("enma"))
    USER_FUNC(spm::evt_cam::evt_cam3d_evt_zoom_in, 1, 798, 200, 484, 798, 200, -230, 500, 11)
    SET(LW(0), PTR("heart"))
    SET(LW(1), 2)
    RUN_CHILD_EVT(&evt_cutscene_pure_heart)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR(diaryScreenEnding), 0, 0)
    WAIT_MSEC(1000)
    SET(GSW(0), 353)
    SET(LF(10), 0)
    SET(LW(0), 30)
    RUN_CHILD_EVT(&evt_memcard_save)
    WAIT_MSEC(1000)
    USER_FUNC(spm::evt_snd::evt_snd_bgmoff_f_d, 2, 3000)
    USER_FUNC(spm::evt_fade::evt_set_transition, 28, 1)
    USER_FUNC(spm::evt_seq::evt_seq_mapchange, PTR("ls4_11"), 0)
RETURN_FROM_CALL()


void main()
{
    wii::os::OSReport("SPM Rel Loader: the mod has ran!\n");
    wii::os::OSReport("%p\n", spm::map_data::mapDataPtr("an1_04")->initScript);

    titleScreenCustomTextPatch();
    checkForDolphin();
    exceptionPatch(); // Seeky's exception handler from Practice Codes
    romfontExpand(); // Font expansion patch from Practice Codes
    evtpatch::evtmgrExtensionInit(); // Initialize EVT scripting extension

    spm::map_data::MapData * an1_04_md = spm::map_data::mapDataPtr("an1_04");
    spm::evtmgr::EvtScriptCode* jaydesRequestLink = evtpatch::getEvtInstruction(an1_04_md->initScript, 139);
    spm::evtmgr::EvtScriptCode* jaydesRequest = reinterpret_cast<spm::evtmgr::EvtScriptCode*>(evtpatch::getInstructionArgv(jaydesRequestLink)[3]);
    evtpatch::hookEvtReplaceBlock(jaydesRequest, 25, (spm::evtmgr::EvtScriptCode*)funnyCutscene, 179);
}

}
