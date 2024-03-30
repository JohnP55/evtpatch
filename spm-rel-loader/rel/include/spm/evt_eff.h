#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_eff {

extern "C" {
// NPCname is unused if mode is not 1, X Y and Z are unused if mode 2 is not used.
// evt_eff_exclamation_question (int Mode, char * instanceName, char * NPCname, isQuestion/isExclamation, int unknown, float x, float y, float z, int unknown, int unknown, int unknown)
// Modes: 0 = Mario, 1 = NPC, 2 = Fixed coodernates
EVT_DECLARE_USER_FUNC(evt_eff_exclamation_question, 11)

}

}