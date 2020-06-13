//
// Created by xelarse on 13/06/2020.
//

#ifndef CHIPPU_8U_GFX_HPP
#define CHIPPU_8U_GFX_HPP

//Class that interfaces with SDL in order to draw to screen
//This class handles the given pixel set in the CPU and scales and adjusts them to fit window res and renders

class Gfx
{
public:

	Gfx() = default;
	~Gfx() = default;

	//Sets up the window ready for rendering
	void init();

	bool isRunning();

	// Draws the current cpu pixels to screen, NEEDS A PARAMETER LATER FOR CPU DATA
	void draw();

private:

	//Potentially a pixel buffer like SFML ?

};

#endif //CHIPPU_8U_GFX_HPP
