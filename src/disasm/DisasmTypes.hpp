#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include <Zydis/Zydis.h>

struct DisasmLine
{
    uint64_t address;
    std::string bytes;
    std::string instruction;
    std::string comment;

    // Zydis
    ZydisDecodedInstruction decoded;
    std::vector<ZydisDecodedOperand> operands;
};
