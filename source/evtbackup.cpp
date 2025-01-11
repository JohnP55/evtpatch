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

static Stack<Stack<EvtVar>*>* localWorkStack[EVT_ENTRY_MAX];

void backupLocalWork(s32 index, EvtVar value, EvtEntry* evtEntry) {
    s32 entryIndex = getEvtEntryIdx(evtEntry);

    // Check if a top-level stack exists
    if (localWorkStack[entryIndex] == nullptr) {
        localWorkStack[entryIndex] = new Stack<Stack<EvtVar>*>();
    }

    Stack<Stack<EvtVar>*>* topStack = localWorkStack[entryIndex];

    // Check if a sub-stack exists
    if (topStack->isEmpty() || topStack->peek() == nullptr) {
        topStack->push(new Stack<EvtVar>());
    }

    // Push the value into the top sub-stack
    Stack<EvtVar>* subStack = topStack->peek();
    subStack->push(spm::evtmgr_cmd::evtGetValue(evtEntry, value));
}

EvtVar restoreLocalWork(s32 index, EvtEntry* evtEntry) {
    s32 entryIndex = getEvtEntryIdx(evtEntry);
    Stack<Stack<EvtVar>*>* topStack = localWorkStack[entryIndex];

    Stack<EvtVar>* subStack = topStack->peek();

    EvtVar ret = subStack->pop();

    // If the either stack becomes empty, delete it
    if (subStack->isEmpty()) {
        delete subStack;
        topStack->pop();
    }

    if (topStack->isEmpty()) {
        delete topStack;
        localWorkStack[entryIndex] = nullptr;
    }

    return ret;
}


void destroyStack(s32 index) {
    Stack<Stack<EvtVar>*>* topStack = localWorkStack[index];
    if (topStack != nullptr) {
        while (!topStack->isEmpty()) {
            Stack<EvtVar>* subStack = topStack->pop();
            if (subStack != nullptr) {
                subStack->clear();
                delete subStack;
            }
        }
        delete topStack;
        localWorkStack[index] = nullptr;
    }
}


s32 evt_backupLocalWork(EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar * args = (EvtVar * ) evtEntry -> pCurData;
  backupLocalWork(args[0], args[1], evtEntry);
  return 2;
}

s32 evt_backupLocalWorkAll(EvtEntry * evtEntry, bool firstRun) {
  for (s32 i = 0; i < 16; i++) {
  backupLocalWork(i, evtEntry->lw[i], evtEntry);
}
  return 2;
}

s32 evt_restoreLocalWork(EvtEntry * evtEntry, bool firstRun) {
  spm::evtmgr::EvtVar * args = (EvtVar * ) evtEntry -> pCurData;
  spm::evtmgr_cmd::evtSetValue(evtEntry, args[0], restoreLocalWork(args[1], evtEntry));
  return 2;
}

s32 evt_restoreLocalWorkAll(EvtEntry * evtEntry, bool firstRun) {
  for (s32 i = 0; i < 16; i++) {
  spm::evtmgr_cmd::evtSetValue(evtEntry, evtEntry->lw[i], restoreLocalWork(i, evtEntry));
}
  return 2;
}

}
