#include "cpu.h"
#include <iostream>
#include <vector>
#include <sstream>

cpu::cpu()
{
}

void cpu::reset()
{
	A = 0;
	Y = 0;
	X = 0;
	SP = 0xFD;
	PC = 0;
	P = 0x24;
}


void cpu::setFlag(FLAGS flag, bool value)
{
	if (value) {
		P |= flag;
	}
	else {
		P &= ~flag;
	}
}

bool cpu::getFlag(FLAGS flag)
{
	return (P & flag) != 0;
}

void cpu::execute(uint16_t opcode)
{
	auto it = opcode_map.find(opcode);
	if (it != opcode_map.end()) {
		(this->*(it->second))();
	}
	else {
		std::cerr << "Unknown opcode: " << std::hex << int(opcode) << std::endl;
	}

}

void cpu::displayRegisters() {
	std::cout << "A: " << std::hex << +A << "  ";
	std::cout << "X: " << std::hex << +X << "  ";
	std::cout << "Y: " << std::hex << +Y << "  ";
	std::cout << "SP: " << std::hex << +SP << "  ";
	std::cout << "PC: " << std::hex << +PC << "  ";
	std::cout << "P: " << std::hex << +P << std::endl;
}

void cpu::LDA_Immediate()
{
	A = memory[PC + 1];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 2;
}

void cpu::LDA_ZeroPage()
{
	uint8_t address = memory[PC + 1];
	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 2;
}

void cpu::LDA_ZeroPageX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + X);
	A = memory[effectiveAddress];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 2;
}

void cpu::LDA_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 3;
}

void cpu::LDA_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + X;

	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 3;
}

void cpu::LDA_absoluteY()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + Y;

	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 3;
}

void cpu::LDA_indirectX()
{
	uint8_t zpAddr = static_cast<uint8_t>(memory[PC + 1] + X);  // wrap in zero page
	uint8_t low = memory[zpAddr];
	uint8_t high = memory[(uint8_t)(zpAddr + 1)];
	uint16_t address = (high << 8) | low;

	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 2;
}

void cpu::LDA_indirectY()
{
	uint8_t zpAddr = memory[PC + 1];
	uint8_t low = memory[zpAddr];
	uint8_t high = memory[(uint8_t)(zpAddr + 1)];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + Y;

	A = memory[address];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 2;
}

void cpu::LDX_Immediate()
{
	X = memory[PC + 1];
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 2;
}

void cpu::LDX_ZeroPage()
{
	uint8_t address = memory[PC + 1];
	X = memory[address];
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 2;
}

void cpu::LDX_ZeroPageY()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + Y);
	X = memory[effectiveAddress];
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 2;
}

void cpu::LDX_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	X = memory[address];
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 3;
}

void cpu::LDX_absoluteY()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + Y;

	X = memory[address];
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 3;
}


void cpu::LDY_Immediate()
{
	Y = memory[PC + 1];
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 2;
}

void cpu::LDY_ZeroPage()
{
	uint8_t address = memory[PC + 1];
	Y = memory[address];
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 2;
}

void cpu::LDY_ZeroPageX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + Y);
	Y = memory[effectiveAddress];
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 2;
}

void cpu::LDY_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	Y = memory[address];
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 3;
}

void cpu::LDY_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + X;

	Y = memory[address];
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 3;
}

void cpu::STA_zeropage()
{
	uint8_t address = memory[PC + 1];
	memory[address] = A;
	PC += 2;
}

void cpu::STA_zeropageX()
{
	uint8_t base = memory[PC + 1];
	uint8_t address = base + X;  // Wrap automatically via uint8_t
	memory[address] = A;
	PC += 2;
}

void cpu::STA_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	memory[address] = A;
	PC += 3;
}

void cpu::STA_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t base = (high << 8) | low;
	uint16_t address = base + X;

	memory[address] = A;
	PC += 3;
}


void cpu::STA_absoluteY()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t base = (high << 8) | low;
	uint16_t address = base + Y;

	memory[address] = A;
	PC += 3;
}

