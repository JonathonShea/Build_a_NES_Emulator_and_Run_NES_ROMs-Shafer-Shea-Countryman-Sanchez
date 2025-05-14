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
}

void PPU::initializeFrameBuffer(int width, int height, const std::string& filename) {
    // Step 1: Create a pixel buffer filled with black pixels
    std::vector<uint8_t> pixelBuffer(width * height * 3, 0); // 3 bytes per pixel (RGB), initialized to 0 (black)

    // Step 2: Write the pixel buffer to a BMP file
    writeBMP(pixelBuffer, width, height, filename);

    std::cout << "Initialized frame buffer with dimensions " << width << "x" << height
              << " and saved to " << filename << std::endl;
}

void PPU::setPixel(std::vector<uint8_t>& pixelBuffer, int x, int y, const RGB& color, int imageWidth, int imageHeight) {
    // BMP stores pixels bottom-up, so we invert y
    int flippedY = (imageHeight - 1) - y;

    int bytesPerPixel = 3; // RGB
    int index = (flippedY * imageWidth + x) * bytesPerPixel;

    if (index + 2 < pixelBuffer.size()) { // Bounds check
        pixelBuffer[index + 0] = color.b; // BMP uses BGR format
        pixelBuffer[index + 1] = color.g;
        pixelBuffer[index + 2] = color.r;
    }
}

void PPU::writePixel(int x, int y, const RGB& color, const std::string& filename) {
    std::vector<uint8_t> pixelBuffer;
    int imageWidth, imageHeight;

    // Step 1: Read the existing BMP image into the pixel buffer
    if (!readBMP(filename, pixelBuffer, imageWidth, imageHeight)) {
        std::cerr << "Failed to read BMP file!" << std::endl;
        return;
    }

    // Step 2: Ensure that (x, y) is within bounds
    if (x < 0 || x >= imageWidth || y < 0 || y >= imageHeight) {
        std::cerr << "Invalid coordinates: (" << x << ", " << y
            << ") out of bounds (width: " << imageWidth
            << ", height: " << imageHeight << ")" << std::endl;
        return;
    }

    // Step 3: Calculate the index in the pixel buffer for the given (x, y)
    int paddingSize = (4 - (imageWidth * 3) % 4) % 4;
    int rowSize = (imageWidth * 3) + paddingSize;
    int index = (y * rowSize) + (x * 3);

    // Step 4: Update the pixel at the given position (x, y)
    pixelBuffer[index + 0] = color.b; // Blue
    pixelBuffer[index + 1] = color.g; // Green
    pixelBuffer[index + 2] = color.r; // Red

    // Step 5: Write the updated pixel buffer back to the BMP file
    writeBMP(pixelBuffer, imageWidth, imageHeight, filename);
}

/*
Function to write a scanline to the BMP file
To create the colors vector:

std::vector<RGB> colors;
colors.reserve(256); // Nes scanline is 256 pixels wide

to add a color to the vector:
colors.emplace_back(RGB{r, g, b}); // where r, g, b are the RGB values for the color

Then call the function:
writeScanline(scanline, colors, filename);
where scanline is the y-coordinate of the scanline to write (0-239 for a 240p image) and filename is the path to the BMP file.
*/

void PPU::writeScanline(int scanline, const std::vector<RGB>& colors, const std::string& filename) {
    std::vector<uint8_t> pixelBuffer;
    int imageWidth, imageHeight;

    // Step 1: Read the existing BMP image into the pixel buffer
    if (!readBMP(filename, pixelBuffer, imageWidth, imageHeight)) {
        std::cerr << "Failed to read BMP file!" << std::endl;
        return;
    }

    // Step 2: Validate scanline number
    if (scanline < 0 || scanline >= imageHeight) {
        std::cerr << "Invalid scanline: " << scanline
                  << " out of bounds (height: " << imageHeight << ")" << std::endl;
        return;
    }

    // Step 3: Validate color list length
    if (colors.size() != static_cast<size_t>(imageWidth)) {
        std::cerr << "Color list size (" << colors.size()
                  << ") does not match image width (" << imageWidth << ")" << std::endl;
        return;
    }

    // Step 4: Calculate padding and row size
    int paddingSize = (4 - (imageWidth * 3) % 4) % 4;
    int rowSize = (imageWidth * 3) + paddingSize;
    int rowStartIndex = scanline * rowSize;

    // Step 5: Update each pixel in the scanline
    for (int x = 0; x < imageWidth; ++x) {
        int index = rowStartIndex + (x * 3);
        pixelBuffer[index + 0] = colors[x].b; // Blue
        pixelBuffer[index + 1] = colors[x].g; // Green
        pixelBuffer[index + 2] = colors[x].r; // Red
    }

    // Step 6: Write the updated pixel buffer back to the BMP file
    writeBMP(pixelBuffer, imageWidth, imageHeight, filename);
}

