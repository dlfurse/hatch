#include <hatch/utility/integral.hh>
#include <gtest/gtest.h>

namespace hatch {

  TEST(IntegralTest, EightShiftsToSixteen) {
    u16 a = unsigned_max_v<widths::bits8>;
    u8 b = unsigned_max_v<widths::bits8>;
    u16 c = a << unsigned_bits_v<widths::bits8> | b;
    EXPECT_EQ(c, unsigned_max_v<widths::bits16>);
  }

  TEST(IntegralTest, SixteenShiftsToThirtyTwo) {
    u32 a = unsigned_max_v<widths::bits16>;
    u16 b = unsigned_max_v<widths::bits16>;
    u32 c = a << unsigned_bits_v<widths::bits16> | b;
    EXPECT_EQ(c, unsigned_max_v<widths::bits32>);
  }

  TEST(IntegralTest, ThirtyTwoShiftsToSixtyFour) {
    u64 a = unsigned_max_v<widths::bits32>;
    u32 b = unsigned_max_v<widths::bits32>;
    u64 c = (a << unsigned_bits_v<widths::bits32>) | b;
    EXPECT_EQ(c, unsigned_max_v<widths::bits64>);
  }

} // namespace hatch