#include <gtest/gtest.h>
#include <PPU.h>

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
}