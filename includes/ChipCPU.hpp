//
// Created by xelarse on 13/06/2020.
//

#ifndef CHIPPU_8U_CHIPCPU_HPP
#define CHIPPU_8U_CHIPCPU_HPP

#include <memory>

using byte = unsigned char;			//Byte variable for memory dealings
using opcode = unsigned short;		// opcodes are 2 bytes long, shorts are 16 bits

/*	System memory map
	 	0x000-0x1FF - Chip 8 interpreter (contains font set in emu) - 1+2+4+8+16+32+64+128+256 aka 0 - 511
		0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F) -
		0x200-0xFFF - Program ROM and work RAM - 512 - 4096
*/


// INFO ACCESSED AT - https://en.wikipedia.org/wiki/CHIP-8#Virtual_machine_description
// TUT AT - http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter//

class ChipCPU
{
public:
	ChipCPU() = default;
	~ChipCPU() = default;

	// Clears the memory, registers and screen. Ensures everything is at default
	void init();

	// Takes a filepath and attempts to load the ROM, if false file loading failed
	bool isValidGame(const char* filepath);

	// Loads the ROM in ready for execution by the emulator
	bool loadGame(const char* filepath);

	// Check if the cpu is still actively processing
	bool isRunning();

	// Emulates a single cycle
	void tick();

	// Returns the state of the draw flag
	bool shouldDraw();

	// Sets key states based upon inputted bitset?
	void setKeys();

private:

	// Gets the current opcode based on the program counter
	opcode fetchOpcode(const unsigned short _pc);

	// Takes the inputted opcode and runs the appropriate logic for it
	void decodeAndExecuteOpcode(const opcode& _code);

	void opcodeGroup8XYN(const opcode& _code);
	void opcodeGroupFXNN(const opcode& _code);
	void opcodeGroupEXNN(const opcode& _code);

	//Chip 8 has 4k memory in total
	std::array<byte, 4096> memory = {};

	//Chip 8 has 15 8-bit general registers V0 - VF
	std::array<byte, 16> V = {};

	//Chip 8 has an index register "I" and a program counter "pc" both 16 bits in size
	unsigned short I = 0;
	unsigned short pc = 0;

	//Graphics system draws to screen by XORing if a pixel is turned off as a result of drawing VF is set, used for collision detection
	//Since the screen is 64 x 32 of black or white pixels its another easy byte array of on or off, could potentially make better with a bitset here?
	std::array<byte, (64*32)> gfx = {};

	//Chip 8 doesnt have interrupts or hardware registers but it has two timer registers that count as 60hz. When set above zero they count down to it.
	//System buzzer sounds when the timer reaches zero
	unsigned char delayTimer = 0;
	unsigned char soundTimer = 0;

	//Chip 8 has opcodes that allow the program to jump to certain address, a subroutine. a stack will be needed to remember which memory location to come back to once this is done
	//In order to remember which level of the stack is currently being used a stack pointer is required
	std::array<unsigned short, 16> stack = {};
	unsigned short stackPtr = 0;

	//Chip 8 has a hex based keypad 0x0 - 0xf, meaning you can have a total of 16 keys, typically 8, 4, 6, 2 are used for directional input. To remember the state of a key it gets saved here
	std::array<byte, 16> key = {};

};

#endif //CHIPPU_8U_CHIPCPU_HPP