void PPU::writeBMP(const std::vector<uint8_t>& pixelBuffer, int imageWidth, int imageHeight, const std::string& filename) {
    std::ofstream out(filename, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << " for writing.\n";
        return;
    }

    // Calculate padding for row alignment
    int paddingSize = (4 - (imageWidth * 3) % 4) % 4;
    int rowSize = (imageWidth * 3) + paddingSize;
    int pixelDataSize = rowSize * imageHeight;
    int fileSize = 54 + pixelDataSize;

    // BMP Header
    out.put('B').put('M');
    out.write(reinterpret_cast<const char*>(&fileSize), 4);
    int reserved = 0;
    out.write(reinterpret_cast<const char*>(&reserved), 4);
    int dataOffset = 54;
    out.write(reinterpret_cast<const char*>(&dataOffset), 4);

    // DIB Header
    int headerSize = 40;
    out.write(reinterpret_cast<const char*>(&headerSize), 4);
    out.write(reinterpret_cast<const char*>(&imageWidth), 4);
    out.write(reinterpret_cast<const char*>(&imageHeight), 4);
    uint16_t planes = 1;
    out.write(reinterpret_cast<const char*>(&planes), 2);
    uint16_t bitsPerPixel = 24;
    out.write(reinterpret_cast<const char*>(&bitsPerPixel), 2);
    int compression = 0;
    out.write(reinterpret_cast<const char*>(&compression), 4);
    out.write(reinterpret_cast<const char*>(&pixelDataSize), 4);
    int ppm = 2835; // 72 DPI
    out.write(reinterpret_cast<const char*>(&ppm), 4);
    out.write(reinterpret_cast<const char*>(&ppm), 4);
    int colorsUsed = 0;
    int importantColors = 0;
    out.write(reinterpret_cast<const char*>(&colorsUsed), 4);
    out.write(reinterpret_cast<const char*>(&importantColors), 4);

    // Pixel Data
    for (int y = 0; y < imageHeight; ++y) {
        out.write(reinterpret_cast<const char*>(&pixelBuffer[y * imageWidth * 3]), imageWidth * 3);
        // Write padding
        for (int p = 0; p < paddingSize; ++p)
            out.put(0);
    }

    out.close();
}

bool PPU::readBMP(const std::string& filename, std::vector<uint8_t>& pixelBuffer, int& imageWidth, int& imageHeight) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Failed to open " << filename << " for reading.\n";
        return false;
    }

    // Read BMP Header
    char header[54];
    in.read(header, 54);

    // Get image dimensions
    imageWidth = *reinterpret_cast<int*>(&header[18]);
    imageHeight = *reinterpret_cast<int*>(&header[22]);

    // Calculate padding for row alignment
    int paddingSize = (4 - (imageWidth * 3) % 4) % 4;

    // Prepare the pixel buffer
    int rowSize = (imageWidth * 3) + paddingSize;
    int pixelDataSize = rowSize * imageHeight;
    pixelBuffer.resize(pixelDataSize);

    // Read pixel data into the buffer
    in.read(reinterpret_cast<char*>(pixelBuffer.data()), pixelDataSize);
    in.close();

    return true;
}

