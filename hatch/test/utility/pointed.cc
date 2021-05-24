#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <cstdint>

namespace hatch {

  class PointedTest : public ::testing::Test {
  protected:
    using test_pair = std::pair<int16_t, int16_t>;

    test_pair _data;

    void SetUp() override {
      _data.first = 12;
      _data.second = 987;
    }

  };

  TEST_F(PointedTest, SanityTest) {
    pointed<test_pair> ptr;

    EXPECT_FALSE(ptr);

    ptr = &_data;

    EXPECT_EQ(ptr->first, 12);
    EXPECT_EQ(ptr->second, 987);
  }

}