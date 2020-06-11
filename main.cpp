#include <iostream>
#include <bitset>


void printHex(unsigned int val)
{
	std::cout << std::hex << val <<  std::dec << std::endl;
}

void printBinary(unsigned int val)
{
	std::cout << std::bitset<32>(val) << std::endl;
}


int main() {

	std::cout << "Hello world from CMake!" << std::endl;


    ////Bitwise operators
    // AND &
    // 1 & 1 = 1
    // 1 & 0 = 0
    // 0 & 0 = 0

    // OR |
    // 1 | 0 = 1
    // 1 | 1 = 1
    // 0 | 0 = 0

    //XOR ^ - Exclusive OR, something has to be true but only one instance of it
    //1 ^ 0 = 1
    //1 ^ 1 = 0
    //0 ^ 0 = 0

    //NOT ~ - it goes infront of something, inverses the bits, good for flipping flags in bitfields
    // ~0 = 32 bit number of 1's depends on the bit version of the software, these days generally 64
    // ~1 = 32 bit number of 0's

    ////Bit shifting
    // left shift << - essentially shifts all bits along the given amount - can extend to the size of the OS - 32 bit means 32 potential shifts to the left
    // 0100 << 1 = 1000 aka 4 became 8 so this whole operation is essentially ( lhs * 2^rhs )
    // 0001 << 2 = 0100 aka 1 became 2

    // right shift >> - Same as left shift but shifts all right the given amount - when shifting a large amount it eventually becomes 0
    // 1000 >> 1 = 0100 aka 8 became 4 so this whole operation is essentially ( lhs / 2^rhs )
    // 0100 >> 2 = 0001 aka 4 became 1

    //// HOW ITS USEFUL - in hexadecimal each digit is 4 bits so it lines up nicely
    // AND & - extracting parts of this number cause essentially we have 1 2 3 4 saves as individual 4 bits
	const unsigned int number = 0x1234;

	//Say we want the first 8 bits, this case being 3 and 4 we use AND with a mask of size 8 bits, this give us those 8 bits back
	unsigned int bitMask = 0x00ff;

	unsigned int andNumber = number & bitMask;

	//		0x1234
	//	&	0x00ff
	//	=	0x0034

    std::cout << "Hex and Bin for original number " << number << std::endl;
    printHex(number);
    printBinary(number);

	std::cout << "Hex and Bin for AND number " << andNumber << std::endl;
	printHex(andNumber);
	printBinary(andNumber);

	// ISSUE with this is we cant just get 12 from 34 with ADD as we have the trailing zeros
	// IN COMES BIT SHIFTING

	//TO get the number 12 from from 0x1234
	unsigned int firstByteNumber = 0x1234 & 0xff00;

	std::cout << "using bit shifting we can get the first byte from the hex" << std::endl;
	std::cout << "first we and to get the bits that we want: 0x1234 & 0xff00 = " << firstByteNumber << std::endl;
	std::cout << "then bit shift it a byte to the left to get the result were looking for: 0x1234 >> 8 = " << (firstByteNumber >> 8) << std::endl;


	//This is very useful for color values
	const unsigned int colourVal = 0x44bbeb;	// A blue colour in the format RGB

	//in this example each byte in the hex from left to right is R, G and B consisting of values between 0 - 255

	// Extracting the R, G and B values directly

	// Mask for the channel and shift the the bits depending on how far it is along the colour
	unsigned int colourValR = (colourVal & 0xff0000) >> 16;
	unsigned int colourValG = (colourVal & 0xff00) >> 8;
	unsigned int colourValB = (colourVal & 0xff);

	std::cout << "The colour values for the colour " << std::hex << colourVal << std::dec << " are as follows: " << std::endl;
	std::cout << "R: " << colourValR << std::endl;
	std::cout << "G: " << colourValG << std::endl;
	std::cout << "B: " << colourValB << std::endl;

	// Now to do something to it, minus some blue and put it back
	colourValB -= 50;

	unsigned int newColourVal = 0;

	//Since all the bits when shifting to the left are zero, if we OR them with the the colour val it will change them to the colour val

	newColourVal = 0x00 | colourValR;					//add the R channel
	newColourVal = (newColourVal << 8) | colourValG;	//add the G channel
	newColourVal = (newColourVal << 8) | colourValB;	//add the G channel

	//This can be shortened to
	newColourVal = colourValR << 16 | colourValG << 8 | colourValB;

	std::cout << "After subtracting 50 from the blue channel and putting it back into hex we get: "; printHex(newColourVal);

	return 0;
}
