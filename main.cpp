#include <iostream>
#include <SDL2/SDL.h>


#include "ChipCPU.hpp"
#include "Gfx.hpp"
#include "Input.hpp"


int main()
{
	ChipCPU chipCpu = ChipCPU();
	Gfx gfx = Gfx();
	Input input = Input();


	//Gets filepath from user for ROM and safety check that it loads in
	std::string filepath;
	std::cout << "Please enter filepath to ROM:" << std::endl;
	std::cin >> filepath;

	while (!chipCpu.isValidGame(filepath.c_str()))
	{
		std::cout << "ROM entered not valid, please specify the correct path to a real ROM:" << std::endl;
		std::cin >> filepath;
	}

	//Set up render system and input callbacks
	gfx.init();
	input.init();

	//Initialises the ChipCPU with the filepath to the ROM
	chipCpu.init();
	chipCpu.loadGame(filepath.c_str());

	//Emulation loop
	while(chipCpu.isRunning() && gfx.isRunning())
	{
		//Emulate a cycle
		chipCpu.tick();

		//Because the system does not draw every cycle, we should set a draw flag when we need to update our screen. Only two opcodes should set this flag:
		//0x00E0 – Clears the screen
		//0xDXYN – Draws a sprite on the screen
		if (chipCpu.shouldDraw())
		{
			//Will need to take data from cpu to draw
			gfx.draw();
		}

		//Updates the set keys on the CPU, will need to add a parameter to this to take data from input class
		chipCpu.setKeys();
	}

	// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter//
	// Up to Emulation cycle

	return 0;
}
