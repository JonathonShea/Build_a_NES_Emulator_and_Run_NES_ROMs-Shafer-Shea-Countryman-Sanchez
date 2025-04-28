/*
    Created by Ryan Countryman on 11/17/2024
    Last Updated: 3/23/2025
*/
#include "PPU.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <array>
//#include <SDL2/SDL.h>
#include <iomanip>

PPU::PPU(){
    patternTables.resize(2, std::vector<uint8_t>(256 * 8 * 8, 0)); //Left and Right Pattern Tables initialized with tile map
    paletteMemory.resize(32, 0x0F); // Initialize with black (0x0F)
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

std::array<uint8_t, 64> PPU::getPatternTile(int tableIndex, int tileIndex) const {
    std::array<uint8_t, 64> tile{};

    if (tableIndex < 0 || tableIndex >= 2 || tileIndex < 0 || tileIndex >= 256) {
        throw std::out_of_range("Invalide Pattern Table or index");
    }

    const int offset = tileIndex * 64;

    for (int i = 0; i < 64; ++i) {
        tile[i] = patternTables[tableIndex][offset + i];
    }

    return tile;
}

//Global NES color palette with specified RGB values
const RGB PPU::nes_color_palette[64] = {
    {84, 84, 84},   // $00
    {0, 30, 116},   // $01
    {8, 16, 144},   // $02
    {48, 0, 136},   // $03
    {68, 0, 100},   // $04
    {92, 0, 48},    // $05
    {84, 4, 0},     // $06
    {60, 24, 0},    // $07
    {32, 42, 0},    // $08
    {8, 58, 0},     // $09
    {0, 64, 0},     // $0A
    {0, 60, 0},     // $0B
    {0, 50, 60},    // $0C
    {0, 0, 0},      // $0D
    {0, 0, 0},      // $0E
    {0, 0, 0},      // $0F

    {152, 150, 152},// $10
    {8, 76, 196},   // $11
    {48, 50, 236},  // $12
    {92, 30, 228},  // $13
    {136, 20, 176}, // $14
    {160, 20, 100}, // $15
    {152, 34, 32},  // $16
    {120, 60, 0},   // $17
    {84, 90, 0},    // $18
    {40, 114, 0},   // $19
    {8, 124, 0},    // $1A
    {0, 118, 40},   // $1B
    {0, 102, 120},  // $1C
    {0, 0, 0},      // $1D
    {0, 0, 0},      // $1E
    {0, 0, 0},      // $1F

    {236, 238, 236},// $20
    {76, 154, 236}, // $21
    {120, 124, 236},// $22
    {176, 98, 236}, // $23
    {228, 84, 236}, // $24
    {236, 88, 180}, // $25
    {236, 106, 100},// $26
    {212, 136, 32}, // $27
    {160, 170, 0},  // $28
    {116, 196, 0},  // $29
    {76, 208, 32},  // $2A
    {56, 204, 108}, // $2B
    {56, 180, 204}, // $2C
    {60, 60, 60},   // $2D
    {0, 0, 0},      // $2E
    {0, 0, 0},      // $2F

    {236, 238, 236},// $30
    {168, 204, 236},// $31
    {188, 188, 236},// $32
    {212, 178, 236},// $33
    {236, 174, 236},// $34
    {236, 174, 212},// $35
    {236, 180, 176},// $36
    {228, 196, 144},// $37
    {204, 210, 120},// $38
    {180, 222, 120},// $39
    {168, 226, 144},// $3A
    {152, 226, 180},// $3B
    {160, 214, 228},// $3C
    {160, 162, 160},// $3D
    {0, 0, 0},      // $3E
    {0, 0, 0}       // $3F
};

//Obtain RGB Value from provided Hex Index
RGB PPU::getColor(uint8_t paletteIndex) const {
    return nes_color_palette[paletteIndex & 0x3F];
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

// Read from palette memory
uint8_t PPU::readPaletteMemory(uint16_t address) {
    // Ensure the address is in the palette range (0-31)
    address &= 0x1F;

    // Handle palette mirroring
    if (address >= 0x10 && (address & 0x03) == 0) {
        address &= 0x0F;
    }

    return paletteMemory[address];
}

// Write to palette memory
void PPU::writePaletteMemory(uint16_t address, uint8_t data) {
    address &= 0x1F;

    // Handle palette mirroring
    if (address >= 0x10 && (address & 0x03) == 0) {
        address &= 0x0F;
    }

    paletteMemory[address] = data & 0x3F;
}

void PPU::step(){
    
}

//Access Proper NameTable/AttributeTable
int PPU::getTableIndex(uint16_t address) const{
    address &= 0x0FFF; //Restricting access to only 0x2000 - 0x3FFF

    int tableIndex = -1;

    //Vertical Mirroring
    if (address < 0x0400) {
        tableIndex = 0; //NameTable 0
    }
    else if (address < 0x0800){
        tableIndex = 1; //NameTable 1
    }
    else if (address < 0x0C00) {
        tableIndex = 0; //Mirrored NameTable 0
    }
    else {
        tableIndex = 1; //Mirrored NameTable 1
    }

    return tableIndex;

}

//Write to NameTable and AttributeTable
void PPU::writeNameTable(uint16_t address, uint8_t data) {
    int tableIndex = getTableIndex(address);
    if (tableIndex == -1) return; //Non-valid Index

    uint16_t offset = address & 0x03FF;

    if (offset < 960) { //First 960 Bytes for Name Table
        nameTables[tableIndex].tiles[offset] = data;
    }
    else { //Last 64 bytes for Attributes Table
        nameTables[tableIndex].attributes[offset - 960] = data;
    }
}

//Read NameTable and AtrributeTable
uint8_t PPU::readNameTable(uint16_t address) const {
    int tableIndex = getTableIndex(address);
    if (tableIndex == -1) return 0xFF; //Non-valid Index

    uint16_t offset = address & 0x03FF;

    if (offset < 960) { //First 960 Bytes for Name Table
        return nameTables[tableIndex].tiles[offset];
    }
    else { //Last 64 bytes for Attributes Table
        return nameTables[tableIndex].attributes[offset - 960];
    }
}

/* USED FOR LOCALIZED DEBUGGING / UTILITY
 static constexpr std::array<uint8_t, 4> magicNumbers = { 0x4E, 0x45, 0x53, 0x1A }; // NES<EOF> magic numbers to identify a NES ROM file

int main(){
    
    
    std::ifstream romFile;
    std::vector<uint8_t> romData;
    std::string filePath = /INSERT PATH TO ROM
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
    createHexDump(cartridge.chrRom, "chrrom_hexdump.txt");

    testPPU.loadPatternTable(cartridge.chrRom);
    
    return 0;
    
}
*/