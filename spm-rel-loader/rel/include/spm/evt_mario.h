#pragma once

#include "evt_cmd.h"

namespace spm::evt_mario {

extern "C" {

// evt_mario_flag8_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_mario_flag8_onoff, 2)

// evt_mario_key_on()
EVT_DECLARE_USER_FUNC(evt_mario_key_on, 0)

// evt_mario_key_off(int)
EVT_DECLARE_USER_FUNC(evt_mario_key_off, 1)

// evt_mario_get_pos(LW_x, LW_y, LW_z)
EVT_DECLARE_USER_FUNC(evt_mario_get_pos, 3)
// evt_mario_set_pose(char * animname, int time)
EVT_DECLARE_USER_FUNC(evt_mario_set_pose, 2)
// evt_mario_face_dir(float facingAngle, int unk1=200)
EVT_DECLARE_USER_FUNC(evt_mario_face_dir, 2)
// evt_mario_set_scale(float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mario_set_scale, 3)
// evt_mario_set_pos(float x, float y, float z)
EVT_DECLARE_USER_FUNC(evt_mario_set_pos, 3)

// evt_mario_flag0_onoff(bool onoff, u32 flags)
EVT_DECLARE_USER_FUNC(evt_mario_flag0_onoff, 2)
}

}