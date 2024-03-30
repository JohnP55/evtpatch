#pragma once

#include "evt_cmd.h"

namespace spm::evt_fade {

extern "C" {

// evt_fade_end_wait(int)
EVT_DECLARE_USER_FUNC(evt_fade_end_wait, 1)
EVT_DECLARE_USER_FUNC(evt_set_transition, 2)
EVT_DECLARE_USER_FUNC(evt_seq_wait, 1)
}

}
