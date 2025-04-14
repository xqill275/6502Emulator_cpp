#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include "assembler.h"

class cpu
{
public:
	cpu();
	assembler testAssembler;
	void reset();
	void execute(uint16_t opcode);
	void displayRegisters();

	std::array<uint8_t, 0x10000> memory = {};



	uint8_t A = 0; // accumulator
	uint8_t Y = 0; // Y register
	uint8_t X = 0; // X register 
	uint8_t SP = 0xFD; // stack pointer
	uint16_t PC = 0; // program counter
	uint8_t P = 0x24; // proccessor status

	// flags bit positionted in P
	enum FLAGS {
		CARRY = (1 << 0),
		ZERO = (1 << 1),
		INTERRUPT = (1 << 2),
		DECIMAL = (1 << 3),
		BREAK = (1 << 4),
		UNUSED = (1 << 5),
		OVERFLOW_ = (1 << 6),
		NEGATIVE = (1 << 7)
	};


	void setFlag(FLAGS flag, bool value);
	bool getFlag(FLAGS flag);

	uint16_t getPC();

	// load memory:
	void loadMemory(const std::vector<uint8_t>& program);


	// run the cpu
	void run(int steps, bool stepMode);
	void loadFile(const std::string& filename);
	int getProgramSize();
private:



	std::map<uint8_t, void(cpu::*)()> opcode_map = {
		// --- LDA ---
		{0xA9, &cpu::LDA_Immediate},   // Immediate
		{0xA5, &cpu::LDA_ZeroPage},    // Zero Page
		{0xB5, &cpu::LDA_ZeroPageX},   // Zero Page,X
		{0xAD, &cpu::LDA_absolute},    // Absolute
		{0xBD, &cpu::LDA_absoluteX},   // Absolute,X
		{0xB9, &cpu::LDA_absoluteY},   // Absolute,Y
		{0xA1, &cpu::LDA_indirectX},   // (Indirect,X)
		{0xB1, &cpu::LDA_indirectY},   // (Indirect),Y

		// --- LDX ---
		{0xA2, &cpu::LDX_Immediate},   // Immediate
		{0xA6, &cpu::LDX_ZeroPage},    // Zero Page
		{0xB6, &cpu::LDX_ZeroPageY},   // Zero Page,Y
		{0xAE, &cpu::LDX_absolute},    // Absolute
		{0xBE, &cpu::LDX_absoluteY},   // Absolute,Y

		// --- LDY ---
		{0xA0, &cpu::LDY_Immediate},   // Immediate
		{0xA4, &cpu::LDY_ZeroPage},    // Zero Page
		{0xB4, &cpu::LDY_ZeroPageX},   // Zero Page,X
		{0xAC, &cpu::LDY_absolute},    // Absolute
		{0xBC, &cpu::LDY_absoluteX},   // Absolute,X

		// --- STA ---
		{0x85, &cpu::STA_zeropage},    // Zero Page
		{0x95, &cpu::STA_zeropageX},   // Zero Page,X
		{0x8D, &cpu::STA_absolute},    // Absolute
		{0x9D, &cpu::STA_absoluteX},   // Absolute,X
		{0x99, &cpu::STA_absoluteY},   // Absolute,Y
		{0x81, &cpu::STA_indirectX},   // (Indirect,X)
		{0x91, &cpu::STA_indirectY},   // (Indirect),Y

		// --- STX ---
		{0x86, &cpu::STX_zeropage},    // Zero Page
		{0x96, &cpu::STX_zeropageY},   // Zero Page,Y
		{0x8E, &cpu::STX_absolute},    // Absolute

		// --- STY ---
		{0x84, &cpu::STY_zeropage},    // Zero Page
		{0x94, &cpu::STY_zeropageX},   // Zero Page,X
		{0x8C, &cpu::STY_absolute},    // Absolute

		// --- stack instructions ---
		{0x48, &cpu::PHA},             // Push A
		{0x08, &cpu::PHP},             // Push P
		{0x68, &cpu::PLA},             // Pull A
		{0x28, &cpu::PLP},             // Pull P

		// --- Transfer Instructions ---
		{0xAA, &cpu::TAX},             // Transfer A -> X
		{0xA8, &cpu::TAY},             // Transfer A -> Y
		{0xBA, &cpu::TSX},             // Transfer SP -> X
		{0x8A, &cpu::TXA},             // Transfer X -> A
		{0x9A, &cpu::TXS},             // Transfer X -> SP
		{0x98, &cpu::TYA},             // Transfer Y -> A

		// --- Decrements / Increments ---
		{0xE6, &cpu::DEC_zeropage},    // Decrement Zero Page
		{0xF6, &cpu::DEC_zeropageX},   // Decrement Zero Page,X
		{0xCE, &cpu::DEC_absolute},    // Decrement Absolute
		{0xDE, &cpu::DEC_absoluteX},   // Decrement Absolute,X
		{0xCA, &cpu::DEX},             // Decrement X
		{0x88, &cpu::DEY},             // Decrement Y
		{0xE7, &cpu::INC_zeropage},    // Increment Zero Page
		{0xF7, &cpu::INC_zeropageX},   // Increment Zero Page,X
		{0xCE, &cpu::INC_absolute},    // Increment Absolute
		{0xDE, &cpu::INC_absoluteX},   // Increment Absolute,X
		{0xC8, &cpu::INY},             // Increment Y
		{0xE8, &cpu::INX}              // Increment X
	};


	//Transfer Instructions
	// LDA
	void LDA_Immediate();
	void LDA_ZeroPage();
	void LDA_ZeroPageX();
	void LDA_absolute();
	void LDA_absoluteX();
	void LDA_absoluteY();
	void LDA_indirectX();
	void LDA_indirectY();

	//LDX Instruction
	void LDX_Immediate();
	void LDX_ZeroPage();
	void LDX_ZeroPageY();
	void LDX_absolute();
	void LDX_absoluteY();

	//LDY Instruction
	void LDY_Immediate();
	void LDY_ZeroPage();
	void LDY_ZeroPageX();
	void LDY_absolute();
	void LDY_absoluteX();

	// STA Instruction
	void STA_zeropage();
	void STA_zeropageX();
	void STA_absolute();
	void STA_absoluteX();
	void STA_absoluteY();
	void STA_indirectX();
	void STA_indirectY();

	// STX Instruction
	void STX_zeropage();
	void STX_zeropageY();
	void STX_absolute();

	// STY Instruction
	void STY_zeropage();
	void STY_zeropageX();
	void STY_absolute();

	// transfer commands 
	void TAX(); // A --> X
	void TAY(); // A --> Y
	void TSX(); // SP --> X
	void TXA(); // X --> A
	void TXS(); // X --> SP
	void TYA(); // Y --> A

	// stack Instruction
	void PHA(); // push A
	void PHP(); // push P
	void PLA(); // pull A
	void PLP(); // pull P

	// Decrements / Increments
	void DEC_zeropage();
	void DEC_zeropageX();
	void DEC_absolute();
	void DEC_absoluteX();


	void DEX();
	void DEY();

	void INC_zeropage();
	void INC_zeropageX();
	void INC_absolute();
	void INC_absoluteX();
	
	void INX();
	void INY();

	// flag instructionts
	void clc();

	void ADC_imediate();
	void ADC_zeroPage();
	void ADC_zeroPageX();
	void ADC_absolute();
	void ADC_absoluteX();
	void ADC_absoluteY();
	void ADC_indirectX();
	void ADC_indirectY();




};