void cpu::STA_indirectX()
{
	uint8_t base = memory[PC + 1];
	uint8_t ptr = base + X;

	uint8_t low = memory[ptr];
	uint8_t high = memory[(uint8_t)(ptr + 1)];
	uint16_t address = (high << 8) | low;

	memory[address] = A;
	PC += 2;
}

void cpu::STA_indirectY()
{
	uint8_t base = memory[PC + 1];

	uint8_t low = memory[base];
	uint8_t high = memory[(uint8_t)(base + 1)];
	uint16_t address = ((high << 8) | low) + Y;

	memory[address] = A;
	PC += 2;
}

void cpu::STX_zeropage()
{
	uint8_t address = memory[PC + 1];
	memory[address] = X;
	PC += 2;
}

void cpu::STX_zeropageY()
{
	uint8_t base = memory[PC + 1];
	uint8_t address = base + Y; 
	memory[address] = X;
	PC += 2;
}

void cpu::STX_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	memory[address] = X;
	PC += 3;
}

void cpu::STY_zeropage()
{
	uint8_t address = memory[PC + 1];
	memory[address] = Y;
	PC += 2;
}

void cpu::STY_zeropageX()
{
	uint8_t base = memory[PC + 1];
	uint8_t ptr = base + X;

	uint8_t low = memory[ptr];
	uint8_t high = memory[(uint8_t)(ptr + 1)];
	uint16_t address = (high << 8) | low;

	memory[address] = Y;
	PC += 2;
}

void cpu::STY_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;

	memory[address] = Y;
	PC += 3;
}

void cpu::TAX()
{
	X = A;
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 1;
}

void cpu::TAY()
{
	Y = A;
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 1;

}

void cpu::TSX()
{
	X = SP;
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 1;
}

void cpu::TXA()
{
	A = X;
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 1;
}

void cpu::TXS()
{
	SP = X;
	PC += 1;
}

void cpu::TYA()
{
	A = Y;
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 1;
}



void cpu::INX()
{
	X += 1;
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 1;
}

void cpu::INY()
{
	Y += 1;
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 1;
}

void cpu::clc()
{
	setFlag(CARRY, false);
	PC += 1;
}

void cpu::ADC_imediate()
{
	uint8_t value = memory[PC + 1];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 2;
}

void cpu::ADC_zeroPage()
{
	uint8_t address = memory[PC + 1];
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 2;
}

void cpu::ADC_zeroPageX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + X);
	uint8_t value = memory[effectiveAddress];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 2;
}

void cpu::ADC_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 3;
}

void cpu::ADC_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + X;
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 3;
}

void cpu::ADC_absoluteY()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + Y;
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 3;
}

void cpu::ADC_indirectX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t zpAddr = static_cast<uint8_t>(baseAddress + X);  // wrap in zero page
	uint8_t low = memory[zpAddr];
	uint8_t high = memory[(uint8_t)(zpAddr + 1)];
	uint16_t address = (high << 8) | low;
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 2;
}

void cpu::ADC_indirectY()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t low = memory[baseAddress];
	uint8_t high = memory[(uint8_t)(baseAddress + 1)];
	uint16_t address = (high << 8) | low;
	uint8_t value = memory[address];
	uint16_t result = A + value + (getFlag(CARRY) ? 1 : 0);
	setFlag(CARRY, result > 0xFF);
	setFlag(ZERO, (result & 0xFF) == 0);
	setFlag(NEGATIVE, (result & 0x80) != 0);
	setFlag(OVERFLOW_, ((A ^ result) & (value ^ result) & 0x80) != 0);
	A = result & 0xFF;
	PC += 2;
}

void cpu::PHA()
{
	SP--;
	memory[0x0100 + SP] = A;
	PC += 1;
}

void cpu::PHP() {
	SP--;
	uint8_t statusToPush = P | 0x30; // Set bits 5 (unused) and 4 (break)
	memory[0x0100 + SP] = statusToPush;
	PC += 1;
}

void cpu::PLA()
{
	SP++;
	A = memory[0x0100 + SP];
	setFlag(ZERO, A == 0);
	setFlag(NEGATIVE, A & 0x80);
	PC += 1;
}

void cpu::PLP()
{
	SP++;
	P = memory[0x0100 + SP];
	PC += 1;
	setFlag(UNUSED, true); // Set unused bit to 1
	setFlag(BREAK, false); // Clear break flag
}

