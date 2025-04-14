#include "assembler.h"
#include <iostream>
#include <sstream>

assembler::assembler() {
    // LDA opcode map
    ldaOpcodes = {
        {AddressingMode::Immediate,  0xA9},
        {AddressingMode::ZeroPage,   0xA5},
        {AddressingMode::ZeroPageX,  0xB5},
        {AddressingMode::Absolute,   0xAD},
        {AddressingMode::AbsoluteX,  0xBD},
        {AddressingMode::AbsoluteY,  0xB9},
        {AddressingMode::IndirectX,  0xA1},
        {AddressingMode::IndirectY,  0xB1}
    };

    // LDY Opcodes
    ldyOpcodes = {
        {AddressingMode::Immediate, 0xA0},  // #Immediate
        {AddressingMode::ZeroPage, 0xA4},   // Zero Page
        {AddressingMode::ZeroPageX, 0xB4},  // Zero Page,X
        {AddressingMode::Absolute, 0xAC},   // Absolute
        {AddressingMode::AbsoluteX, 0xBC}   // Absolute,X
    };

    // LDX Opcodes
    ldxOpcodes = {
        {AddressingMode::Immediate, 0xA2},  // #Immediate
        {AddressingMode::ZeroPage, 0xA6},   // Zero Page
        {AddressingMode::ZeroPageY, 0xB6},  // Zero Page,Y
        {AddressingMode::Absolute, 0xAE},   // Absolute
        {AddressingMode::AbsoluteY, 0xBE}   // Absolute,Y
    };

    staOpcodes = {
        {AddressingMode::ZeroPage,  0x85},
        {AddressingMode::ZeroPageX,   0x95},
        {AddressingMode::Absolute,  0x8D},
        {AddressingMode::AbsoluteX,   0x9D},
        {AddressingMode::AbsoluteY,  0x99},
        {AddressingMode::IndirectX,  0x81},
        {AddressingMode::IndirectY,  0x91}
    };

    stxOpcodes = {
        {AddressingMode::ZeroPage,  0x86},
        {AddressingMode::ZeroPageY,   0x96},
        {AddressingMode::Absolute,  0x8E}
    };

    styOpcodes = {
    {AddressingMode::ZeroPage,  0x84},
    {AddressingMode::ZeroPageX,   0x94},
    {AddressingMode::Absolute,  0x8c}
    };

	// stack instructions
	decOpcodes = {
        {AddressingMode::ZeroPage, 0xC6 },
	    {AddressingMode::ZeroPageX, 0xD6 },
	    {AddressingMode::Absolute, 0xCE },
	    {AddressingMode::AbsoluteX, 0xDE }
    };

	incOpcodes = {
		{AddressingMode::ZeroPage, 0xE6 },
		{AddressingMode::ZeroPageX, 0xF6 },
		{AddressingMode::Absolute, 0xEE },
		{AddressingMode::AbsoluteX, 0xFE }
	};


    // Register instruction handlers
    instructionHandlers["LDA"] = makeGenericHandler(ldaOpcodes);
    instructionHandlers["LDY"] = makeGenericHandler(ldyOpcodes);
    instructionHandlers["LDX"] = makeGenericHandler(ldxOpcodes);
    instructionHandlers["STA"] = makeGenericHandler(staOpcodes);
    instructionHandlers["STX"] = makeGenericHandler(stxOpcodes);
    instructionHandlers["STY"] = makeGenericHandler(styOpcodes);
	instructionHandlers["DEC"] = makeGenericHandler(decOpcodes);
	instructionHandlers["INC"] = makeGenericHandler(incOpcodes);



	instructionHandlers["TAX"] = makeInstructionHandlerForSingleByte(0xAA);
	instructionHandlers["TAY"] = makeInstructionHandlerForSingleByte(0xA8);
	instructionHandlers["TSX"] = makeInstructionHandlerForSingleByte(0xBA);
	instructionHandlers["TXA"] = makeInstructionHandlerForSingleByte(0x8A);
	instructionHandlers["TXS"] = makeInstructionHandlerForSingleByte(0x9A);
	instructionHandlers["TYA"] = makeInstructionHandlerForSingleByte(0x98);

	// stack instructions
	instructionHandlers["PHA"] = makeInstructionHandlerForSingleByte(0x48);
	instructionHandlers["PHP"] = makeInstructionHandlerForSingleByte(0x08);
	instructionHandlers["PLA"] = makeInstructionHandlerForSingleByte(0x68);
	instructionHandlers["PLP"] = makeInstructionHandlerForSingleByte(0x28);

	// Decrement/Increment instructions
	instructionHandlers["INX"] = makeInstructionHandlerForSingleByte(0xE8);
	instructionHandlers["INY"] = makeInstructionHandlerForSingleByte(0xC8);
	instructionHandlers["DEX"] = makeInstructionHandlerForSingleByte(0xCA);
	instructionHandlers["DEY"] = makeInstructionHandlerForSingleByte(0x88);

    programSize = 0;
}

