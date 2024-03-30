#pragma once

#include <types.h>
#include <spm/evtmgr.h>

namespace spm::evt_snd {

extern "C" {

// evt_snd_bgmon(int BGMplayer (0 is normal, others unknown), const char * name)
EVT_DECLARE_USER_FUNC(evt_snd_bgmon, 2)
// evt_snd_bgmoff(int BGMplayer)
EVT_DECLARE_USER_FUNC(evt_snd_bgmoff, 1)
// evt_snd_bgmoff(int BGMplayer, int fadeOut)
EVT_DECLARE_USER_FUNC(evt_snd_bgmoff_f, 2)

// SFX
EVT_DECLARE_USER_FUNC(evt_snd_sfxon, 1) // evt_snd_sfxon(const char * SFXname), plays everywhere at equal volume
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_3d, 4) // evt_snd_sfxon_3d(const char * SFXname, float x, float y, float z), plays at x,y,z
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_player, 1) // evt_snd_sfxon(const char * SFXname) plays from the players position
EVT_DECLARE_USER_FUNC(evt_snd_sfxon_npc, 2) // evt_snd_sfxon_npc(const char * SFXname, const char* instanceName), plays from the npc's position
EVT_DECLARE_USER_FUNC(evt_snd_sfxoff, 1) // evt_snd_sfxoff(SFXid)
EVT_DECLARE_USER_FUNC(evt_snd_sfx_wait_name, 1) // evt_snd_sfx_wait_name(const char * SFXname)
EVT_DECLARE_USER_FUNC(evt_snd_get_last_sfx_id, 1) // evt_snd_get_last_sfx_id(variable), outputs the last played SFX id to the variable
EVT_DECLARE_USER_FUNC(func_800d36f4, 1)

}

}
