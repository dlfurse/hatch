#include <hatch/core/structures.hh>
#include <gtest/gtest.h>

#include <iostream>

namespace hatch {

  class StructuresTest : public ::testing::Test {
  protected:
    class pointer_list_data : public pointer_list<pointer_list_data> {
    public:
      pointer_list_data(unsigned int numerator, unsigned int denominator, double precise) :
          numerator{numerator},
          denominator{denominator},
          precise{precise} {
      }

      unsigned int numerator;
      unsigned int denominator;
      double precise;
    };

  protected:
    void SetUp() override {

    }
  };

  TEST_F(StructuresTest, SimpleListTest) {
    pointer_list_data first{1, 10, 1.1};
    pointer_list_data second{2, 20, 2.2};
    pointer_list_data third{3, 30, 3.3};

    EXPECT_TRUE(first.detached());
    second.insert_after(first);
    EXPECT_FALSE(first.detached());
    third.insert_before(first);

    int count = 0;
    for (auto& data : first) {
      if (count == 0) {
        EXPECT_EQ(data.numerator, 1);
        EXPECT_EQ(data.denominator, 10);
        EXPECT_EQ(data.precise, 1.1);
      } else if (count == 1) {
        EXPECT_EQ(data.numerator, 2);
        EXPECT_EQ(data.denominator, 20);
        EXPECT_EQ(data.precise, 2.2);
      } else if (count == 2) {
        EXPECT_EQ(data.numerator, 3);
        EXPECT_EQ(data.denominator, 30);
        EXPECT_EQ(data.precise, 3.3);
      }
      ++count;
    }
  }

} // namespace hatch

