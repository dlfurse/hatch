#include <hatch/utility/meta.hh>
#include <gtest/gtest.h>

namespace hatch {

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