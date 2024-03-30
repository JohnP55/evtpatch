#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_npc {

extern "C" {

typedef enum {
    ANIMS = 0xe
} NPCProperty;

// evt_npc_set_position(const char * instanceName, float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_npc_set_position, 4)

// evt_npc_entry(const char * instanceName, const char * animPoseName, int instanceId)
EVT_DECLARE_USER_FUNC(evt_npc_entry, 3)

EVT_DECLARE_USER_FUNC(func_80103054, 1)
EVT_DECLARE_USER_FUNC(evt_npc_set_anim, 3)
EVT_DECLARE_USER_FUNC(func_80104694, 2)

// evt_npc_flag8_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_flag8_onoff, 3)

// evt_npc_animflag_onoff(const char * instanceName, bool on, u32 mask)
EVT_DECLARE_USER_FUNC(evt_npc_animflag_onoff, 3)

// evt_npc_set_property(const char * instanceName, int propertyId, int value)
EVT_DECLARE_USER_FUNC(evt_npc_set_property, 3)

// evt_npc_set_property(const char * instanceName, int partId, int propertyId, int value)
// partId -1 updates all parts
EVT_DECLARE_USER_FUNC(evt_npc_modify_part, 4)

EVT_DECLARE_USER_FUNC(func_801059d0, 2)

// evt_npc_entry_from_template(int instanceIdOverride, int templateId, int x, int y,
//                             int z, &int instanceId, &char * instanceName)
EVT_DECLARE_USER_FUNC(evt_npc_entry_from_template, 7)

EVT_DECLARE_USER_FUNC(func_801072a4, 1)
EVT_DECLARE_USER_FUNC(func_80107cfc, 0)
EVT_DECLARE_USER_FUNC(func_80107d20, 0)
EVT_DECLARE_USER_FUNC(func_80108194, 2)

// evt_npc_teleport_effect(bool appearDisappear, const char * instanceName)
// Found by JohnP55
EVT_DECLARE_USER_FUNC(evt_npc_teleport_effect, 2)

EVT_DECLARE_USER_FUNC(evt_npc_delete, 1) // 80102e68

EVT_DECLARE_USER_FUNC(evt_npc_flip_to, 2) // const char * instanceName, bool 3D/2D
EVT_DECLARE_USER_FUNC(evt_npc_finish_flip_instant, 1) // const char * instanceName
EVT_DECLARE_USER_FUNC(evt_npc_flip, 1) // const char * instanceName
EVT_DECLARE_USER_FUNC(evt_npc_wait_flip_finished, 0)
EVT_DECLARE_USER_FUNC(evt_npc_add_flip_part, 1) // const char * instanceName


EVT_DECLARE_USER_FUNC(evt_npc_walk_to, 8) // char * name, float xDest, float zDest, int limit_time, int param_5, u32 flags, int param_7, int param_8
EVT_DECLARE_USER_FUNC(evt_npc_jump_to, 6) // char * name, float destX, float destY, float destZ, float height, u32 timeMilliseconds
EVT_DECLARE_USER_FUNC(evt_npc_glide_to, 10) // char * name, float x, float y, float z, float limit_time, float speed, float y_curve, int easing_mode, u32 flags, int param_10

EVT_DECLARE_USER_FUNC(evt_npc_face_dir, 2) // char * instanceName, int dir (-1 = left, 1 = right)
EVT_DECLARE_USER_FUNC(evt_npc_wait_facing, 1) // char * instanceName

EVT_DECLARE_USER_FUNC(evt_wait_anim_end, 2)

EVT_DECLARE_USER_FUNC(evt_npc_set_scale, 4) // evt_npc_set_scale(char* npcName, float x, float y, float z)

EVT_DECLARE_USER_FUNC(evt_npc_get_position, 4) // evt_npc_get_position(char* npcName, out lw x, out lw y, out lw z)

EVT_DECLARE_USER_FUNC(evt_npc_set_unitwork, 3) // evt_npc_set_unitwork(char* npcName, int unitWorkId, void* val)

EVT_DECLARE_USER_FUNC(evt_npc_id_to_name, 2) // evt_npc_id_to_name(int instanceId, out lw name)
EVT_DECLARE_USER_FUNC(evt_set_axis_movement_unit, 2) // evt_set_axis_movement_unit(char* npcName, int axisMovementUnit)
EVT_DECLARE_USER_FUNC(evt_npc_face_mario, 1) // evt_npc_face_mario(char* npcName)
EVT_DECLARE_USER_FUNC(evt_npc_set_disp_func_2, 3) // evt_npc_set_disp_func_2(char* npcName, void* dispFunc2, int dispFunc2Param)
EVT_DECLARE_USER_FUNC(evt_npc_set_rgba, 5) // evt_npc_set_rgba(char* npcName, int r, int g, int b, int a) //
s32 evt_disp_func_static(spm::evtmgr::EvtEntry* evtEntry, bool isFirstCall);
s32 evt_disp_func_rainbow(spm::evtmgr::EvtEntry* evtEntry, bool isFirstCall);
}

}
