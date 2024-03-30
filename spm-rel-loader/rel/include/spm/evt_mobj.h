#pragma once

#include "evt_cmd.h"

namespace spm::evt_mobj {

extern "C" {

// Script to save the game
EVT_DECLARE(mobj_save_blk_sysevt)

// evt_mobj_flag_onoff(bool on, bool flag4, const char * name, u32 mask)
EVT_DECLARE_USER_FUNC(evt_mobj_flag_onoff, 4)

// evt_mobj_zyo(const char * name, int lockItemId, float x, float y, float z, float yRotation,
//              EvtScriptCode * interactScript, EvtScriptCode * openScript, EvtVar collectedVar)
EVT_DECLARE_USER_FUNC(evt_mobj_zyo, 9)

// evt_mobj_thako(int type, const char * name, float x, float y, float z, EvtScriptCode * interactScript,
//                EvtScriptCode * openScript, UNK arg8, UNK arg9)
EVT_DECLARE_USER_FUNC(evt_mobj_thako, 9)

// evt_mobj_kan(bool rotation, const char * name, float x, float y, float z, EvtScriptCode * interactScript,
//              UNK arg7)
EVT_DECLARE_USER_FUNC(evt_mobj_kan, 7)

// evt_mobj_arrow(const char * name, float x, float y, float z, UNK arg5)
EVT_DECLARE_USER_FUNC(evt_mobj_arrow, 5)

// evt_mobj_blk(s32 type, char * instanceName, float x, float y, float z, itemID/chanceTable, EvtScript * hitScript, bool disableHitting)
EVT_DECLARE_USER_FUNC(evt_mobj_blk, 8)

// evt_mobj_entry(char* instanceName, char* templateName, int unk1=0)
EVT_DECLARE_USER_FUNC(evt_mobj_entry, 3)

// evt_mobj_entry_from_template(int unk1=0, char* instanceName)
EVT_DECLARE_USER_FUNC(evt_mobj_entry_from_template, 2)

// evt_mobj_set_position(char* mobjName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_position, 4)

// evt_mobj_set_rotation(char* mobjName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_rotation, 4)

// evt_mobj_set_scale(char* mobjName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mobj_set_scale, 4)

}

}