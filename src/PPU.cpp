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

static uint32_t framebuffer[PPU_WIDTH * PPU_HEIGHT];

PPU::PPU(std::shared_ptr<Bus> bus, std::shared_ptr<Cartridge> cart, std::shared_ptr<OAM> oam) : dot(0),
    m_cart(cart), m_bus(bus), m_oam(oam) {
    patternTables.resize(2, std::vector<uint8_t>(256 * 8 * 8, 0)); //Left and Right Pattern Tables initialized with tile map
    paletteMemory.resize(32, 0x0F); // Initialize with black (0x0F)
    scroll_latch = false;
    scroll_x = 0;
    scroll_y = 0;
    addr_latch = false;
    addr_high = 0;
    addr_low = 0;
    vram_address = 0;
    scanlineBuffer = std::vector<RGB>();
    setVBlank();
}

PPU::PPU(){
    paletteMemory = std::vector<uint8_t>(0xFFFF); // for testing
    patternTables.resize(2, std::vector<uint8_t>(256 * 8 * 8, 0));
}

const uint32_t* PPU::getFrameBuffer() {
    return framebuffer;
}

void PPU::writeToFrameBuffer(int scanline, const std::vector<RGB>& colors) {
    if (scanline < 0 || scanline >= PPU_HEIGHT) {
        std::cerr << "Invalid scanline: " << scanline << " (must be between 0 and " << PPU_HEIGHT - 1 << ")" << std::endl;
        return;
    }

    if (colors.size() != PPU_WIDTH) {
        std::cerr << "Color list size (" << colors.size() << ") does not match frame buffer width (" << PPU_WIDTH << ")" << std::endl;
        return;
    }

    for (int x = 0; x < PPU_WIDTH; ++x) {
        int index = (scanline * PPU_WIDTH) + x;
        framebuffer[index] = (colors[x].r << 16) | (colors[x].g << 8) | colors[x].b; // Pack RGB into a 32-bit value
    }
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

void PPU::writePatternTable(uint16_t address, uint8_t data) {
    if (address >= chrRam.size()) {
        std::cerr << "CHR-RAM write is out of bounds at address: " << std::hex << address << std::endl;
        return;
    }
    chrRam[address] = data;
    
    int tableIndex = address / 0x1000; // 0 -> Left Table : 1 -> Right Table
    uint16_t localAddr = address % 0x1000;

    int tileIndex = localAddr / 16; //
    int tileByte = localAddr % 16; //Where in the tile does the byte fall

    if (tileIndex >= 256) {
        std::cerr << "Tile index out of range. Index: " << tileIndex << std::endl;
        return;
    }

    bool isLowplane = tileByte < 8;
    int row = tileByte % 8;

    if (isLowplane) { //Stage for imcomplete tiles
        tilePlaneLow[tableIndex][tileIndex][row] = data;
    }
    else {
        tilePlaneHigh[tableIndex][tileIndex][row] = data;
    }
    
    if (!isLowplane) {
        uint8_t low = tilePlaneLow[tableIndex][tileIndex][row]; //Pull staged tile data
        uint8_t high = tilePlaneHigh[tableIndex][tileIndex][row];

        for (int col = 0; col < 8; ++col) {
            uint8_t bit1 = (low >> (7 - col)) & 1;
            uint8_t bit2 = (high >> (7 - col)) & 1;
            uint8_t pixelVal = (bit2 << 1) | bit1;
            patternTables[tableIndex][tileIndex * 64 + row * 8 + col] = pixelVal;
        }

    }
}

std::array<uint8_t, 64> PPU::getPatternTile(int tableIndex, int tileIndex) const {
    std::array<uint8_t, 64> tile{};

    if (tableIndex < 0 || tableIndex >= 2 || tileIndex < 0 || tileIndex >= 256) {
        throw std::out_of_range("Invalid Pattern Table or index");
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

/**
 * This is the function that performs each "tick" of the PPU. Certain actions are performed based on the current cycle.
 * stepScanLine() is also called which performs additional operations based on a different set of cycle values.
 * Cycle wraps around at the end.
 */
void PPU::step() {
    // Visible scanlines: 0-239. Checking this in the other function
    // VBlank: scanline 241-260 CPU do thing
    // Pre-render: scanline 261
    cpuWrite(0x2000, m_bus->read(0x2000));
    cpuWrite(0x2005, m_bus->read(0x2005));
    cpuWrite(0x2001, m_bus->read(0x2001));
    cpuWrite(0x2006, m_bus->read(0x2006));
    // Reset here! CPU does stuff or something
    if (scanline == 241 && dot == 1) {
        setVBlank();
        cpuWrite(0x2002, 0);
        setNMI(); // CUrrently breaking stuff?
    }
    if (scanline == 261)
    {
        toggle2 = !toggle2;
        clearVBlank(); // useless
        // I think just zeroing this out works
        // All three used bits here get cleared
        PPUSTATUS = 0; 
    }
    stepScanline();
}


// Might bake into step, this is just breaking up the PPU step function. THis is where each pixel (dot) is handled
// Should this be called step dot? Maybe
void PPU::stepScanline()
{
    // Only render visible scanlines (0-239)
    if(scanline < 240){ 
        if (dot < 256) { // Visible pixels. Do stuff here!!!!
            int timing = dot % 8; // Using this to determine what value we are pulling on the cycle (nametable, attribute, etc.)
            if(timing == 1){ 
                // TODO: fetch nametable byte here
                vram_address = PPUADDR & 0x0F00;
                vram_address = vram_address >> 8;
                uint16_t nametable_addr =  0x2000 | (vram_address & 0x0FFF);
                fetched_nametable_byte = readNameTable(nametable_addr);
            }
            else if (timing == 3) {
                // fetch attribute byte here
                uint8_t coarse_x = vram_address & 0x1F;
                uint8_t coarse_y = (vram_address >> 5) & 0x1F;

                uint16_t attribute_addr = 0x23C0
                                        | (vram_address & 0x0C00)
                                        | ((coarse_y >> 2) << 3)
                                        | (coarse_x >> 2);
                fetched_attribute_byte = readNameTable(attribute_addr);
            }
            else if (timing == 5) {
                // TODO: fetch low byte of pattern table here
                uint8_t tile_index = fetched_nametable_byte;
                uint8_t fine_y = (vram_address >> 12) & 0x7;
                int table = (PPUCTRL & 0x10) ? 1 : 0;
                fetched_pattern_low = patternTables[table][tile_index * 16 + fine_y];

                // Load into shift register
                tile_low_shift.Insert(fetched_pattern_low);
            }

            else if (timing == 7) {
                // TODO: fetch high byte of pattern table here
                uint8_t tile_index = fetched_nametable_byte;
                uint8_t fine_y = (vram_address >> 12) & 0x7;
                int table = (PPUCTRL & 0x10) ? 1 : 0;
                fetched_pattern_high = patternTables[table][tile_index * 16 + fine_y + 8];

                // Load into shift register
                tile_high_shift.Insert(fetched_pattern_high);
                //std::cout << fetched_pattern_high << std::endl;
            }
            else if (timing == 0) {
                // TODO: switch nametable used here. Think this just flip a bit
                // TODO: scroll_x gets incremented here I think too
                // Switch nametable and increment horizontal position
                if ((vram_address & 0x001F) == 31) { // If we're at the end of a nametable row
                    vram_address &= ~0x001F;         // Clear the horizontal position
                    vram_address ^= 0x0400;          // Switch horizontal nametable
                }
                else {
                    vram_address++;                  // Increment horizontal position
                }

                // Increment scroll_x
                scroll_x = (scroll_x + 1) % 256;
            }
            // if(RenderingEnabled())
                scanlineBuffer.push_back(RenderScanline()); // This is really just generating the scanline that gets pumped into the following function. Shift registers get updated here?
            }
            else{
                // Not doing anything else for the other values.
                // Most of these are taking up two PPU clock cycles so we just do them on the first cycle
            }

            
            // Write the scanline to the BMP file (filename should be set elsewhere)
            // if(RenderingEnabled()){
     

                
            // }
    }

    if(dot >= 257 && dot <= 320) { // 257-320
        // TODO: Some sort of OAM/Sprite eval happens here. This is for the next scanline

    }  

        if (dot == 257) {
            // Clear secondary OAM 
            for (int i = 0; i < 8; i++) {
                sprite_data[i].y_pos = -1;
                sprite_data[i].tile_index = -1;
                sprite_data[i].attributes = -1;
                sprite_data[i].x_pos = -1;
            }

            // Initialize sprite count and overflow flag
            int activeSprites = 0;  // number of active sprites
            bool spriteOverflow = false;  // is true if number of sprites < 8

            // Check if sprite rendering is enabled
            if (PPUMASK & 0x10) {  // check bit 4 of PPUMASK
                // Pixel height is 16 if the fifth bit of PPUCTRL is set, else 8
                int spriteHeight;
                if (PPUCTRL & 0x20) {
                    spriteHeight = 16;
                }
                else {
                    spriteHeight = 8;
                }

                // cycle through OAM to find sprites for the next scanline
                for (int i = 0; i < oamSize; i++) {
                    Sprite& sprite = m_oam->sprites[i];
                    int y = sprite.y_pos;

                    // calculate the next scanline
                    int next_scanline = (scanline + 1) % 262;

                    // Check if this sprite will be visible on the next scanline
                    if (y <= next_scanline && next_scanline < y + spriteHeight) {
                        // If we've already found 8 sprites, set overflow flag
                        if (activeSprites >= 8) {
                            PPUSTATUS |= 0x20;
                            spriteOverflow = true;
                            break;  // Stop adding sprites
                        }

                        // Copy sprite to secondary OAM 
                        sprite_data[activeSprites] = sprite;
                        activeSprites++;  // increment the number of sprites
                    }
                }
            }

            // Clear sprite overflow flag
            if (!spriteOverflow) {
                PPUSTATUS &= ~0x20;
            }
        }

        // fetch sprite data for sprites found in eval
        int spriteIndex = (dot - 257) / 8;
        // ensure sprite is valid
        if (spriteIndex < 8 && sprite_data[spriteIndex].y_pos != -1) {
            // Get sprite data for current sprite
            Sprite& sprite = sprite_data[spriteIndex];

            // calculate which tile and row to fetch based on sprite attributes
            uint8_t tileIndex = sprite.tile_index;
            bool tall = (PPUCTRL & 0x20) != 0; // set to true if sprite has a height of 16
            uint8_t spritePatternTable;
            if (tall) {
                spritePatternTable = tileIndex & 1;  // use bit 0 of tile index
            }
            else {
                spritePatternTable = (PPUCTRL & 0x08) >> 3;  // use bit 3 of PPUCTRL
            }

            // Calculate Y offset within the sprite
            int next_scanline = (scanline + 1) % 262;
            uint8_t spriteY = next_scanline - sprite.y_pos;

            // Handle vertical flipping
            if (sprite.attributes & 0x80) {
                if (tall) {
                    spriteY = 15 - spriteY;  // For 8x16 sprites
                }
                else {
                    spriteY = 7 - spriteY;   // For 8x8 sprites
                }
            }

            // Calculate pattern table address
            uint16_t patternAddr;
            if (tall) {
                // 8x16 sprites
                tileIndex &= ~1; // Clear bottom bit for 8x16 sprites
                if (spriteY < 8) {
                    patternAddr = (spritePatternTable * 0x1000) + (tileIndex * 16) + spriteY;
                }
                else {
                    patternAddr = (spritePatternTable * 0x1000) + (tileIndex * 16) + 16 + (spriteY - 8);
                }
            }
            else {
                // 8x8 sprites
                patternAddr = (spritePatternTable * 0x1000) + (tileIndex * 16) + spriteY;
            }

            uint8_t spriteLow = Read(patternAddr);
            uint8_t spriteHigh = Read(patternAddr + 8);
            // Handle horizontal flipping
            if (sprite.attributes & 0x40) {
                // Bit reversal for horizontal flip
                uint8_t flippedLow = 0;
                uint8_t flippedHigh = 0;
                for (int b = 0; b < 8; b++) {
                    flippedLow |= ((spriteLow >> b) & 1) << (7 - b);
                    flippedHigh |= ((spriteHigh >> b) & 1) << (7 - b);
                }
                spriteLow = flippedLow;
                spriteHigh = flippedHigh;
            }

            // Store sprite data for rendering
            sprite_pattern_low[spriteIndex] = spriteLow;
            sprite_pattern_high[spriteIndex] = spriteHigh;
        }
    }


    dot++;
    if (dot == 340){
        // This is just a test for now.
        // Settings some BS value.
        // std::vector<RGB> scanlineBuffer;
        // for(int i = 0; i < 256; i++){
        //     RGB val(1,1,1);
        //     if(scanline % 2 == 0){
        //         toggle2? val.b = 200: val.b = 0;
        //         toggle? val.g = 200: val.g = 0;
        //     }
        //     else{
        //         toggle2? val.r = 200: val.r = 0;
        //     }
        //     toggle = !toggle;
        //     scanlineBuffer.push_back(val);
        // }
        // Should be done. Let er rip
        if(scanline < 240){
            writeToFrameBuffer(scanline, scanlineBuffer); // Gwyn's output to SDL drawing
            scanlineBuffer = std::vector<RGB>();
        }
        
    }
    if (dot > 340) {
        dot = 0;
        scanline++;
        if (scanline > 261) {
            scanline = 0;
        }
    }
}

void PPU::setNMI()
{
    if((PPUSTATUS & vBlankMask) && (PPUCTRL & 0x80)){
         m_bus->nmi = true;      
    }
}

// Gonna create the actual scanline here. Should this be render pixel? Probably
RGB PPU::RenderScanline() {
// TODO: stuff gets muxed into the shift registers and then that is evaluated as the color???
// Think sprites come in last through a priority mux.
    uint8_t pixel = tile_low_shift[15 - GetFineX()] << 3;
    pixel |= tile_high_shift[15 - GetFineX()] << 2;
    pixel |= attr_low_shift[7 - GetFineX()] << 1;
    pixel |= attr_low_shift[7 - GetFineX()];
    pixel &= 0xF0;
    return getColor(pixel);
}

int PPU::Read(uint16_t addr) const
{
    /**
     * 0000-0FFF: Pattern table 0
     * 1000-1FFF: Pattern table 1
     * 2000-23FF: Nametable 0
     * 2400-27FF: Nametable 1
     * 2800-2BFF: Nametable 2
     * 2C00-2FFF: Nametable 3
     * 3000-3EFF: Unused
     * 3F00-3F1F: Palette RAM indexes
     * 3F20-3FFF: Mirrors of 3F00-3F1F
     */
    if(addr < 0x1FFF){
        return m_cart->ReadChrRom(addr); // Read from CHR ROM
    }
    else if(addr >= 0x2000 && addr < 0x2FFF){
        return 0; // vram read for nametables
    }
    else if(addr >= 0x3000 && addr < 0x3EFF){
        return 0; // mirror
    }
    else{
    }
    return 0; // Invalid address
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

void PPU::write(uint16_t address, uint8_t data) {
    if (address < 0x2000) {
        //Possible overhaul of current PatternTable write logic
        writePatternTable(address, data);
    }
    else if (address >= 2000 && address <= 0x3EFF){
        writeNameTable(address,data);
    }
    else if (address >= 0x3F00 && address <= 0x3FFF) {
        writePaletteMemory(address, data);
    }
    else {
        std::cerr << "Invalid PPU Write to address: $" << std::hex << address << std::endl;
    }
}

//PPU MMIO Registers - - https://www.nesdev.org/wiki/PPU_registers -- For ref
void PPU::cpuWrite(uint16_t address, uint8_t data) {
    switch (address & 0x2007) {
    case 0x2000: // PPUCTRL
        PPUCTRL = data;
        m_bus->write(0x2000, data); // Write to bus
        break;

    case 0x2001: // PPUMASK
        PPUMASK = data;
        m_bus->write(0x2001, data); // Write to bus
        break;

    case 0x2002: // PPUSTATUS - READ ONLY
        m_bus->write(0x2002, PPUSTATUS); // Read PPU status
        break;

    case 0x2003: // OAMADDR
        OAMADDR = data;
        
        break;

    case 0x2004: // OAMDATA - Write to OAM
        OAMDATA = data;
        if (m_oam) {
            uint8_t spriteIndex = OAMADDR / 4;  // calculate sprite (0 - 63)
            uint8_t byteOffset = OAMADDR % 4;  // calculate sprite attribute (0 - 3)

            if (spriteIndex < oamSize) {
                switch (byteOffset) {
                case 0: // Update Y position
                    m_oam->sprites[spriteIndex].y_pos = static_cast<int8_t>(data);
                    break;
                case 1: // Update tile index
                    m_oam->sprites[spriteIndex].tile_index = static_cast<int8_t>(data);
                    break;
                case 2: // Update attributes
                    m_oam->sprites[spriteIndex].attributes = static_cast<int8_t>(data);
                    break;
                case 3: // X position
                    m_oam->sprites[spriteIndex].x_pos = static_cast<int8_t>(data);
                    break;
                }
            }
        }
        OAMADDR++;
        break;

    case 0x2005: // PPUSCROLL
        if (!scroll_latch) {
            // First write (false = x)
            scroll_x = data;
            scroll_latch = true;  // Ensures next write is y scroll
        }
        else {
            // Second write (true = y)
            scroll_y = data;
            scroll_latch = false;  // Ensures next write is x scroll
        }
        m_bus->write(0x2005, data); // Write to bus
        break;

    case 0x2006: // PPUADDR 
        if (!addr_latch) {
            // First write (false = addr_high)
            addr_high = data & 0x3F; // Only 6 bits are valid for high byte
            addr_latch = true;
        }
        else {
            // Second write (true = addr_low)
            addr_low = data;
            vram_address = (addr_high << 8) | addr_low;  // 
            addr_latch = false;
        }
        m_bus->write(0x2006, data); // Write to bus
        break;

    case 0x2007: // PPUDATA - Write to current VRAM address
        // Write to PPU memory at the current VRAM address
        write(vram_address, data);

        // Increment VRAM address based on PPUCTRL bit 2
        vram_address += (PPUCTRL & 0x04) ? 32 : 1;
        m_bus->write(0x2007, data); // Write to bus
        break;

    default:
        std::cerr << "Unknown PPU MMIO write $" << std::hex << address << std::endl;
    }
}
