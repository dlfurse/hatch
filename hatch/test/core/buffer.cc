#include <hatch/core/async.hh>
#include <gtest/gtest.h>

namespace hatch {
  class BufferTest : public ::testing::Test {

  };

  TEST_F(BufferTest, NothingTest) {
    auto a = 1;
    auto b = 2;
    b--;
    EXPECT_TRUE(a == b);
  }
}

