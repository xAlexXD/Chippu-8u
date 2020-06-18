//
// Created by xelarse on 13/06/2020.
//

#include "ChipCPU.hpp"

void ChipCPU::init()
{
	//Init registers and mem once
}

bool ChipCPU::isValidGame(const char* filepath)
{
	return false;
}

bool ChipCPU::loadGame(const char* filepath)
{
	return false;
}

bool ChipCPU::isRunning()
{
	return false;
}

void ChipCPU::tick()
{
	// Using the program counter fetch the opcode for this tick
	// Yes this could be put in the parameter of decode and execute but for clarity
	opcode ticksOpcode = fetchOpcode(pc);

	// Using the calculated opcode determine the operation to do and execute it
	decodeAndExecuteOpcode(ticksOpcode);

	//Update Timers
}

bool ChipCPU::shouldDraw()
{
	return false;
}

void ChipCPU::setKeys()
{

}

opcode ChipCPU::fetchOpcode(const unsigned short _pc)
{
	// Since opcodes are 2 bytes and this array stores single bytes
	// Take the index of the pc and the following index and
	// use bitwise ops to append them to each other for the full opcode
	return memory[_pc] << 8 | memory[_pc + 1];
}

void ChipCPU::decodeAndExecuteOpcode(const opcode& _code)
{
	// Takes the opcode given by the parameter, decodes it then runs the appropriate logic
	// Since there are groups of certain opcodes that share a given amount of bits they will be tested and filtered first
	const opcode firstNibble = _code & 0xF000;

	if (firstNibble == 0x8000)
	{
		opcodeGroup8XYN(_code);
		return;
	}
	else if (firstNibble == 0xF000)
	{
		opcodeGroupFXNN(_code);
		return;
	}
	else if (firstNibble == 0xE000)
	{
		opcodeGroupEXNN(_code);
		return;
	}

	// The remaining opcodes get tested in the switch below.
	// This starts bottom up in the opcode list as there are a few that are zero

	switch (firstNibble)
	{
		case 0xD000:
			// DXYN - DISP - Draw(Vx, Vy, N) - draws at sprite co ord {vx,vy} with a width of 8 pixels and a height of N
			break;
		case 0xC000:
			// CXNN - RAND - Vx=rand()&NN - sets VX to the result of a bitwise and operation on ran number. Typically ran = 0 - 255
			break;
		case 0xB000:
			// BNNN - FLOW - PC=V0+NNN - Jumps to the address NNN plus V0
			break;
		case 0xA000:
			// ANNN - MEM - Sets I to the address NNN
			break;
		case 0x9000:
			// 9XY0 - COND - if(Vx != Vy) - Skips next instruction if VX != VY
			break;
		case 0x7000:
			// 7XNN - CONST - Vx += NN - Adds NN to VX, No change to carry flag
			break;
		case 0x6000:
			// 6XNN - CONST - Vx = NN - Sets Vx to NN
			break;
		case 0x5000:
			// 5XY0 - COND - if(Vx == Vy) - Skips next instruction if VX == VY
			break;
		case 0x4000:
			// 4XNN - COND - if(Vx != NN) - Skips next instruction if VX != NN
			break;
		case 0x3000:
			// 3XNN - COND - if(Vx == NN) - Skips next instruction if VX == NN
			break;
		case 0x2000:
			// 2NNN - FLOW - *(0xNNN)() - Calls subroutine at NNN
			break;
		case 0x1000:
			// 1NNN - FLOW - goto NNN - Jumps to address NNN
			break;
		case 0x0000:
			switch (_code)
			{
				case 0x00EE:
					// 00EE - FLOW - return; - Returns from a subroutine
					break;
				case 0x00E0:
					// 00E0 - DISP - disp_clear() - Clears the screen
					break;
				default:
					// 0NNN - CALL - Calls machine code routine at address NNN, not needed for most roms
					break;
			}
			break;
	}
}

void ChipCPU::opcodeGroup8XYN(const opcode& _code)
{
	// All 8 codes do something different depending on the last nibble / the N

	switch (_code & 0x000F)
	{
		case 0x0:
			// 8XY0 - ASSIGN - Vx = Vy
			break;
		case 0x1:
			// 8XY1 - BITOP - Vx = Vx | Vy - sets Vx to (Vx OR Vy)
			break;
		case 0x2:
			// 8XY2 - BITOP - Vx = Vx & Vy - sets Vx to (Vx AND Vy)
			break;
		case 0x3:
			// 8XY3 - BITOP - Vx = Vx ^ Vy - sets Vx to (Vx XOR Vy)
			break;
		case 0x4:
			// 8XY4 - MATH - Vx += Vy - Adds Vy to Vx. VF is set to 1 when theres a carry and 0 when there isnt
			break;
		case 0x5:
			// 8XY5 - MATH - Vx -= Vy - Vy is subtracted from Vx. VF is set to 0 when there's a borrow and 1 when there isnt
			break;
		case 0x6:
			// 8XY6 - BITOP - Vx >>= 1 - Stores the least significant bit of Vx in VF and then shifts Vx to the right by 1
			break;
		case 0x7:
			// 8XY7 - MATH - Vx = Vy - Vx - Sets Vx to Vy minus Vx. VF is set to 0 when theres a borrow and 1 when there isnt
			break;
		case 0xE:
			// 8XYE - BITOP - Vx <<= 1 - Stores the most significant bit of Vx in VF and then shifts Vx left by 1
			break;
	}
}

void ChipCPU::opcodeGroupFXNN(const opcode& _code)
{

}

void ChipCPU::opcodeGroupEXNN(const opcode& _code)
{
	// Operation is dependant on the final byte of the opcode

	switch (_code & 0x00FF)
	{
		case 0x9E:
			// EX9E - KEYOP - if(key() == Vx) - skips the next instruction if the key stored in Vx is pressed. usually next instruction is a jump to skip a code block
			break;
		case 0xA1:
			// EXA1 - KEYOP - if(key() != Vx) - skips the next instruction if the key stored in Vx isnt pressed. usually next instruction is a jump to skip a code block
			break;
	}
}