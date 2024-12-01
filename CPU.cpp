// Created by Abel Sanchez om 11/17/2024
// Last updated: 12/1/2024

#include "CPU.h"
#include <iostream>

void CPU::respTest()
{
    std::cout << "The CPU says hi! Say hi back or else..." << std::endl;
}

int main()
{
    CPU testCPU;
    testCPU.respTest();
    return 0;
}