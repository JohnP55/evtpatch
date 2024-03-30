#include "evtpatch.h"
#include "evtopcodes.h"
#include <spm/memory.h>
#include <spm/system.h>
#include <wii/string.h>

namespace mod::evtpatch {

using namespace spm::evtmgr;


const EvtScriptCode trampoline[] = { RUN_CHILD_EVT(0) };
const EvtScriptCode scriptEnd[] = { Return, EvtEnd };

#define TRAMPOLINE_EVT_OFFSET_BYTES 0x4 // first instruction, first argument
#define TRAMPOLINE_EVT_OFFSET TRAMPOLINE_EVT_OFFSET_BYTES / sizeof(EvtScriptCode) // first instruction, first argument
#define TRAMPOLINE_SIZE_BYTES sizeof(trampoline)
#define TRAMPOLINE_SIZE TRAMPOLINE_SIZE_BYTES / sizeof(EvtScriptCode)
#define SCRIPT_END_SIZE_BYTES sizeof(scriptEnd)
#define SCRIPT_END_SIZE SCRIPT_END_SIZE_BYTES / sizeof(EvtScriptCode)

/// @brief Gets the offset in bytes of a line from the start of an evt script
/// @param script The evt script to look through
/// @param line The line number to find the offset of
/// @return The offset of the line, in bytes, from the start of the script
s32 getLineOffset(EvtScriptCode* script, s32 line) {
    EvtScriptCode* instruction = script;
    s32 offset = 0;
    while (true) {
        s32 size = getSizeOfInstruction(instruction);
        offset += size;
        instruction = script + offset;
    }
    return offset;
}

/// @brief Writes a RUN_CHILD_EVT instruction with a specified evt script at a specified pointer
/// @param ptr The place to write the instruction at
/// @param script The script that will be the child evt
void insertTrampoline(EvtScriptCode* ptr, EvtScriptCode* script) {
    wii::string::memcpy(ptr, trampoline, TRAMPOLINE_SIZE_BYTES);
    ptr[TRAMPOLINE_EVT_OFFSET] = reinterpret_cast<s32>(script);
}


#pragma region ConstOverrideOverloads
// // basically the rel loader framework expects scripts to be const, but the premise of hooking into them kinda goes against that
// // therefore these exist
// /// @brief Adds a hook to another evt script, automatically restoring original instructions overwritten by the hook
// /// @param script The evt script that will be hooked into
// /// @param line The line number to hook at
// /// @param dst The evt script that will be executed
// void hookEvt(const EvtScriptCode* script, s32 offset, const EvtScriptCode* dst) {
//     hookEvt((EvtScriptCode*) script, offset, (EvtScriptCode*) dst);
// }
// /// @brief Adds a hook to another evt script, without restoring original instructions
// /// @param script The evt script that will be hooked into
// /// @param line The line number to hook at
// /// @param dst The evt script that will be executed
// void hookEvtReplace(const EvtScriptCode* script, s32 offset, const EvtScriptCode* dst) {
//     hookEvtReplace((EvtScriptCode*) script, offset, (EvtScriptCode*) dst);
// }
// /// @brief Adds a hook to another evt script, without restoring original instructions and comes back at a specified offset
// /// @param script The evt script that will be hooked into
// /// @param lineStart The line number to hook at
// /// @param dst The evt script that will be executed
// /// @param lineEnd The line number to come back to once `dst` is done executing.
// void hookEvtReplaceBlock(const EvtScriptCode* script, s32 lineStart, const EvtScriptCode* dst, s32 lineEnd) {
//     hookEvtReplaceBlock((EvtScriptCode*) script, lineStart, (EvtScriptCode*) dst, lineEnd);
// }
#pragma endregion


/// @brief Adds a hook to another evt script, automatically restoring original instructions overwritten by the hook
/// @param script The evt script that will be hooked into
/// @param line The line number to hook at
/// @param dst The evt script that will be executed
void hookEvt(EvtScriptCode* script, s32 line, EvtScriptCode* dst) {
    hookEvtByOffset(script, getLineOffset(line), dst);
}
/// @brief Adds a hook to another evt script, automatically restoring original instructions overwritten by the hook
/// @param script The evt script that will be hooked into
/// @param offset The offset to hook at, in bytes, from the start of the script
/// @param dst The evt script that will be executed
void hookEvtByOffset(EvtScriptCode* script, s32 offset, EvtScriptCode* dst) {
    EvtScriptCode* src = script + offset;
    assert(isStartOfInstruction(*src), "Cannot hook on non-instruction (what are you doing :sob:)");
    
    u32 lenOriginalInstructions = 0;
    s32 curInstLen;
    do {
        curInstLen = getNumArgs(src[lenOriginalInstructions]);
        lenOriginalInstructions += (1 + curInstLen);
    } while (lenOriginalInstructions < TRAMPOLINE_SIZE);

    EvtScriptCode* dynamicEvtForwarder = new EvtScriptCode[TRAMPOLINE_SIZE + lenOriginalInstructions + SCRIPT_END_SIZE];
    insertTrampoline(dynamicEvtForwarder, dst);
    wii::string::memcpy(dynamicEvtForwarder + TRAMPOLINE_SIZE, src, lenOriginalInstructions * sizeof(EvtScriptCode)); // add original instructions
    wii::string::memcpy(dynamicEvtForwarder + TRAMPOLINE_SIZE + lenOriginalInstructions, scriptEnd, lenOriginalInstructions * sizeof(EvtScriptCode)); // add epilog
    insertTrampoline(src, dynamicEvtForwarder);
}


/// @brief Adds a hook to another evt script, without restoring original instructions
/// @param script The evt script that will be hooked into
/// @param line The line number to hook at
/// @param dst The evt script that will be executed
void hookEvtReplace(EvtScriptCode* script, s32 line, EvtScriptCode* dst) {
    hookEvtReplaceByOffset(script, getLineOffset(line), dst);
}

/// @brief Adds a hook to another evt script, without restoring original instructions
/// @param script The evt script that will be hooked into
/// @param offset The offset to hook at, in bytes, from the start of the script
/// @param dst The evt script that will be executed
void hookEvtReplaceByOffset(EvtScriptCode* script, s32 offset, EvtScriptCode* dst) {
    EvtScriptCode* src = script + offset;
    assert(isStartOfInstruction(*src), "Cannot hook on non-instruction (what are you doing :sob:)");
    
    u32 lenOriginalInstructions = 0;
    s32 curInstLen;
    do {
        curInstLen = getNumArgs(src[lenOriginalInstructions]);
        lenOriginalInstructions += (1 + curInstLen);
    } while (lenOriginalInstructions < TRAMPOLINE_SIZE);
    
    wii::string::memset(src, 0, lenOriginalInstructions); // pad anything left with 0s
    insertTrampoline(src, dst);
}


/// @brief Adds a hook to another evt script, without restoring original instructions and comes back at a specified offset
/// @param script The evt script that will be hooked into
/// @param lineStart The line number to hook at
/// @param dst The evt script that will be executed
/// @param lineEnd The line number to come back to once `dst` is done executing.
void hookEvtReplaceBlock(EvtScriptCode* script, s32 lineStart, EvtScriptCode* dst, s32 lineEnd) {
    hookEvtReplaceBlockByOffset(script, getLineOffset(script, lineStart), dst, getLineOffset(script, lineEnd));
}

/// @brief Adds a hook to another evt script, without restoring original instructions and comes back at a specified offset
/// @param script The evt script that will be hooked into
/// @param offsetStart The offset to hook at, in bytes, from the start of the script
/// @param dst The evt script that will be executed
/// @param offsetEnd The offset to come back to once `dst` is done executing, in bytes, from the start of the script
void hookEvtReplaceBlockByOffset(EvtScriptCode* script, s32 offsetStart, EvtScriptCode* dst, s32 offsetEnd) {
    EvtScriptCode* src = script + offsetStart;
    s32 length = offsetEnd-offsetStart;
    assert(isStartOfInstruction(*src), "Cannot hook on non-instruction (what are you doing :sob:)");

    insertTrampoline(src, length);
}

}
