#pragma once

#include <evt_cmd.h>
#include "stack.hh"

namespace mod::evtbackup {
  using namespace spm::evtmgr;
  void backupLocalWork(s32 index, EvtVar value, EvtEntry * evtEntry);
  EvtVar restoreLocalWork(s32 index, EvtEntry * evtEntry);
  void destroyStack(s32 index);

  //note: index is equal to LW value (i.e the index for LW(0) would be zero)

  // evt_backupLocalWork(s32 index, EvtVar value)
  EVT_DECLARE_USER_FUNC(evt_backupLocalWork, 2)

  // evt_restoreLocalWork(s32 index, EvtVar value)
  EVT_DECLARE_USER_FUNC(evt_restoreLocalWork, 2)

    // backs up all local work to the stack
    EVT_DECLARE_USER_FUNC(evt_backupLocalWorkAll, 0)

    // restores all local work from the stack
    EVT_DECLARE_USER_FUNC(evt_restoreLocalWorkAll, 0)

}
