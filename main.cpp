#include <iostream>

#include <calc/calc.h>

int main() {
    std::cout << "Hey did you know 8 + 10 = " << Calculator::add(8, 10) << std::endl;
    std::cout << "Calc library working = " << CALC_LIB_PREPROCESSOR_WORKING << std::endl;
    std::cout << "Calc lib version = " << CALC_LIB_VERSION << std::endl;
    return 0;
}
