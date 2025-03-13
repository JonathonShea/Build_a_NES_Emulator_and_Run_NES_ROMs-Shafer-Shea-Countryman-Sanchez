/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 3/11/2025
*/
#include "PPU.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <array>
#include <SDL2/SDL.h>
#include <iomanip>


void PPU::respTest()
{
    std::cout << "Hello from the PPU" << std::endl;
}

PPU::PPU(){
    patternTables.resize(2, std::vector<uint8_t>(256 * 8 * 8, 0)); //Left and Right Pattern Tables initialized with tile map
}

void PPU::loadPatternTable(const std::vector<uint8_t>& chrROM) {
    if (chrROM.size() < 8192) { // Ensure pattern table is correct size
        throw std::runtime_error("CHR-ROM not correct size");
    }

    for (int table = 0; table < 2; table++) { //Loop for each pattern table Left and Right
        
        for (int tile = 0; tile < 256; tile++) { //Iterate over each tile
            int tileOffset = (table * 4096) + (tile * 16);
            
            for (int row = 0; row < 8; row++) { //Tiles are segmented into lowbit and highbit planes
                uint8_t lowPlane = chrROM[tileOffset + row];
                uint8_t highPlane = chrROM[tileOffset + row + 8];

                for (int col = 0; col < 8; col++) {
                    uint8_t bit1 = (lowPlane >> (7 - col)) & 1; //LSB
                    uint8_t bit2 = (highPlane >> (7 - col)) & 1; //MSB

                    /*Store and format MSB and LSB accordingly
                         //pixelVal = 0b00 ->Transparent 
                         //         = 0b01 -> Color 1
                         //         = 0b10 -> Color 2
                                    = 0b11 -> Color 3 */
                    uint8_t pixelVal = (bit2 << 1) | bit1;

                    patternTables[table][(tile * 64) + (row * 8) + col] = pixelVal;
                }
            }

       }
    }

}

void PPU::printPatternTables() {
    for (int table = 0; table < 2; table++) {
        std::cout << "Pattern Table " << table << ":\n";

        // Loop through each tile in the table
        for (int tile = 0; tile < 256; tile++) {
            std::cout << "Tile " << tile << ":\n";

            // Loop through each row of the tile
            for (int row = 0; row < 8; row++) {
                // Loop through each column in the row
                for (int col = 0; col < 8; col++) {
                    int index = (table * 256 * 8 * 8) + (tile * 8 * 8) + (row * 8) + col;
                    uint8_t pixelVal = patternTables[table][index];

                    std::cout << (int)pixelVal << " ";
                }
                std::cout << std::endl; // End of row
            }
            std::cout << std::endl; // End of tile
        }
    }
}


void createHexDump(const std::vector<uint8_t>& chrRom, const std::string& outputFile) {
    std::ofstream file(outputFile);

    if (!file.is_open()) {
        std::cerr << "Error opening file for output" << std::endl;
        return;
    }

    const int bytesPerRow = 16;  
    for (size_t i = 0; i < chrRom.size(); i += bytesPerRow) {
        file << std::hex << std::setw(6) << std::setfill('0') << i << ": "; // Print offset

        for (size_t j = 0; j < bytesPerRow && (i + j) < chrRom.size(); ++j) {
            file << std::hex << std::setw(2) << std::setfill('0') << (int)chrRom[i + j] << " ";
        }

        file << "\n";
    }

    file.close();
    std::cout << "Hex dump written to " << outputFile << std::endl;
}



/* USED FOR LOCALIZED DEBUGGING / UTILITY
 static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file

int main(){
    
    
    std::ifstream romFile;
    std::vector<uint8_t> romData;
    std::string filePath = "Donkey Kong (USA) (Rev 1) (e-Reader Edition).nes";
    std::cout << "ATTEMPTING TO OPEN\t" << filePath << std::endl;
    //filePath = Utilities::OpenFileDialog();

    romFile.open(filePath, std::ios::binary);
    if (romFile.is_open()) {
        std::cout << "Opened file: " << filePath << std::endl;
        romData = std::vector<uint8_t>(std::istreambuf_iterator<char>(romFile), std::istreambuf_iterator<char>());
        romFile.close();
    }
    else {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return(0);
    }
    for (int i = 0; i < magicNumbers.size(); i++) {
        if (romData[i] != magicNumbers[i]) {
            std::cout << "Invalid NES ROM file" << std::endl;
            return(0);
        }
    }

    ///auto path = Utilities::OpenFileDialog();
    PPU testPPU;
    Cartridge cartridge(romData);
    createHexDump(cartridge.chrRom, "dk_hexdump.txt");

    testPPU.loadPatternTable(cartridge.chrRom);
    
    return 0;
    
}
*/