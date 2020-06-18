//
// Created by xelarse on 13/06/2020.
//

#include "ChipCPU.hpp"

void ChipCPU::init()
{
	//Init registers and mem once

	pc = 0x200;			//Program counter starts at 0x200 as the memory prior is used by the system
	I = 0;				//Resets index register
	stackPtr = 0;		//Resets stack ptr

	gfx.fill(0);		//Resets the display
	stack.fill(0);	//Resets the stack
	V.fill(0);		//Resets the V0 - VF registers
	memory.fill(0);	//Resets the memory

	// Load fontset
	for (int i = 0; i < 80; ++i)
	{
		memory.at(i) = fontset.at(i);
	}

	// Reset timers - Do these go like high or zero?

}

bool ChipCPU::isValidGame(const char* filepath)
{
	// Load in game into a byte buffer

	// if byte buffer is larger than the memory buffer size MINUS 512 aka 0x200 then return false
	// else return true
	// This is to ensure the game fits into the cpu memory; the first 512 bytes of the memory buffer are used by system as the pc starts at 0x200
	return false;
}

bool ChipCPU::loadGame(const char* filepath)
{
	// Load the game into a byte buffer
	std::array<byte, 4096 - 512> gameData = {};	//Load it here

	// Take the contents of the buffer and store it in the cpus memory. Starts at 0x200 aka 512 into the memory due to everything prior being reserved for system
	for (int i = 0; i < gameData.size(); ++i)
	{
		memory.at(i + 512) = gameData.at(i);
	}
	return true;
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
	if(delayTimer > 0)
	{
		--delayTimer;
	}

	if(soundTimer > 0)
	{
		if(soundTimer == 1)
		{
			//Play beep sound
		}
		--soundTimer;
	}
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
	// Operation is dependant on the final byte of the opcode

	switch (_code & 0x00FF)
	{
		case 0x07:
			// FX07 - TIMER - Vx = get_delay() - sets Vx to the value of the delay timer
			break;
		case 0x0A:
			// FX0A - KEYOP - Vx = get_key() - A key press is awaited and then stored in Vx. (Blocking Operation, all instructions halted until the next key event)
			break;
		case 0x15:
			// FX15 - TIMER - delay_timer(Vx) - Sets the delay timer to Vx
			break;
		case 0x18:
			// FX18 - SOUND - sound_timer(Vx) - Set the sound timer to Vx
			break;
		case 0x1E:
			// FX1E - MEM - I += Vx - Adds Vx to I. VF is not affected.
			break;
		case 0x29:
			// FX29 - MEM - I = sprite_addr[Vx] - Sets I to the location of the sprite for the characters in Vx. Characters 0-F in hex are represented by a 4x5 font
			break;
		case 0x33:
			// FX33 - BCD - set_BCD(Vx); *(I+0) = BCD(3); *(I+1) = BCD(2); *(I+2) = BCD(1); - This is long, look it up
			break;
		case 0x55:
			// FX55 - MEM - reg_dump(Vx, &I) - Stores V0 to Vx (including Vx) in memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.
			break;
		case 0x65:
			// FX65 - MEM - reg_load(Vx, &I) - Fills V0 to Vx (including Vx) with values from memory starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified
			break;
	}
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