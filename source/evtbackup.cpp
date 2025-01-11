#include "evtbackup.h"
#include "evtpatch.h"
#include <msl/string.h>
#include <spm/evtmgr_cmd.h>
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/os.h>

namespace mod::evtbackup {

using namespace spm::evtmgr;
using namespace mod::evtpatch;

static Stack<EvtVar>* localWorkStack[EVT_ENTRY_MAX][16];

void backupLocalWork(s32 index, EvtVar value, EvtEntry * evtEntry) {
  s32 entryIndex = getEvtEntryIdx(evtEntry);
  Stack<EvtVar>* stack = localWorkStack[entryIndex][index];
  if (stack == nullptr) {
      localWorkStack[entryIndex][index] = new Stack<EvtVar>();
      stack = localWorkStack[entryIndex][index];
  }
  stack->push(spm::evtmgr_cmd::evtGetValue(evtEntry, value));
}

EvtVar restoreLocalWork(s32 index, EvtEntry * evtEntry) {
  s32 entryIndex = getEvtEntryIdx(evtEntry);
  Stack<EvtVar>* stack = localWorkStack[entryIndex][index];
  EvtVar ret = stack->pop();
  if (stack->isEmpty()) {
    stack->clear();
    delete stack;
    localWorkStack[entryIndex][index] = nullptr;
  }
  return ret;
}

void destroyStack(s32 index) {
  for (s32 i = 0; i < 16; i++) {
      if (localWorkStack[index][i] != nullptr) {
        Stack<EvtVar>* stack = localWorkStack[index][i];
        stack->clear();
        delete stack;
        localWorkStack[index][i] = nullptr;
      }
  }
}

s32 evt_backupLocalWork(EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar * args = (EvtVar * ) evtEntry -> pCurData;
  backupLocalWork(args[0], args[1], evtEntry);
  return 2;
}

s32 evt_restoreLocalWork(EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar * args = (EvtVar * ) evtEntry -> pCurData;
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], restoreLocalWork(args[1], evtEntry));
  return 2;
}

}
