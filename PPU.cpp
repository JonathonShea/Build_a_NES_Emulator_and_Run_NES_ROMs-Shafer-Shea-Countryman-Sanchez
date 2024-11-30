/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 11/17/2024
*/
#include "PPU.h"
#include <iostream>

void PPU::respTest()
{
    std::cout << "Hello from the PPU" << std::endl;
}

int main()
{
    PPU testPPU;
    testPPU.respTest();
    return 0;
}