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
		cpu.setCarryFlag(true);
		cpu.SBC(0x55);
		ASSERT_EQ(cpu.accumulator, 5);
	}

	TEST_F(CPUSBCTest, SBC_simple_sub_carry_set) {
		cpu.write(0x55, 5);
		cpu.setCarryFlag(false);
		cpu.SBC(0x55);
		ASSERT_EQ(cpu.accumulator, 4);
	}

	TEST_F(CPUSBCTest, SBC_carry_bit) {
		cpu.write(0x56, 0xFE);
		cpu.SBC(0x56);
		cpu.write(0x55, 0x02);
		cpu.SBC(0x55);
		ASSERT_TRUE(cpu.getCarryFlag());
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

	class CPUStackTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
			cpu.accumulator = 0x00;
			cpu.stack_pointer = 0xFF;
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUStackTest, stack_PHA) {
		cpu.accumulator = 0x50;
		cpu.PHA();
		uint8_t value = cpu.getStackTESTING().back();

		ASSERT_EQ(cpu.accumulator, value);
		ASSERT_EQ(cpu.stack_pointer, 0xFE);

	}

	TEST_F(CPUStackTest, stack_PLA_StackSize_1) {
		uint8_t value = 0x50;
		cpu.setStackBackTESTING(value);
		ASSERT_EQ(cpu.stack_pointer, 0xFE); 
		cpu.PLA();

		ASSERT_EQ(cpu.accumulator, value);
		ASSERT_EQ(cpu.stack_pointer, 0xFF);
	}

	TEST_F(CPUStackTest, stack_PLA_StackSize_EMPTY) {
		cpu.PLA();
		
		ASSERT_EQ(cpu.accumulator, 0x00);
		ASSERT_EQ(cpu.stack_pointer, 0xFF);
	}

	TEST_F(CPUStackTest, stack_PHP) {
		cpu.status = 0x88;
		EXPECT_FALSE(cpu.getBreakCommandFlag());
		cpu.PHP();
		
		uint8_t value = cpu.getStackTESTING().back();
		ASSERT_EQ(cpu.status, value);
		ASSERT_EQ(cpu.stack_pointer, 0xFE);
		ASSERT_TRUE(cpu.getBreakCommandFlag());
	}

	TEST_F(CPUStackTest, stack_PLP_StackSize_1) {
		uint8_t value = 0x88;
		cpu.setStackBackTESTING(value);
		EXPECT_EQ(cpu.stack_pointer, 0xFE);
		cpu.PLP();

		ASSERT_EQ(cpu.status, value);
		ASSERT_EQ(cpu.stack_pointer, 0xFF);
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_TRUE(cpu.getDecimalModeFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUStackTest, stack_PLP_StackSize_EMPTY) {
		cpu.PLP();

		ASSERT_EQ(cpu.status, 0x00);
		ASSERT_EQ(cpu.stack_pointer, 0xFF);
	}

	TEST_F(CPUStackTest, stack_TXS_non_zero) {
		cpu.x = 0x50;
		cpu.TXS();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_EQ(cpu.status, 0x00);
	}

	TEST_F(CPUStackTest, stack_TXS_upper_boundary) {
		cpu.x = 0xFF;
		cpu.TXS();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_EQ(cpu.status, 0x00);
	}

	TEST_F(CPUStackTest, stack_TXS_lower_boundary) {
		cpu.x = 0x00;
		cpu.TXS();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_EQ(cpu.status, 0x00);
	}

	TEST_F(CPUStackTest, stack_TSX_non_zero) {
		cpu.stack_pointer = 0x50;
		cpu.TSX();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_FALSE(cpu.getNegativeFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUStackTest, stack_TSX_upper_boundary) {
		cpu.stack_pointer = 0xFF;
		cpu.TSX();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_TRUE(cpu.getNegativeFlag());
		ASSERT_TRUE(cpu.getZeroFlag());
	}

	TEST_F(CPUStackTest, stack_TSX_lower_boundary) {
		cpu.stack_pointer = 0x00;
		cpu.TSX();
		ASSERT_EQ(cpu.stack_pointer, cpu.x);
		ASSERT_FALSE(cpu.getNegativeFlag());
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	class CPUTransferTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUTransferTest, TXA_SetsZeroFlag) 
	{
		cpu.x = 0x00;
		cpu.TXA();
		ASSERT_TRUE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TXA_UnsetsSetZeroFlag) 
	{
		cpu.x = 0x01;
		cpu.TXA();
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TXA_SetsNegativeFlag) 
	{
		cpu.x = 0x80;
		cpu.TXA();
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TXA_UnsetsSetNegativeFlag) {
		cpu.x = 0x7F;
		cpu.TXA();
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TXA_TransfersXToAccumulator) 
	{
		cpu.x = 0x42;
		cpu.TXA();
		ASSERT_EQ(cpu.accumulator, cpu.x);
	}

	TEST_F(CPUTransferTest, TYA_SetsZeroFlag)
	{
		cpu.y = 0x00;
		cpu.TYA();
		ASSERT_TRUE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TYA_UnsetsSetZeroFlag)
	{
		cpu.y = 0x01;
		cpu.TYA();
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TYA_SetsNegativeFlag)
	{
		cpu.y = 0x80;
		cpu.TYA();
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TYA_UnsetsSetNegativeFlag) {
		cpu.y = 0x7F;
		cpu.TYA();
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TYA_TransfersYToAccumulator) {
		cpu.y = 0x42;
		cpu.TYA();
		ASSERT_EQ(cpu.accumulator, cpu.y);
	}

	TEST_F(CPUTransferTest, TAX_SetsZeroFlag)
	{
		cpu.accumulator = 0x00;
		cpu.TAX();
		ASSERT_TRUE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TAX_UnsetsSetZeroFlag)
	{
		cpu.accumulator = 0x01;
		cpu.TAX();
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TAX_SetsNegativeFlag)
	{
		cpu.accumulator = 0x80;
		cpu.TAX();
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TAX_UnsetsSetNegativeFlag) {
		cpu.accumulator = 0x7F;
		cpu.TAX();
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TAX_TransfersAccumulatorToX) {
		cpu.accumulator = 0x42;
		cpu.TAX();
		ASSERT_EQ(cpu.x, cpu.accumulator);
	}

	TEST_F(CPUTransferTest, TAY_SetsZeroFlag)
	{
		cpu.accumulator = 0x00;
		cpu.TAY();
		ASSERT_TRUE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TAY_UnsetsSetZeroFlag)
	{
		cpu.accumulator = 0x01;
		cpu.TAY();
		ASSERT_FALSE(cpu.getZeroFlag());
	}

	TEST_F(CPUTransferTest, TAY_SetsNegativeFlag)
	{
		cpu.accumulator = 0x80;
		cpu.TAY();
		ASSERT_TRUE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TAY_UnsetsSetNegativeFlag) {
		cpu.accumulator = 0x7F;
		cpu.TAY();
		ASSERT_FALSE(cpu.getNegativeFlag());
	}

	TEST_F(CPUTransferTest, TAX_TransfersAccumulatorToY) {
		cpu.accumulator = 0x42;
		cpu.TAY();
		ASSERT_EQ(cpu.y, cpu.accumulator);
	}

	class CPUBranchTest : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUBranchTest, BCC_BranchWithPositiveOffsetCarryClear) {
		cpu.setCarryFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BCC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BCC_BranchWithNegativeOffsetCarryClear) {
		cpu.setCarryFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BCC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BCC_CarrySet) {
		cpu.setCarryFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BCC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BCC_BranchWithWrapAround) {
		cpu.setCarryFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BCC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BCS_BranchWithPositiveOffsetCarrySet) {
		cpu.setCarryFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BCS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BCS_BranchWithNegativeOffsetCarrySet) {
		cpu.setCarryFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BCS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BCS_CarryCleared) {
		cpu.setCarryFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BCS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BCS_BranchWithWrapAround) {
		cpu.setCarryFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BCS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BMI_BranchWithPositiveOffsetNegativeSet) {
		cpu.setNegativeFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BMI(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BMI_BranchWithNegativeOffsetNegativeSet) {
		cpu.setNegativeFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BMI(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BMI_NegativeCleared) {
		cpu.setNegativeFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BMI(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BMI_BranchWithWrapAround) {
		cpu.setNegativeFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BMI(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BPL_BranchWithPositiveOffsetNegativeClear) {
		cpu.setNegativeFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BPL(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BPL_BranchWithNegativeOffsetNegativeClear) {
		cpu.setNegativeFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BPL(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BPL_NegativeSet) {
		cpu.setNegativeFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BPL(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BPL_BranchWithWrapAround) {
		cpu.setNegativeFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BPL(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BVC_BranchWithPositiveOffsetOverflowClear) {
		cpu.setOverflowFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BVC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BVC_BranchWithNegativeOffsetOverflowClear) {
		cpu.setOverflowFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BVC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BVC_OverflowSet) {
		cpu.setOverflowFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BVC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BVC_BranchWithWrapAround) {
		cpu.setOverflowFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BVC(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BVS_BranchWithPositiveOffsetOverflowSet) {
		cpu.setOverflowFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BVS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	TEST_F(CPUBranchTest, BVS_BranchWithNegativeOffsetOverflowSet) {
		cpu.setOverflowFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -3);
		cpu.BVS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0FFF);
	}

	TEST_F(CPUBranchTest, BVS_OverflowCleared) {
		cpu.setOverflowFlag(false);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BVS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUBranchTest, BVS_BranchWithWrapAround) {
		cpu.setOverflowFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, -128);
		cpu.BVS(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x0F82);
	}

	TEST_F(CPUBranchTest, BNE_BrranchWithPositiveOffsetZeroFlagSet) {
		cpu.setOverflowFlag(true);
		cpu.program_counter = 0x1000;
		cpu.write(0x1001, 5);
		cpu.BNE(0x1001);
		ASSERT_EQ(cpu.program_counter, 0x1007);
	}

	class CPUAddressingModes : public ::testing::Test {
	protected:
		void SetUp() override {
			cpu.clearStatus();
		}

		void TearDown() override {
		}

		CPU cpu;
	};

	TEST_F(CPUAddressingModes, ImpliedAddressing) {
		cpu.program_counter = 0x1000;
		uint16_t test_addr = cpu.addr_implied();
		ASSERT_EQ(test_addr, 0);
		ASSERT_EQ(cpu.program_counter, 0x1000);
	}

	TEST_F(CPUAddressingModes, AccumulatorAddressing) {
		cpu.program_counter = 0x1000;
		cpu.accumulator = 0x42;
		uint16_t test_addr = cpu.addr_accumulator();
		ASSERT_EQ(test_addr, 0);
		ASSERT_EQ(cpu.program_counter, 0x1000);
		ASSERT_EQ(cpu.accumulator, 0x42);
	}

	TEST_F(CPUAddressingModes, ImmediateAddressing) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x42);
		uint16_t test_addr = cpu.addr_immediate();
		ASSERT_EQ(test_addr, 0x1000);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0x42);
	}

	TEST_F(CPUAddressingModes, ZeroPageAddressing) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x42);
		cpu.write(0x0042, 0x78);
		uint16_t test_addr = cpu.addr_zero_page();
		ASSERT_EQ(test_addr, 0x42);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0x78);
	}

	TEST_F(CPUAddressingModes, ZeroPageXAddressing) {
		cpu.program_counter = 0x1000;
		cpu.x = 0x10;
		cpu.write(0x1000, 0x42);
		cpu.write(0x0052, 0x78); 
		uint16_t test_addr = cpu.addr_zero_page_x();
		ASSERT_EQ(test_addr, 0x52);  // 0x42 + 0x10 = 0x52
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0x78);
	}

	TEST_F(CPUAddressingModes, ZeroPageXAddressingWraparound) {
		cpu.program_counter = 0x1000;
		cpu.x = 0xFF; 
		cpu.write(0x1000, 0x80); 
		cpu.write(0x007F, 0xAB); 
		uint16_t test_addr = cpu.addr_zero_page_x();
		ASSERT_EQ(test_addr, 0x7F);  // (0x80 + 0xFF) & 0xFF = 0x7F
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xAB);
	}

	TEST_F(CPUAddressingModes, ZeroPageYAddressing) {
		cpu.program_counter = 0x1000;
		cpu.y = 0x15;
		cpu.write(0x1000, 0x40);
		cpu.write(0x0055, 0x99);
		uint16_t test_addr = cpu.addr_zero_page_y();
		ASSERT_EQ(test_addr, 0x55);  // 0x40 + 0x15 = 0x55
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0x99);
	}

	TEST_F(CPUAddressingModes, ZeroPageYAddressingWraparound) {
		cpu.program_counter = 0x1000;
		cpu.y = 0xF0; 
		cpu.write(0x1000, 0x90);
		cpu.write(0x0080, 0xCD);
		uint16_t test_addr = cpu.addr_zero_page_y();
		ASSERT_EQ(test_addr, 0x80);  // (0x90 + 0xF0) & 0xFF = 0x80
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xCD);
	}

	TEST_F(CPUAddressingModes, AbsoluteAddressing) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x34);  
		cpu.write(0x1001, 0x12);  
		cpu.write(0x1234, 0xAB);  
		uint16_t test_addr = cpu.addr_absolute();
		ASSERT_EQ(test_addr, 0x1234);
		ASSERT_EQ(cpu.program_counter, 0x1002);
		ASSERT_EQ(cpu.read(test_addr), 0xAB);
	}

	TEST_F(CPUAddressingModes, AbsoluteXAddressing) {
		cpu.program_counter = 0x1000;
		cpu.x = 0x10; 
		cpu.write(0x1000, 0x34);  // Low byte of address
		cpu.write(0x1001, 0x12);  // High byte of address
		cpu.write(0x1244, 0xCD); 
		uint16_t test_addr = cpu.addr_absolute_x();
		ASSERT_EQ(test_addr, 0x1244);  // 0x1234 + 0x10 = 0x1244
		ASSERT_EQ(cpu.program_counter, 0x1002);
		ASSERT_EQ(cpu.read(test_addr), 0xCD);
	}

	TEST_F(CPUAddressingModes, AbsoluteXAddressingPageCrossing) {
		cpu.program_counter = 0x1000;
		cpu.x = 0xD0;
		cpu.write(0x1000, 0x50);  // Low byte of address
		cpu.write(0x1001, 0x12);  // High byte of address
		cpu.write(0x1320, 0xEF); 
		uint16_t test_addr = cpu.addr_absolute_x();
		ASSERT_EQ(test_addr, 0x1320);  // 0x1250 + 0xD0 = 0x1320
		ASSERT_EQ(cpu.program_counter, 0x1002);
		ASSERT_EQ(cpu.read(test_addr), 0xEF);
	}

	TEST_F(CPUAddressingModes, AbsoluteYAddressing) {
		cpu.program_counter = 0x1000;
		cpu.y = 0x20;  // Set Y register
		cpu.write(0x1000, 0x34);  // Low byte of address
		cpu.write(0x1001, 0x12);  // High byte of address
		cpu.write(0x1254, 0xDD);
		uint16_t test_addr = cpu.addr_absolute_y();
		ASSERT_EQ(test_addr, 0x1254);  // 0x1234 + 0x20 = 0x1254
		ASSERT_EQ(cpu.program_counter, 0x1002);
		ASSERT_EQ(cpu.read(test_addr), 0xDD);
	}

	TEST_F(CPUAddressingModes, AbsoluteYAddressingPageCrossing) {
		cpu.program_counter = 0x1000;
		cpu.y = 0xCC;  
		cpu.write(0x1000, 0x40);  // Low byte of address
		cpu.write(0x1001, 0x12);  // High byte of address
		cpu.write(0x130C, 0xBB);  
		uint16_t test_addr = cpu.addr_absolute_y();
		ASSERT_EQ(test_addr, 0x130C);  // 0x1240 + 0xCC = 0x130C
		ASSERT_EQ(cpu.program_counter, 0x1002);
		ASSERT_EQ(cpu.read(test_addr), 0xBB);
	}

	TEST_F(CPUAddressingModes, IndirectAddressing) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x34);  // Low byte of pointer address
		cpu.write(0x1001, 0x12);  // High byte of pointer address
		cpu.write(0x1234, 0x78);  // Low byte of target address
		cpu.write(0x1235, 0x56);  // High byte of target address
		uint16_t test_addr = cpu.addr_indirect();
		ASSERT_EQ(test_addr, 0x5678);  // The address read from 0x1234-0x1235
		ASSERT_EQ(cpu.program_counter, 0x1002);
	}

	TEST_F(CPUAddressingModes, IndirectAddressingPageBoundaryBug) {
		// The high byte is fetched from the same page, not the next page
		// So instead of reading from 0x12FF and 0x1300, it reads from 0x12FF and 0x1200
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0xFF);  // Low byte of pointer address 
		cpu.write(0x1001, 0x12);  // High byte of pointer address
		cpu.write(0x12FF, 0x78);  // Low byte at the correct address
		cpu.write(0x1200, 0x56);  // High byte at the buggy address (should be 0x1300)
		uint16_t test_addr = cpu.addr_indirect();
		ASSERT_EQ(test_addr, 0x5678);
		ASSERT_EQ(cpu.program_counter, 0x1002);
	}

	TEST_F(CPUAddressingModes, IndexedIndirectXAddressing) {
		cpu.program_counter = 0x1000;
		cpu.x = 0x10; 
		cpu.write(0x1000, 0x20);
		cpu.write(0x0030, 0x78);  // Low byte of target
		cpu.write(0x0031, 0x56);  // High byte of target
		cpu.write(0x5678, 0xBD);
		uint16_t test_addr = cpu.addr_indexed_indirect_x();
		ASSERT_EQ(test_addr, 0x5678);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xBD);
	}

	TEST_F(CPUAddressingModes, IndexedIndirectXAddressingWraparound) {
		cpu.program_counter = 0x1000;
		cpu.x = 0xFF;  
		cpu.write(0x1000, 0x80);  
		cpu.write(0x007F, 0x34);  // Low byte of target
		cpu.write(0x0080, 0x12);  // High byte of target
		cpu.write(0x1234, 0xAC);
		uint16_t test_addr = cpu.addr_indexed_indirect_x();
		ASSERT_EQ(test_addr, 0x1234);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xAC);
	}

	TEST_F(CPUAddressingModes, IndirectIndexedYAddressing) {
		cpu.program_counter = 0x1000;
		cpu.y = 0x10;
		cpu.write(0x1000, 0x40);
		cpu.write(0x0040, 0x78);  // Low byte of base address
		cpu.write(0x0041, 0x56);  // High byte of base address
		cpu.write(0x5688, 0xEE);
		uint16_t test_addr = cpu.addr_indirect_indexed_y();
		ASSERT_EQ(test_addr, 0x5688);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xEE);
	}

	TEST_F(CPUAddressingModes, IndirectIndexedYAddressingPageCrossing) {
		cpu.program_counter = 0x1000;
		cpu.y = 0xCC; 
		cpu.write(0x1000, 0x50);  
		cpu.write(0x0050, 0x40);  // Low byte of base address
		cpu.write(0x0051, 0x56);  // High byte of base address
		cpu.write(0x570C, 0xFF);
		uint16_t test_addr = cpu.addr_indirect_indexed_y();
		ASSERT_EQ(test_addr, 0x570C);
		ASSERT_EQ(cpu.program_counter, 0x1001);
		ASSERT_EQ(cpu.read(test_addr), 0xFF);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingForwardBranch) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x0A);  // Positive offset: 10
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x100B);// 0x1001 + 10 = 0x100B
		ASSERT_EQ(cpu.program_counter, 0x1001);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingBackwardBranch) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0xF6);  // Negative offset: -10
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x0FF7); // 0x1001 + (-10) = 0x0FF7
		ASSERT_EQ(cpu.program_counter, 0x1001);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingMaximumPositiveOffset) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x7F);
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x1080); // 0x1001 + 127 = 0x1080
		ASSERT_EQ(cpu.program_counter, 0x1001);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingMaximumNegativeOffset) {
		cpu.program_counter = 0x1000;
		cpu.write(0x1000, 0x80);
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x0F81); // 0x1001 + (-128) = 0x0F81
		ASSERT_EQ(cpu.program_counter, 0x1001);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingPageCrossingForward) {
		cpu.program_counter = 0x10F8;
		cpu.write(0x10F8, 0x0A);
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x1103); // 0x10F9 + 10 = 0x1103
		ASSERT_EQ(cpu.program_counter, 0x10F9);
	}

	TEST_F(CPUAddressingModes, RelativeAddressingPageCrossingBackward) {
		cpu.program_counter = 0x1004;
		cpu.write(0x1004, 0xF6);  
		uint16_t test_addr = cpu.addr_relative();
		ASSERT_EQ(test_addr, 0x0FFB); // 0x1005 + (-10) = 0x0FFB
		ASSERT_EQ(cpu.program_counter, 0x1005);
	}

}