void PPU::dumpPatternTablesToBitmap(const std::string& filename) {
    const int tileSize = 8;         // 8x8 tiles
    const int tilesPerRow = 16;     // 16 tiles per row
    const int tableWidth = tileSize * tilesPerRow; // 128 pixels
    const int tableHeight = tileSize * tilesPerRow; // 128 pixels
    const int imageWidth = tableWidth * 2;  // Two tables side by side
    const int imageHeight = tableHeight;

    // Fill the pixel buffer with black (initial value)
    std::vector<uint8_t> pixelBuffer(imageWidth * imageHeight * 3, 0);

    for (int table = 0; table < 2; ++table) {
        for (int tile = 0; tile < 256; ++tile) {
            int tileX = tile % 16;
            int tileY = tile / 16;

            for (int row = 0; row < 8; ++row) {
                for (int col = 0; col < 8; ++col) {
                    int index = (tile * 64) + (row * 8) + col;
                    uint8_t pixelVal = patternTables[table][index];

                    RGB color;
                    switch (pixelVal) {
                    case 0: color = { 0, 0, 0 }; break; // Black (transparent)
                    case 1: color = { 85, 85, 85 }; break; // Dark gray
                    case 2: color = { 170, 170, 170 }; break; // Light gray
                    case 3: color = { 255, 255, 255 }; break; // White
                    }

                    int x = (table * tableWidth) + (tileX * tileSize) + col;
                    int y = (tileY * tileSize) + row;
                    setPixel(pixelBuffer, x, y, color, imageWidth, imageHeight);
                }
            }
        }
    }

    // Write the pixel buffer to BMP file
    writeBMP(pixelBuffer, imageWidth, imageHeight, filename);
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

    uint8_t low = tilePlaneLow[tableIndex][tileIndex][row]; //Pull staged tile data
    uint8_t high = tilePlaneHigh[tableIndex][tileIndex][row];

    for (int col = 0; col < 8; ++col) {
        uint8_t bit1 = (low >> (7 - col)) & 1;
        uint8_t bit2 = (high >> (7 - col)) & 1;
        uint8_t pixelVal = (bit2 << 1) | bit1;
        patternTables[tableIndex][tileIndex * 64 + row * 8 + col] = pixelVal;
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
    PPUCTRL = m_bus->read(0x2000); //update this guy
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
                uint16_t nametable_addr =  0x2000 | (vram_address & 0x0FFF);
                uint8_t nametable_byte = Read(nametable_addr);
                writeNameTable(nametable_addr, nametable_byte);
            }
            else if (timing == 3) {
                // TODO: fetch attribute byte here
            }
            else if (timing == 5) {
                // TODO: fetch low byte of pattern table here
            }

            else if (timing == 7) {
                // TODO: fetch high byte of pattern table here
            }
            else if (timing == 0) {
                // TODO: switch nametable used here. Think this just flip a bit
                // TODO: scroll_x gets incremented here I think too
            }
            else{
                // Not doing anything else for the other values.
                // Most of these are taking up two PPU clock cycles so we just do them on the first cycle
            }


            // Write the scanline to the BMP file (filename should be set elsewhere)
            if(RenderingEnabled()){

                RenderScanline(); // This is really just generating the scanline that gets pumped into the following function. Shift registers get updated here?
                
            }
        }
    }
    if(dot >= 257 && dot <= 320) { // 257-320
        // TODO: Some sort of OAM/Sprite eval happens here. This is for the next scanline
    }
    dot++;
    if (dot == 340){
        // This is just a test for now.
        // Settings some BS value.
        std::vector<RGB> frame;
        for(int i = 0; i < 256; i++){
            RGB val(1,1,1);
            if(scanline % 2 == 0){
                toggle2? val.b = 200: val.b = 0;
                toggle? val.g = 200: val.g = 0;
            }
            else{
                toggle2? val.r = 200: val.r = 0;
            }
            toggle = !toggle;
            frame.push_back(val);
        }
        // Should be done. Let er rip
        writeScanline(scanline, frame, framebufferFilename); // Gwyn's output to SDL drawing
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
void PPU::RenderScanline() {
// TODO: stuff gets muxed into the shift registers and then that is evaluated as the color???
// Think sprites come in last through a priority mux.
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
