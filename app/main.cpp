//
// Created by sheaj on 11/15/2024.
//

#include <iostream>
#include <fstream>
#include <string> 
#include <Cartridge.h>
#include <Utilities.h>

static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file


int main(int argc, const char * argv[]){

  std::cout << "Hello World!" <<std::endl;
  return 0;
}
