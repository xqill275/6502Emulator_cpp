#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <functional>
#include <algorithm>

class assembler
{
public:
    enum class AddressingMode {
        Immediate,
        ZeroPage,
        ZeroPageX,
        ZeroPageY,
        Absolute,
        AbsoluteX,
        AbsoluteY,
        IndirectX,
        IndirectY,
        Unknown
    };

    assembler();

    void loadFile(const std::string& filename);
    void assembleProgram();
    std::vector<uint8_t> getAssembledProgram() const;
    int programSize;

private:
    std::vector<uint8_t> assembledProgram;
    std::vector<std::string> inputProgram;

    using InstructionHandler = std::function<void(size_t&)>;
    std::map<std::string, InstructionHandler> instructionHandlers;

    std::map<AddressingMode, uint8_t> ldaOpcodes;
    std::map<AddressingMode, uint8_t> ldyOpcodes;
    std::map<AddressingMode, uint8_t> ldxOpcodes;
    std::map<AddressingMode, uint8_t> staOpcodes;
    std::map<AddressingMode, uint8_t> stxOpcodes;
    std::map<AddressingMode, uint8_t> styOpcodes;

    AddressingMode detectAddressingMode(const std::string& operand);
    uint16_t parseOperand(const std::string& operand);
    void assembleInstructionWithOperand(const std::string& operand, const std::map<AddressingMode, uint8_t>& opcodeMap);

    InstructionHandler makeGenericHandler(const std::map<AddressingMode, uint8_t>& opcodeMap);
};