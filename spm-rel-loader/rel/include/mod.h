#pragma once
#include "spm/evt_npc.h"
namespace mod {

#define MOD_VERSION "SPM Practice Codes v8"
#define DIMENTIO_TEMPLATE_ID 225


#define SET_CHAR_ANIMS(instanceName, tribeAnimDef) \
    USER_FUNC(spm::evt_npc::evt_npc_set_property, PTR(instanceName), spm::evt_npc::NPCProperty::ANIMS, PTR(tribeAnimDef))


#define SPAWN_CHARACTER(instanceName, modelName, initialAnimations) \
    USER_FUNC(spm::evt_npc::evt_npc_entry, PTR(instanceName), PTR(modelName), 0) \
    SET_CHAR_ANIMS(instanceName, initialAnimations)

extern "C" {
    extern void* fireBurstEvt;
}

void main();

}
