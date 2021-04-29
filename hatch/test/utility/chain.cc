#include <hatch/utility/chain.hh>
#include <gtest/gtest.h>

#include <cstdint>

namespace hatch {

  class ChainTest : public ::testing::Test {
  protected:
    class test_chain : public chain<test_chain> {
    public:
      test_chain(uint16_t number) : value{number} {
      }

      test_chain(test_chain&& moved) noexcept :
          chain<test_chain>(std::move(moved)),
          value{moved.value} {
        moved.value = 0;
      }

      test_chain& operator=(test_chain&& moved) noexcept {
        chain<test_chain>::operator=(std::move(moved));
        value = moved.value;
        moved.value = 0;
        return *this;
      }

      void splice(test_chain& link) {
        chain<test_chain>::splice(link);
      }

      uint16_t value;
    };

    test_chain first{1};
    test_chain second{2};
    test_chain third{3};
    test_chain fourth{4};
    test_chain fifth{5};
    test_chain sixth{6};
  };

  TEST_F(ChainTest, EmptyTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());
    EXPECT_TRUE(fourth.alone());
    EXPECT_TRUE(fifth.alone());
    EXPECT_TRUE(sixth.alone());
  }

  TEST_F(ChainTest, SimpleSpliceTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());

    second.splice(third);

    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &second);
    EXPECT_EQ(&second.prev(), &third);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_TRUE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    first.splice(second);

    EXPECT_EQ(&first.next(), &second);
    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&second.prev(), &first);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    second.splice(second.next());

    EXPECT_EQ(&first.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&third.prev(), &first);

    EXPECT_FALSE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(third.alone());

    first.splice(first);

    EXPECT_EQ(&first.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&third.prev(), &first);

    EXPECT_FALSE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_FALSE(third.alone());
  }

  TEST_F(ChainTest, MultipleSpliceTest) {
    EXPECT_TRUE(first.alone());
    EXPECT_TRUE(second.alone());
    EXPECT_TRUE(third.alone());

    EXPECT_TRUE(fourth.alone());
    EXPECT_TRUE(fifth.alone());
    EXPECT_TRUE(sixth.alone());

    second.splice(third);
    first.splice(second);

    fifth.splice(sixth);
    fourth.splice(fifth);

    EXPECT_FALSE(first.alone());
    EXPECT_FALSE(second.alone());
    EXPECT_FALSE(third.alone());

    EXPECT_FALSE(fourth.alone());
    EXPECT_FALSE(fifth.alone());
    EXPECT_FALSE(sixth.alone());

    second.splice(fifth);

    EXPECT_EQ(&fourth.next(), &second);
    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&third.next(), &first);
    EXPECT_EQ(&first.next(), &fifth);
    EXPECT_EQ(&fifth.next(), &sixth);
    EXPECT_EQ(&sixth.next(), &fourth);
    EXPECT_EQ(&fourth.prev(), &sixth);
    EXPECT_EQ(&second.prev(), &fourth);
    EXPECT_EQ(&third.prev(), &second);
    EXPECT_EQ(&first.prev(), &third);
    EXPECT_EQ(&fifth.prev(), &first);
    EXPECT_EQ(&sixth.prev(), &fifth);
  }

  TEST_F(ChainTest, MoveConstructorTest) {
    first.splice(second);
    first.splice(third);
    first.splice(fourth);

    EXPECT_EQ(&first.next(), &second);
    EXPECT_EQ(&first.prev(), &fourth);

    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&second.prev(), &first);

    EXPECT_EQ(&third.next(), &fourth);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_EQ(&fourth.next(), &first);
    EXPECT_EQ(&fourth.prev(), &third);
    
    auto node = test_chain{std::move(second)};

    EXPECT_EQ(&first.next(), &node);
    EXPECT_EQ(&first.prev(), &fourth);

    EXPECT_EQ(&node.next(), &third);
    EXPECT_EQ(&node.prev(), &first);

    EXPECT_EQ(&third.next(), &fourth);
    EXPECT_EQ(&third.prev(), &node);

    EXPECT_EQ(&fourth.next(), &first);
    EXPECT_EQ(&fourth.prev(), &third);

    EXPECT_EQ(&second.next(), &second);
    EXPECT_EQ(&second.prev(), &second);
  }

  TEST_F(ChainTest, MoveAssignmentTest) {
    first.splice(second);
    first.splice(third);
    first.splice(fourth);

    EXPECT_EQ(&first.next(), &second);
    EXPECT_EQ(&first.prev(), &fourth);

    EXPECT_EQ(&second.next(), &third);
    EXPECT_EQ(&second.prev(), &first);

    EXPECT_EQ(&third.next(), &fourth);
    EXPECT_EQ(&third.prev(), &second);

    EXPECT_EQ(&fourth.next(), &first);
    EXPECT_EQ(&fourth.prev(), &third);

    fifth = std::move(second);

    EXPECT_EQ(&first.next(), &fifth);
    EXPECT_EQ(&first.prev(), &fourth);

    EXPECT_EQ(&fifth.next(), &third);
    EXPECT_EQ(&fifth.prev(), &first);

    EXPECT_EQ(&third.next(), &fourth);
    EXPECT_EQ(&third.prev(), &fifth);

    EXPECT_EQ(&fourth.next(), &first);
    EXPECT_EQ(&fourth.prev(), &third);

    EXPECT_EQ(&second.next(), &second);
    EXPECT_EQ(&second.prev(), &second);
  }

}
  
  