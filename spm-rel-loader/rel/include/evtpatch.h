#pragma once

#include <evt_cmd.h>

namespace mod::evtpatch {

/// @brief Gets the number of arguments for a given instruction in an evt script
/// @param instruction The instruction word to check
/// @return The number of arguments in the instruction
inline s32 getNumArgs(spm::evtmgr::EvtScriptCode instruction) {
    return instruction >> 16;
}
/// @brief Gets the opcode for a given instruction in an evt script
/// @param instruction The instruction word to check
/// @return The opcode of the instruction.
inline s32 getOpcode(spm::evtmgr::EvtScriptCode instruction) {
    return instruction & 0xffff;
}
/// @brief Checks if a given EvtScriptCode is likely the start of an instruction (Not 100% guaranteed)
/// @param instruction The instruction word to validate
/// @return Whether it seems to be a valid start of instruction
inline bool isStartOfInstruction(spm::evtmgr::EvtScriptCode instruction) {
    return getNumArgs(instruction) < 0x100 && getOpcode(instruction) < EVT_max; // if you see an evt opcode with more than 256 arguments lmk :)
}
/// @brief Gets the size of an instruction, in EvtScriptCodes (int32s)
/// @param instruction A pointer to the instruction
/// @return The size of the instruction, in EvtScriptCodes (int32s)
inline s32 getInstructionLength(spm::evtmgr::EvtScriptCode* instruction) {
    return (1 + getNumArgs(instruction));
}
/// @brief Gets the size of an instruction, in bytes
/// @param instruction A pointer to the instruction
/// @return The size of the instruction, in bytes
inline s32 getInstructionSize(spm::evtmgr::EvtScriptCode* instruction) {
    return getInstructionLength(instruction) * sizeof(spm::evtmgr::EvtScriptCode);
}

s32 getLineOffset(spm::evtmgr::EvtScriptCode* script, s32 line);
void insertTrampoline(spm::evtmgr::EvtScriptCode* ptr, spm::evtmgr::EvtScriptCode* scriptAddr);

void hookEvt(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* dst);
void hookEvtByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* dst);
void hookEvtReplace(spm::evtmgr::EvtScriptCode* script, s32 line, spm::evtmgr::EvtScriptCode* dst);
void hookEvtReplaceByOffset(spm::evtmgr::EvtScriptCode* script, s32 offset, spm::evtmgr::EvtScriptCode* dst);
void hookEvtReplaceBlock(spm::evtmgr::EvtScriptCode* script, s32 lineStart, spm::evtmgr::EvtScriptCode* dst, s32 lineEnd);
void hookEvtReplaceBlockByOffset(spm::evtmgr::EvtScriptCode* script, s32 offsetStart, spm::evtmgr::EvtScriptCode* dst, s32 offsetEnd);

}
