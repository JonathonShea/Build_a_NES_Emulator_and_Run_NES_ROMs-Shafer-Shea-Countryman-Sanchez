#include <gtest/gtest.h>
#include <CPU.h>

namespace ProcessorTests {
	class CPUADCTest : public testing::Test {
	protected:
		CPUADCTest() {}

		~CPUADCTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUADCTest, ADC_simple_add) {
		ASSERT_EQ(cpu.accumulator, 0);
		cpu.write(0x55, 3);
		cpu.ADC(0x55);
		ASSERT_EQ(cpu.read(0x55), 3);
		ASSERT_EQ(cpu.accumulator, 3);
	}

	TEST_F(CPUADCTest, ADC_carry_bit) {
		cpu.write(0x56, 0xFE);
		cpu.ADC(0x56);
		cpu.write(0x55, 0x02);
		cpu.ADC(0x55);
		ASSERT_TRUE(cpu.status & 0x01);
	}

	TEST_F(CPUADCTest, ADC_signed_underflow) {
		EXPECT_FALSE(cpu.getOverFlowFlag());
		cpu.write(0x01, 0x3F);
		cpu.write(0x02, 0x3F);
		cpu.ADC(0x01);
		cpu.ADC(0x02);
		ASSERT_TRUE(cpu.getOverFlowFlag());
	}

	TEST_F(CPUADCTest, ADC_negative) {
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.write(0x01, 0x80);
		cpu.ADC(0x01);
		ASSERT_TRUE(cpu.getNegativeFlag());
	}


	class CPUINCTest : public testing::Test {
	protected:
		CPUINCTest() {}

		~CPUINCTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUINCTest, INC_simple) {
		int val = 5;
		cpu.write(0x00, val);
		cpu.INC(0x00);
		ASSERT_EQ(cpu.read(0x00), val + 1);
	}

	TEST_F(CPUINCTest, INC_negative_flag) {
		int val = 0x7F;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.write(0x00, val);
		cpu.INC(0x00);
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.read(0x00), val + 1);
	}



	class CPUDECTest : public testing::Test {
	protected:
		CPUDECTest() {}

		~CPUDECTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUDECTest, DEC_simple) {
		int val = 5;
		cpu.write(0x00, val);
		cpu.DEC(0x00);
		ASSERT_EQ(cpu.read(0x00), val - 1);
	}

	TEST_F(CPUDECTest, DEC_negative_flag) {
		int val = 0x81;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.write(0x00, val);
		cpu.DEC(0x00);
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.read(0x00), val - 1);
	}



	class CPUINXTest : public testing::Test {
	protected:
		CPUINXTest() {}

		~CPUINXTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();

		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUINXTest, INX_simple) {
		int val = 5;
		cpu.x = val;
		cpu.INX();
		ASSERT_EQ(cpu.x, val + 1);
	}

	TEST_F(CPUINXTest, INX_negative_flag) {
		int val = 0x81;
		cpu.x = val;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.INX();
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.x, val + 1);
	}

	class CPUDEXTest : public testing::Test {
	protected:
		CPUDEXTest() {}

		~CPUDEXTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();

		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUDEXTest, DEX_simple) {
		int val = 5;
		cpu.x = val;
		cpu.DEX();
		ASSERT_EQ(cpu.x, val - 1);
	}

	TEST_F(CPUDEXTest, DEX_negative_flag) {
		int val = 0x81;
		cpu.x = val;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.DEX();
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.x, val - 1);
	}

	class CPUINYTest : public testing::Test {
	protected:
		CPUINYTest() {}

		~CPUINYTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();

		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUINYTest, INY_simple) {
		int val = 5;
		cpu.y = val;
		cpu.INY();
		ASSERT_EQ(cpu.y, val + 1);
	}

	TEST_F(CPUINYTest, INY_negative_flag) {
		int val = 0x81;
		cpu.y = val;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.INY();
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.y, val + 1);
	}

	class CPUDEYTest : public testing::Test {
	protected:
		CPUDEYTest() {}

		~CPUDEYTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();

		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUDEYTest, DEY_simple) {
		int val = 5;
		cpu.y = val;
		cpu.DEY();
		ASSERT_EQ(cpu.y, val - 1);
	}

	TEST_F(CPUDEYTest, DEY_negative_flag) {
		int val = 0x81;
		cpu.y = val;
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.DEY();
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_EQ(cpu.y, val - 1);
	}

	class CPUShiftTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUShiftTest, ASL_Test) {
		uint16_t addr = 0x2000;
		uint8_t value = 0x55;

		cpu.write(addr, value);
		uint8_t beforeShift = cpu.read(addr);

		cpu.ASL(addr);
		uint8_t afterShift = cpu.read(addr);

		ASSERT_EQ(afterShift, 0xAA);
	}

	TEST_F(CPUShiftTest, LSR_Test) {
		uint16_t addr = 0x2000;
		uint8_t value = 0x55;

		cpu.write(addr, value);
		uint8_t beforeShift = cpu.read(addr);

		cpu.LSR(addr);
		uint8_t afterShift = cpu.read(addr);

		ASSERT_EQ(afterShift, 0x2A);
	}

	TEST_F(CPUShiftTest, ROL_Test) {
		uint16_t addr = 0x2000;
		uint8_t value = 0x55;

		cpu.write(addr, value);
		cpu.setCarryFlag(false);
		uint8_t beforeShift = cpu.read(addr);

		cpu.ROL(addr);
		uint8_t afterShift = cpu.read(addr);

		ASSERT_EQ(afterShift, 0xAA);
	}

	TEST_F(CPUShiftTest, ROR_Test) {
		uint16_t addr = 0x2000;
		uint8_t value = 0x55;

		cpu.write(addr, value);
		cpu.setCarryFlag(false);
		uint8_t beforeShift = cpu.read(addr);

		cpu.ROR(addr);
		uint8_t afterShift = cpu.read(addr);

		ASSERT_EQ(afterShift, 0x2A);
	}


	class CPUSBCTest : public testing::Test {
	protected:
		CPUSBCTest() {}

		~CPUSBCTest() override {
		}

		void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
			cpu.clearStatus();
			cpu.accumulator = 10; // test start point
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
		CPU cpu;

	};

	TEST_F(CPUSBCTest, SBC_simple_sub_carry_clear) {
		cpu.write(0x55, 5);
		cpu.SBC(0x55);
		cpu.setCarryFlag(true);
		ASSERT_EQ(cpu.accumulator, 5);
	}

	TEST_F(CPUSBCTest, SBC_simple_sub_carry_set) {
		cpu.write(0x55, 5);
		cpu.SBC(0x55);
		cpu.setCarryFlag(false);
		ASSERT_EQ(cpu.accumulator, 4);
	}

	TEST_F(CPUSBCTest, SBC_carry_bit) {
		cpu.write(0x56, 0xFE);
		cpu.SBC(0x56);
		cpu.write(0x55, 0x02);
		cpu.ADC(0x55);
		ASSERT_TRUE(cpu.status & 0x01);
	}

	TEST_F(CPUSBCTest, SBC_signed_underflow) {
		EXPECT_FALSE(cpu.getOverFlowFlag());
		cpu.write(0x01, 0x3F);
		cpu.write(0x02, 0x3F);
		cpu.SBC(0x01);
		cpu.SBC(0x02);
		ASSERT_TRUE(cpu.getOverFlowFlag());
	}

	TEST_F(CPUSBCTest, SBC_negative) {
		EXPECT_FALSE(cpu.getNegativeFlag());
		cpu.write(0x01, 0x80);
		cpu.SBC(0x01);
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

}