void assembler::loadFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string word;

    while (file >> word) {
        inputProgram.push_back(word);
    }
}

void assembler::assembleProgram() {
    for (size_t i = 0; i < inputProgram.size(); ++i) {
        std::string instruction = inputProgram[i];
        if (instructionHandlers.find(instruction) != instructionHandlers.end()) {
            instructionHandlers[instruction](i);
        }
        else {
            std::cerr << "Unknown instruction: " << instruction << std::endl;
        }
    }
}

std::vector<uint8_t> assembler::getAssembledProgram() const {
    return assembledProgram;
}

assembler::AddressingMode assembler::detectAddressingMode(const std::string& operand) {
    if (operand[0] == '#') return AddressingMode::Immediate;
    if (operand.substr(0, 2) == "($" && operand.find(",X") != std::string::npos) return AddressingMode::IndirectX;
    if (operand.find("),Y") != std::string::npos) return AddressingMode::IndirectY;
    if (operand.find(",X") != std::string::npos) {
        return operand.size() <= 6 ? AddressingMode::ZeroPageX : AddressingMode::AbsoluteX;
    }
    if (operand.find(",Y") != std::string::npos) {
        return operand.size() <= 6 ? AddressingMode::ZeroPageY : AddressingMode::AbsoluteY;
    }
    if (operand.size() <= 4 && operand.find(',') == std::string::npos) return AddressingMode::ZeroPage;
    if (operand.size() > 4) return AddressingMode::Absolute;
    return AddressingMode::Unknown;
}

uint16_t assembler::parseOperand(const std::string& operand) {
    std::string clean = operand;
    clean.erase(std::remove_if(clean.begin(), clean.end(), [](char c) {
        return c == '#' || c == '$' || c == '(' || c == ')' || c == ',' || c == 'X' || c == 'Y';
        }), clean.end());

    return static_cast<uint16_t>(std::stoi(clean, nullptr, 16));
}

void assembler::assembleInstructionWithOperand(const std::string& operand, const std::map<AddressingMode, uint8_t>& opcodeMap) {
    AddressingMode mode = detectAddressingMode(operand);
    if (opcodeMap.find(mode) == opcodeMap.end()) {
        std::cerr << "Unsupported addressing mode: " << operand << std::endl;
        return;
    }

    uint8_t opcode = opcodeMap.at(mode);
    assembledProgram.push_back(opcode);
    programSize += 1;

    uint16_t value = parseOperand(operand);
    if (mode == AddressingMode::Absolute || mode == AddressingMode::AbsoluteX || mode == AddressingMode::AbsoluteY) {
        assembledProgram.push_back(value & 0xFF);
        assembledProgram.push_back((value >> 8) & 0xFF);
    }
    else {
        assembledProgram.push_back(static_cast<uint8_t>(value));
    }
}

assembler::InstructionHandler assembler::makeGenericHandler(const std::map<AddressingMode, uint8_t>& opcodeMap) {
    return [=](size_t& i) {
        if (i + 1 >= inputProgram.size()) {
            std::cerr << "Missing operand after instruction" << std::endl;
            return;
        }
        std::string operand = inputProgram[++i];
        assembleInstructionWithOperand(operand, opcodeMap);
        };
}

// makeInstructionHandlerForSingleByte
assembler::InstructionHandler assembler::makeInstructionHandlerForSingleByte(uint8_t opcode) {
	return [=](size_t& i) {
		assembledProgram.push_back(opcode);
		programSize += 1;
		};
}