void cpu::DEC_zeropage()
{
	uint8_t address = memory[PC + 1];
	memory[address]--;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 2;
}

void cpu::DEC_zeropageX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + X);
	memory[effectiveAddress]--;
	setFlag(ZERO, memory[effectiveAddress] == 0);
	setFlag(NEGATIVE, memory[effectiveAddress] & 0x80);
	PC += 2;
}

void cpu::DEC_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;
	memory[address]--;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 3;
}

void cpu::DEC_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + X;
	memory[address]--;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 3;
}



void cpu::DEX()
{
	X--;
	setFlag(ZERO, X == 0);
	setFlag(NEGATIVE, X & 0x80);
	PC += 1;
}

void cpu::DEY()
{
	Y--;
	setFlag(ZERO, Y == 0);
	setFlag(NEGATIVE, Y & 0x80);
	PC += 1;
}

void cpu::INC_zeropage()
{
	uint8_t address = memory[PC + 1];
	memory[address]++;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 2;
}

void cpu::INC_zeropageX()
{
	uint8_t baseAddress = memory[PC + 1];
	uint8_t effectiveAddress = static_cast<uint8_t>(baseAddress + X);
	memory[effectiveAddress]++;
	setFlag(ZERO, memory[effectiveAddress] == 0);
	setFlag(NEGATIVE, memory[effectiveAddress] & 0x80);
	PC += 2;
}

void cpu::INC_absolute()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t address = (high << 8) | low;
	memory[address]++;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 3;
}

void cpu::INC_absoluteX()
{
	uint8_t low = memory[PC + 1];
	uint8_t high = memory[PC + 2];
	uint16_t baseAddress = (high << 8) | low;
	uint16_t address = baseAddress + X;
	memory[address]++;
	setFlag(ZERO, memory[address] == 0);
	setFlag(NEGATIVE, memory[address] & 0x80);
	PC += 3;
}


uint16_t cpu::getPC()
{
	return PC;
}

void cpu::loadMemory(const std::vector<uint8_t>& program)
{
	memory[0x20] = 100;
	PC = 0x0600;  // Explicitly set start address
	for (size_t i = 0; i < program.size(); ++i) {
		memory[PC + i] = program[i];
	}

	std::cout << "Debug memory: ";
	for (size_t i = 0; i < program.size(); ++i) {
		std::cout << std::hex << +memory[PC + i] << " ";
	}
	std::cout << std::endl;
}




void cpu::run(int steps, bool stepMode)
{
	for (int step = 0; step < steps; ++step) {
		if (!stepMode) {
			std::cout << "PC: " << std::hex << getPC() << std::endl;
			uint8_t opcode = memory[getPC()];
			std::cout << "Opcode: " << std::hex << +opcode << std::endl;
			execute(opcode);
		}
		else {
			std::string input;
			std::cout << "\n[Press Enter to step or type command]: ";
			std::getline(std::cin, input);

			if (input.rfind("mem", 0) == 0) {  // starts with "mem"
				std::istringstream ss(input);
				std::string cmd;
				uint16_t start = 0, end = 0;

				ss >> cmd >> std::hex >> start;
				if (!(ss >> std::hex >> end)) {
					end = start; // If only one address is given
				}

				std::cout << "Memory from $" << std::hex << start << " to $" << end << ":\n";
				for (uint16_t addr = start; addr <= end; ++addr) {
					std::cout << std::hex << "[$" << addr << "] = $" << +memory[addr] << "\n";
				}

				--step;  // Don't advance instruction if just checking memory
				continue;
			}

			std::cout << "PC: " << std::hex << getPC() << std::endl;
			uint8_t opcode = memory[getPC()];
			std::cout << "Opcode: " << std::hex << +opcode << std::endl;
			execute(opcode);
			displayRegisters();
		}
	}

	if (!stepMode) {
		displayRegisters();
	}
}


void cpu::loadFile(const std::string& filename) {
	testAssembler.loadFile(filename);
	testAssembler.assembleProgram();
	loadMemory(testAssembler.getAssembledProgram());
}

int cpu::getProgramSize()
{
	int programsize = testAssembler.programSize;
	return programsize;
}


