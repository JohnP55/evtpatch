#pragma once

#include "evt_cmd.h"

#include <types.h>

namespace spm::evt_cam {
    extern "C" {
        EVT_DECLARE_USER_FUNC(evt_cam3d_evt_zoom_in, 9) // evt_cam3d_evt_zoom_in(int isOrtho, float zoomDestPosX, float zoomDestPosY, float zoomDestPosZ, float zoomDestTargetX, float zoomDestTargetY, float zoomDestTargetZ, int unk5, int unk6 )
        EVT_DECLARE_USER_FUNC(evt_reset_camera, 2)
    }
}