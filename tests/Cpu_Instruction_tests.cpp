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

}

class CPUINCTest : public testing::Test {
protected:
	CPUINCTest() {}

	~CPUINCTest() override {
	}

	void SetUp() override {
		// Code here will be called immediately after the constructor (right
		// before each test).

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

TEST_F(CPUINCTest, )