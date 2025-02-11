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

	class CPUBitwiseTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUBitwiseTest, bitwise_and_zero_zero) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.AND(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_and_none_alike) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0xFF);
		cpu.AND(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_and_all_alike) {
		cpu.accumulator = 0xFF;
		cpu.write(0x1000, 0xFF);
		cpu.AND(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_and_shared_neg_bit) {
		cpu.accumulator = 0xF2;
		cpu.write(0x1000, 0x81);
		cpu.AND(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x80);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_or_all_zero) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.ORA(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_or_all_non_zero) {
		cpu.accumulator = 0xFF;
		cpu.write(0x1000, 0xFF);
		cpu.ORA(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_or_none_alike) {
		cpu.accumulator = 0xAA;
		cpu.write(0x1000, 0x55);
		cpu.ORA(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_or_alternate_bit_result) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x55);
		cpu.ORA(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x55);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_eor_zeros) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.EOR(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_eor_all_non_zero) {
		cpu.accumulator = 0xFF;
		cpu.write(0x1000, 0xFF);
		cpu.EOR(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_eor_none_alike) {
		cpu.accumulator = 0xAA;
		cpu.write(0x1000, 0x55);
		cpu.EOR(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_eor_some_alike) {
		cpu.accumulator = 0xAA;
		cpu.write(0x1000, 0x5B);
		cpu.EOR(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xF1);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_bit_zero_zero) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.BIT(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getOverFlowFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_bit_none_alike_neg_value) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0xFF);
		cpu.BIT(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getOverFlowFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_bit_none_alike_neg_accumulator) {
		cpu.accumulator = 0xFF;
		cpu.write(0x1000, 0x00);
		cpu.BIT(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getOverFlowFlag());
		ASSERT_FALSE(cpu.getOverFlowFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_bit_all_alike) {
		cpu.accumulator = 0xFF;
		cpu.write(0x1000, 0xFF);
		cpu.BIT(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xFF);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getOverFlowFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUBitwiseTest, bitwise_bit_shared_neg_bit) {
		cpu.accumulator = 0xF2;
		cpu.write(0x1000, 0x81);
		cpu.BIT(0x1000);
		ASSERT_EQ(cpu.accumulator, 0xF2);
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getOverFlowFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	class CPUCompareTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUCompareTest, compare_cmp_zeros_a_equal_m) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.CMP(0x1000);
		
		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cmp_a_less_m) {
		cpu.accumulator = 0x40;
		cpu.write(0x1000, 0x80);
		cpu.CMP(0x1000);

		ASSERT_FALSE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cmp_a_greater_m) {
		cpu.accumulator = 0x80;
		cpu.write(0x1000, 0x40);
		cpu.CMP(0x1000);

		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpx_zeros_x_equal_m) {
		cpu.x = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.CPX(0x1000);

		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpx_x_less_m) {
		cpu.x = 0x40;
		cpu.write(0x1000, 0x80);
		cpu.CPX(0x1000);

		ASSERT_FALSE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpx_x_greater_m) {
		cpu.x = 0x80;
		cpu.write(0x1000, 0x40);
		cpu.CPX(0x1000);

		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpy_zeros_y_equal_m) {
		cpu.y = 0x00;
		cpu.write(0x1000, 0x00);
		cpu.CPY(0x1000);

		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_TRUE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpy_y_less_m) {
		cpu.y = 0x40;
		cpu.write(0x1000, 0x80);
		cpu.CPY(0x1000);

		ASSERT_FALSE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUCompareTest, compare_cpy_y_greater_m) {
		cpu.y = 0x80;
		cpu.write(0x1000, 0x40);
		cpu.CPY(0x1000);

		ASSERT_TRUE(cpu.getCarryFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
		ASSERT_FALSE(cpu.getNegativeFlag());
	}




	class CPUJMPTest : public testing::Test {
	protected:
		CPUJMPTest() {}

		~CPUJMPTest() override {
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

	TEST_F(CPUJMPTest, JMP_ABS) {
		uint8_t val = 0x05;
		cpu.write(0x01, val);
		cpu.JMP_ABS(0x01);
		ASSERT_EQ(val, cpu.program_counter);
	}

	TEST_F(CPUJMPTest, JMP_ABS_two_bytes) {
		uint8_t val1 = 0xFC;
		uint8_t val2 = 0x15;
		uint8_t val3 = 0xFC15;
		cpu.write(0x01, val1);
		cpu.write(0x01, val2);
		cpu.JMP_ABS(0x01);
		ASSERT_EQ(val3, cpu.program_counter);
	}

	TEST_F(CPUJMPTest, JMP_IND) {
		uint8_t addr = 0x05;
		uint8_t val = 0xFC;
		cpu.write(0x01, addr);
		cpu.write(addr, val);
		cpu.JMP_IND(0x01);
		ASSERT_EQ(val, cpu.program_counter);
	}

	TEST_F(CPUJMPTest, JMP_IND_bug) {
		uint8_t addr = 0x05;
		uint8_t val = 0xFF;
		cpu.write(0x01, addr);
		cpu.write(addr, val);
		cpu.JMP_IND(0x01);
		ASSERT_NE(val, cpu.program_counter);
		ASSERT_EQ(0, cpu.program_counter);

	}

	class CPUAccessTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUAccessTest, access_LDA_assign) {
		cpu.accumulator = 0x00;
		cpu.write(0x1000, 0x50);
		cpu.LDA(0x1000);
		ASSERT_EQ(cpu.accumulator,0x50);
	}

	TEST_F(CPUAccessTest, access_LDA_assign_empty_address) {
		cpu.accumulator = 0x00;
		cpu.LDA(0x1000);
		ASSERT_EQ(cpu.accumulator, 0x00);
	}

	TEST_F(CPUAccessTest, access_STA_assign) {
		cpu.accumulator = 0x50;
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STA(addr);
		ASSERT_EQ(cpu.accumulator, cpu.read(addr));
	}

	TEST_F(CPUAccessTest, access_STA_assign_empty_accumulator) {
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STA(addr);
		ASSERT_EQ(0x00, cpu.read(addr));
	}

	TEST_F(CPUAccessTest, access_LDX_assign) {
		cpu.x = 0x00;
		cpu.write(0x1000, 0x50);
		cpu.LDX(0x1000);
		ASSERT_EQ(cpu.x, 0x50);
	}

	TEST_F(CPUAccessTest, access_LDX_assign_empty_address) {
		cpu.x = 0x00;
		cpu.LDX(0x1000);
		ASSERT_EQ(cpu.x, 0x00);
	}

	TEST_F(CPUAccessTest, access_STX_assign) {
		cpu.x = 0x50;
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STX(addr);
		ASSERT_EQ(cpu.x, cpu.read(addr));
	}

	TEST_F(CPUAccessTest, access_STX_assign_empty_x) {
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STX(addr);
		ASSERT_EQ(0x00, cpu.read(addr));
	}

	TEST_F(CPUAccessTest, access_LDY_assign) {
		cpu.y = 0x00;
		cpu.write(0x1000, 0x50);
		cpu.LDY(0x1000);
		ASSERT_EQ(cpu.y, 0x50);
	}

	TEST_F(CPUAccessTest, access_LDY_assign_empty_address) {
		cpu.y = 0x00;
		cpu.LDY(0x1000);
		ASSERT_EQ(cpu.y, 0x00);
	}

	TEST_F(CPUAccessTest, access_STY_assign) {
		cpu.y = 0x50;
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STY(addr);
		ASSERT_EQ(cpu.y, cpu.read(addr));
	}

	TEST_F(CPUAccessTest, access_STY_assign_empty_y) {
		uint16_t addr = 0x1000;
		cpu.write(addr, 0x00);
		cpu.STY(addr);
		ASSERT_EQ(0x00, cpu.read(addr));
	}

}