#include <hatch/utility/meta.hh>
#include <gtest/gtest.h>

#include <type_traits>

namespace hatch {

  TEST(MetaTest, VoidedBattery) {
    const auto a = std::is_same_v<voided_t<int>, void>;
    EXPECT_EQ(a, true);
    class test_type;
    const auto b = std::is_same_v<voided_t<test_type>, void>;
    EXPECT_EQ(b, true);
  }

  TEST(MetaTest, TypedComparison) {
    const auto a = typed_v<4>;
    const auto b = typed_v<false>;
    const auto c = typed_v<false>;
    EXPECT_NE(a, b);
    EXPECT_EQ(b, c);
  }

  TEST(MetaTest, CompleteWithIncompleteType) {
    class test_incomplete;
    EXPECT_FALSE(complete_v<test_incomplete>);
  }

  TEST(MetaTest, CompleteWithCompleteType) {
    class test_complete{
    public:
      int value;
    };
    EXPECT_TRUE(complete_v<test_complete>);
  }

}