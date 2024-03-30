#include "evt_cmd.h"
#include "mod.h"
#include "patch.h"
#include "exception.h"
#include "romfontexpand.h"
#include "evtpatch.h"

#include <types.h>
#include <spm/fontmgr.h>
#include <spm/seqdrv.h>
#include <spm/seqdef.h>
#include <wii/OSError.h>
#include <spm/evtmgr_cmd.h>
#include <spm/evt_mario.h>
#include <spm/evt_msg.h>
#include <spm/msgdrv.h>
#include <spm/seqdef.h>
#include <wii/string.h>
#include <wii/types.h>
#include <spm/evt_npc.h>
#include <spm/evt_seq.h>
#include <spm/evt_fade.h>
#include <spm/fadedrv.h>
#include <spm/evt_guide.h>
#include <spm/evt_fairy.h>
#include <spm/evt_mobj.h>
#include <wii/stdio.h>
#include <spm/evt_door.h>
#include <spm/npcdrv.h>
#include <spm/evt_snd.h>
#include <spm/mapdata.h>
#include <spm/evt_eff.h>
#include <spm/evt_cam.h>
#include <spm/rel/an1_04.h>

namespace mod {

/*
    Title Screen Custom Text
    Prints "SPM Rel Loader" at the top of the title screen
*/

static spm::seqdef::SeqFunc *seq_titleMainReal;
static void seq_titleMainOverride(spm::seqdrv::SeqWork *wp)
{
    wii::RGBA green {0, 255, 0, 255};
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
    General mod functions
*/

spm::npcdrv::NPCTribeAnimDef animsDimen[] = {
  {0, "S_2"}, // Flying (ID 0 is idle animation)
  {1, "S_1"}, // Standing
  {20, "W_3"}, // Finger click
  {21, "A_1a"}, // Magic start
  {22, "W_2c"}, // Land
  {23, "A_1c"}, // Magic cast
  {24, "A_2a"}, // Raise hands
  {25, "A_2b"}, // Wave hands
  {26, "A_2c"}, // Raise hands cast
  {27, "W_2a"}, // Fly
  {3, "T_2"}, // Talking (ID 3 is the ID to use when talking)
  {-1, nullptr}
};

EVT_BEGIN(testevt)
    USER_FUNC(spm::evt_msg::evt_msg_print, 1, PTR("<system><p>JohnP55 was here<k><o>"), 0, PTR("system"))
    USER_FUNC(spm::evt_msg::evt_msg_continue)
    RETURN()
EVT_END()

void main()
{
    wii::OSError::OSReport("SPM Rel Loader: the mod has ran!\n");
    wii::OSError::OSReport("%x\n",spm::mapdata::mapDataPtr("mac_02")->script);

    titleScreenCustomTextPatch();
    exceptionPatch(); // Seeky's exception handler from Practice Codes
    romfontExpand(); // Font expansion patch from Practice Codes
    
    spm::mapdata::MapData * mac_02_md = spm::mapdata::mapDataPtr("an1_04"); // Get the mapdata of an1_04
    evtpatch::hookEvt(mac_02_md->script, 0, testevt); // Replace the init script with ours
}

}
