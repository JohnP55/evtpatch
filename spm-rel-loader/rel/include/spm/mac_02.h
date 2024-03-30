#pragma once

namespace spm::mac_02 {

extern "C" {

// functions here
EVT_DECLARE_USER_FUNC(evt_mac_merlon_door_npcs_override, 1)
EVT_DECLARE_USER_FUNC(mac_02_init_evt, 0)

extern spm::evt_door::DoorDesc merlonDoorDesc;

}

}