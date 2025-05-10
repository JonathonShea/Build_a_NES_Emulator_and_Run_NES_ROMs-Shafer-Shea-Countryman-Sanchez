#include <gtest/gtest.h>
#include <PPU.h>
#include <OAM.h>
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


	// CPUWrite Tests
	class PPURegisterTest : public ::testing::Test {
	protected:
		void SetUp() override {
			ppu = std::make_unique<PPU>();
			oam = std::make_shared<OAM>();
			ppu->SetOam(oam);
		}

		std::unique_ptr<PPU> ppu;
		std::shared_ptr<OAM> oam;
	};

	// Test PPUCTRL register (0x2000)
	TEST_F(PPURegisterTest, PPUCTRLRegister) {
		// Write to PPUCTRL
		ppu->cpuWrite(0x2000, 0x8A);
		EXPECT_EQ(ppu->PPUCTRL, 0x8A);
	}

	// Test PPUMASK register (0x2001)
	TEST_F(PPURegisterTest, PPUMASKRegister) {
		// Write to PPUMASK
		ppu->cpuWrite(0x2001, 0x1E);
		EXPECT_EQ(ppu->PPUMASK, 0x1E);
	}

	// Test OAMADDR register (0x2003)
	TEST_F(PPURegisterTest, OAMADDRRegister) {
		// Write to OAMADDR
		ppu->cpuWrite(0x2003, 0x42);
		EXPECT_EQ(ppu->OAMADDR, 0x42);
	}

	// Test OAMDATA register (0x2004)
	TEST_F(PPURegisterTest, OAMDATARegister) {
		ppu->cpuWrite(0x2003, 0x00);

		// Write values to the first sprite via OAMDATA
		ppu->cpuWrite(0x2004, 0x40);  // Update Y
		ppu->cpuWrite(0x2004, 0x05);  // Update tile INdex
		ppu->cpuWrite(0x2004, 0x01);  // Update attributes
		ppu->cpuWrite(0x2004, 0x80);  // Update X

		// Verify the sprite data was written correctly
		EXPECT_EQ(static_cast<int8_t>(0x40), oam->sprites[0].y_pos);
		EXPECT_EQ(static_cast<int8_t>(0x05), oam->sprites[0].tile_index);
		EXPECT_EQ(static_cast<int8_t>(0x01), oam->sprites[0].attributes);
		EXPECT_EQ(static_cast<int8_t>(0x80), oam->sprites[0].x_pos);
	}

	// Test OAMDATA register address wrapping
	TEST_F(PPURegisterTest, OAMDATAAddressWrapping) {
		ppu->cpuWrite(0x2003, 0xFC);

		// Write values across the boundary
		ppu->cpuWrite(0x2004, 0xA0);  // Byte 0xFC
		ppu->cpuWrite(0x2004, 0xA1);  // Byte 0xFD
		ppu->cpuWrite(0x2004, 0xA2);  // Byte 0xFE
		ppu->cpuWrite(0x2004, 0xA3);  // Byte 0xFF
		ppu->cpuWrite(0x2004, 0xA4);  // Should wrap to 0x00

		// Check values at the end of OAM
		int lastIndex = oamSize - 1;

		// Compare as int8_t (signed comparison)
		EXPECT_EQ(static_cast<int8_t>(0xA0), oam->sprites[lastIndex].y_pos);
		EXPECT_EQ(static_cast<int8_t>(0xA1), oam->sprites[lastIndex].tile_index);
		EXPECT_EQ(static_cast<int8_t>(0xA2), oam->sprites[lastIndex].attributes);
		EXPECT_EQ(static_cast<int8_t>(0xA3), oam->sprites[lastIndex].x_pos);

		// Compare unsigned values (cast to uint8_t)
		EXPECT_EQ(0xA0, static_cast<uint8_t>(oam->sprites[lastIndex].y_pos));

		// Check the value that wrapped to 0x00 overwrote the first byte of sprites[0]
		EXPECT_EQ(static_cast<int8_t>(0xA4), oam->sprites[0].y_pos);

		// Verify OAMADDR has wrapped around to 0x01
		EXPECT_EQ(0x01, ppu->OAMADDR);
	}

	// Test PPUSCROLL register (0x2005)
	TEST_F(PPURegisterTest, PPUSCROLLRegister) {
		ppu->cpuWrite(0x2005, 0x42);  // First write (X scroll)
		ppu->cpuWrite(0x2005, 0x37);  // Second write (Y scroll)
		ppu->cpuWrite(0x2005, 0x42);  // First write (X scroll)
		ppu->cpuWrite(0x2005, 0x37);  // Second write (Y scroll)
		EXPECT_EQ(ppu->scroll_x, 0x42);
		EXPECT_EQ(ppu->scroll_y, 0x37);
		ppu->cpuWrite(0x2005, 0x55);  // Third write should set X scroll again
		ppu->cpuWrite(0x2005, 0x66);  // Fourth write should set Y scroll again
		EXPECT_EQ(ppu->scroll_x, 0x55);
		EXPECT_EQ(ppu->scroll_y, 0x66);
	}

	// Test PPUADDR and PPUDATA registers (0x2006, 0x2007)
	TEST_F(PPURegisterTest, PPUADDRAndPPUDATARegisters) {
		// Write to the palette memory using PPUADDR/PPUDATA

		// Set the PPUADDR to 0x3F01 (first palette color)
		ppu->cpuWrite(0x2006, 0x3F);  // High byte
		ppu->cpuWrite(0x2006, 0x01);  // Low byte

		// Write some palette colors
		ppu->cpuWrite(0x2007, 0x30);  // White
		ppu->cpuWrite(0x2007, 0x16);  // Red
		ppu->cpuWrite(0x2007, 0x2A);  // Green
		ppu->cpuWrite(0x2007, 0x12);  // Blue

		// Verify the palette memory was updated
		EXPECT_EQ(ppu->readPaletteMemory(0x3F01), 0x30);
		EXPECT_EQ(ppu->readPaletteMemory(0x3F02), 0x16);
		EXPECT_EQ(ppu->readPaletteMemory(0x3F03), 0x2A);
		EXPECT_EQ(ppu->readPaletteMemory(0x3F04), 0x12);

		// Test VRAM address increments by testing writing to nametable

		// Set the PPUADDR to 0x2000 (start of nametable 0)
		ppu->cpuWrite(0x2006, 0x20);  // High byte
		ppu->cpuWrite(0x2006, 0x00);  // Low byte

		// Write some tile indices
		ppu->cpuWrite(0x2007, 0x01);
		ppu->cpuWrite(0x2007, 0x02);
		ppu->cpuWrite(0x2007, 0x03);

		// Verify the nametable memory was updated
		EXPECT_EQ(ppu->readNameTable(0x2000), 0x01);
		EXPECT_EQ(ppu->readNameTable(0x2001), 0x02);
		EXPECT_EQ(ppu->readNameTable(0x2002), 0x03);

		// Test VRAM address increment mode (PPUCTRL bit 2)

		// Set increment mode to 32 (PPUCTRL bit 2 = 1)
		ppu->cpuWrite(0x2000, 0x04);

		// Set the PPUADDR to 0x2000 again
		ppu->cpuWrite(0x2006, 0x20);  // High byte
		ppu->cpuWrite(0x2006, 0x00);  // Low byte

		// Write with 32-byte increment
		ppu->cpuWrite(0x2007, 0x10);
		ppu->cpuWrite(0x2007, 0x20);

		// Verify the nametable memory was updated with 32-byte increments
		EXPECT_EQ(ppu->readNameTable(0x2000), 0x10);
		EXPECT_EQ(ppu->readNameTable(0x2020), 0x20);
	}
}