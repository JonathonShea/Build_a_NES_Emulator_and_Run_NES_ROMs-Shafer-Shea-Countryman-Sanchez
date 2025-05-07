#include <gtest/gtest.h>
#include <PPU.h>
#include <ostream> 

namespace PPUTests {
	class PPUColorIndexTest : public testing::Test {
	protected:
		PPUColorIndexTest() {}

		~PPUColorIndexTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).;
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		PPU ppu;

	};

	TEST_F(PPUColorIndexTest, $00) { //Expected RGB 84,84,84
		RGB color = ppu.getColor(0x00);
		RGB expected = { 84,84,84 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $09) { //Expected RGB 8,58,0
		RGB color = ppu.getColor(0x09);
		RGB expected = { 8,58,0 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $0F) { //Expected RGB 0,0,0
		RGB color = ppu.getColor(0x0F);
		RGB expected = { 0,0,0 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $10) { //Expected RGB 152,150,152
		RGB color = ppu.getColor(0x10);
		RGB expected = { 152,150,152 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $13) { //Expected RGB 92,30,228
		RGB color = ppu.getColor(0x13);
		RGB expected = { 92,30,228 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $1F) { //Expected RGB 0,0,0
		RGB color = ppu.getColor(0x1F);
		RGB expected = { 0,0,0 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $20) { //Expected RGB 236,238,236
		RGB color = ppu.getColor(0x20);
		RGB expected = { 236,238,236 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $25) { //Expected RGB 236,88,180
		RGB color = ppu.getColor(0x25);
		RGB expected = { 236,88,180 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $2F) { //Expected RGB 0,0,0
		RGB color = ppu.getColor(0x2F);
		RGB expected = { 0,0,0 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $30) { //Expected RGB 236,238,236
		RGB color = ppu.getColor(0x30);
		RGB expected = { 236,238,236 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $37) { //Expected RGB 228,196,144
		RGB color = ppu.getColor(0x37);
		RGB expected = { 228,196,144 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	TEST_F(PPUColorIndexTest, $3F) { //Expected RGB 0,0,0
		RGB color = ppu.getColor(0x3F);
		RGB expected = { 0,0,0 };

		ASSERT_EQ(color.r, expected.r);
		ASSERT_EQ(color.g, expected.g);
		ASSERT_EQ(color.b, expected.b);
	}

	class PPUPaletteReadingTest : public ::testing::Test {
	protected:
		PPU ppu;
	};

	// Basic reading/writing
	TEST_F(PPUPaletteReadingTest, PaletteWriteRead) {
		// Write to a background palette entry ($3F01)
		ppu.writePaletteMemory(0x3F01, 0x15);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F01), 0x15);

		// Write to a sprite palette entry ($3F17)
		ppu.writePaletteMemory(0x3F17, 0x2A);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F17), 0x2A);

		// Overwrite a value
		ppu.writePaletteMemory(0x3F01, 0x01);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F01), 0x01);
	}

	// Test case to verify palette mirroring behavior
	TEST_F(PPUPaletteReadingTest, PaletteMirroring) {
		// Test mirroring $3F00 <-> $3F10
		ppu.writePaletteMemory(0x3F00, 0x01); 
		EXPECT_EQ(ppu.readPaletteMemory(0x3F00), 0x01);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F10), 0x01);

		// Write to the mirror address $3F10
		ppu.writePaletteMemory(0x3F10, 0x11);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F00), 0x11);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F10), 0x11);

		// Test mirroring $3F04 <-> $3F14
		ppu.writePaletteMemory(0x3F04, 0x02);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F14), 0x02);
		ppu.writePaletteMemory(0x3F14, 0x12);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F04), 0x12);

		// Test mirroring $3F08 <-> $3F18
		ppu.writePaletteMemory(0x3F08, 0x03); 
		EXPECT_EQ(ppu.readPaletteMemory(0x3F18), 0x03);
		ppu.writePaletteMemory(0x3F18, 0x13);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F08), 0x13);

		// Test mirroring $3F0C <-> $3F1C
		ppu.writePaletteMemory(0x3F0C, 0x04);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F1C), 0x04);
		ppu.writePaletteMemory(0x3F1C, 0x14);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F0C), 0x14);
	}

	// Test case to verify data masking (writes should be masked to 6 bits)
	TEST_F(PPUPaletteReadingTest, PaletteDataMasking) {
		// Write a value larger than 6 bits
		ppu.writePaletteMemory(0x3F02, 0xFF);
		// Expect the read value to be masked to 6 bits (0x3F = 00111111)
		EXPECT_EQ(ppu.readPaletteMemory(0x3F02), 0x3F);
		
		// Write a value larger than 6 bits
		ppu.writePaletteMemory(0x3F05, 0x5A);
		// Expect the read value to be masked to 6 bits
		EXPECT_EQ(ppu.readPaletteMemory(0x3F05), 0x1A);
	}

	// Test case to verify the initial state of the palette memory
	TEST_F(PPUPaletteReadingTest, PaletteInitialization) {
		// Palette memory should be initialized to 0x0F (Black)
		EXPECT_EQ(ppu.readPaletteMemory(0x3F00), 0x0F) << "Initial value at 0x3F00 is not 0x0F";
		EXPECT_EQ(ppu.readPaletteMemory(0x3F08), 0x0F) << "Initial value at 0x3F08 is not 0x0F";
		EXPECT_EQ(ppu.readPaletteMemory(0x3F11), 0x0F) << "Initial value at 0x3F11 is not 0x0F";
		EXPECT_EQ(ppu.readPaletteMemory(0x3F1F), 0x0F) << "Initial value at 0x3F1F is not 0x0F";

		// Check a mirrored address initial value
		EXPECT_EQ(ppu.readPaletteMemory(0x3F10), 0x0F);
	}

	class PPUIndexTest : public ::testing::Test {
	protected:
		PPU ppu;
	};

	TEST_F(PPUIndexTest, PaletteMaskingLogic) { // Using renamed fixture 'PPUIndexTest'.

		// Test writing 0x40
		ppu.writePaletteMemory(0x3F01, 0x40);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F01), 0x00);

		// Test writing 0x5A
		ppu.writePaletteMemory(0x3F02, 0x5A);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F02), 0x1A);

		// Test writing 0xC5
		ppu.writePaletteMemory(0x3F03, 0xC5);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F03), 0x05);

		// Test writing 0xFF
		ppu.writePaletteMemory(0x3F04, 0xFF);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F04), 0x3F);

		// 5. Test writing 0x3F
		ppu.writePaletteMemory(0x3F05, 0x3F);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F05), 0x3F);

		// 6. Test writing 0x00 (binary 00000000) - Should remain 0x00
		ppu.writePaletteMemory(0x3F06, 0x00);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F06), 0x00);

		// 7. Test writing 0x1B
		ppu.writePaletteMemory(0x3F07, 0x1B);
		EXPECT_EQ(ppu.readPaletteMemory(0x3F07), 0x1B);
	}

	class PPUNameTableTest : public ::testing::Test {
	protected:
		PPU ppu;
	};

	TEST_F(PPUNameTableTest, WriteReadNameTable0Tiles) {
		uint16_t addr = 0x2000; // First byte of NameTable 0
		uint8_t value = 0x42;

		ppu.writeNameTable(addr, value);
		EXPECT_EQ(ppu.readNameTable(addr), value);
	}
	// Test writing and reading to tile section of NameTable 1
	TEST_F(PPUNameTableTest, WriteReadNameTable1Tiles) {
		uint16_t addr = 0x2400; // Start of NameTable 1
		uint8_t value = 0xAA;

		ppu.writeNameTable(addr, value);
		EXPECT_EQ(ppu.readNameTable(addr), value);
	}

	// Test writing and reading to attribute table of NameTable 0
	TEST_F(PPUNameTableTest, WriteReadAttributeTable0) {
		uint16_t addr = 0x23C0; // Attribute table region of NT0
		uint8_t value = 0x55;

		ppu.writeNameTable(addr, value);
		EXPECT_EQ(ppu.readNameTable(addr), value);
	}

	// Test writing and reading to attribute table of NameTable 1
	TEST_F(PPUNameTableTest, WriteReadAttributeTable1) {
		uint16_t addr = 0x27C0; // Attribute table region of NT1
		uint8_t value = 0x99;

		ppu.writeNameTable(addr, value);
		EXPECT_EQ(ppu.readNameTable(addr), value);
	}

	// Test out-of-range address (outside 0x2000-0x2FFF), should return something safe
	TEST_F(PPUNameTableTest, InvalidAddressReturnsDefault) {
		uint16_t addr = 0x3FFF; // Not in NameTable space
		EXPECT_NO_THROW(ppu.readNameTable(addr));
	}
	
	class PPUPatternTableTest : public ::testing::Test {
	protected:
		PPU ppu;
	};

	// Test writing to the pattern table
	TEST_F(PPUPatternTableTest, WritePatternTable_ValidData) {
		// Write data to address 0x1000, which is the start of the left table
		uint16_t address = 0x1000;
		uint8_t data = 0xAB; 

		ppu.writePatternTable(address, data);

		// Check that chrRam at address 0x1000 contains the correct data
		EXPECT_EQ(ppu.chrRam[address], data);

		// Check that the data has been written to the correct position in tilePlaneLow or tilePlaneHigh
		int tableIndex = address / 0x1000;  // 0 -> Left Table
		int tileIndex = (address % 0x1000) / 16;  // Calculate the tile index based on address
		int row = (address % 16) % 8;
		EXPECT_EQ(ppu.tilePlaneLow[tableIndex][tileIndex][row], data);
	}
	
	// Test for valid pattern retrieval
	TEST_F(PPUPatternTableTest, GetPatternTile_ValidTile) {
		// Write low plane for row 0 of tile 1
		uint16_t baseAddr = 0x1000 + (1 * 16); // tile index 1 in right table
		ppu.writePatternTable(baseAddr + 0, 0xAB); // low byte for row 0
		ppu.writePatternTable(baseAddr + 8, 0xCD); // high byte for row 0

		std::array<uint8_t, 64> tile = ppu.getPatternTile(1, 1); // tableIndex 1 (right table), tileIndex 1

		// Now expect correct pixel values for row 0 (first 8 bytes in the tile array)
		EXPECT_EQ(tile[0], ((0xCD >> 7 & 1) << 1) | (0xAB >> 7 & 1));
		EXPECT_EQ(tile[1], ((0xCD >> 6 & 1) << 1) | (0xAB >> 6 & 1));
		EXPECT_EQ(tile[2], ((0xCD >> 5 & 1) << 1) | (0xAB >> 5 & 1));
		EXPECT_EQ(tile[3], ((0xCD >> 4 & 1) << 1) | (0xAB >> 4 & 1));
		EXPECT_EQ(tile[4], ((0xCD >> 3 & 1) << 1) | (0xAB >> 3 & 1));
		EXPECT_EQ(tile[5], ((0xCD >> 2 & 1) << 1) | (0xAB >> 2 & 1));
		EXPECT_EQ(tile[6], ((0xCD >> 1 & 1) << 1) | (0xAB >> 1 & 1));
		EXPECT_EQ(tile[7], ((0xCD >> 0 & 1) << 1) | (0xAB >> 0 & 1));
	}
	
	// Test for invalid table index
	TEST_F(PPUPatternTableTest, GetPatternTile_InvalidTableIndex) {
		EXPECT_THROW(ppu.getPatternTile(2, 0), std::out_of_range);
	}
	
	// Test for invalid tile index
	TEST_F(PPUPatternTableTest, GetPatternTile_InvalidTileIndex) {
		EXPECT_THROW(ppu.getPatternTile(0, 300), std::out_of_range);  // Tile index exceeds 256
	}
	
}