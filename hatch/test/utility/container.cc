#include <hatch/utility/container.hh>
#include <gtest/gtest.h>

#include <cstdint>

namespace hatch {

  class ContainerTest : public ::testing::Test {
  protected:
    class test_data {
    public:
      test_data(const uint64_t& left, const uint64_t& right) : value{left * right} {
      }

      const uint64_t value;
    };

    class test_inheriting : public container<test_inheriting> {
    public:
      test_inheriting(const uint64_t& number) : value{number} {
      }

      const uint64_t value;
    };

    using test_aggregating = container<test_data>;

    std::aligned_storage_t<sizeof(test_inheriting)> inherits_raw;
    test_inheriting* inherits_ptr;

    std::aligned_storage_t<sizeof(test_aggregating), alignof(test_aggregating)> aggregates_raw;
    test_aggregating* aggregates_ptr;
  };

  TEST_F(ContainerTest, CreateInheritsTest) {
    constexpr auto number = 789345;

    auto object = test_inheriting{number};

    EXPECT_EQ(object.get().value, number);
  }

  TEST_F(ContainerTest, CreateAggregatesTest) {
    constexpr auto left = 12;
    constexpr auto right = 5234;
    constexpr auto result = left * right;

    auto object = test_aggregating{left, right};

    EXPECT_EQ(object.get().value, result);
  }

  TEST_F(ContainerTest, PlacementNewInheritsTest) {
    constexpr uint64_t number = 1;

    inherits_ptr = new (&inherits_raw) test_inheriting{number};

    EXPECT_EQ(inherits_ptr->get().value, number);
  }

  TEST_F(ContainerTest, PlacementNewAggregatesTest) {
    constexpr auto left = 12;
    constexpr auto right = 5234;
    constexpr auto result = left * right;

    aggregates_ptr = new (&aggregates_raw) test_aggregating{left, right};

    EXPECT_EQ(aggregates_ptr->get().value, result);
  }

}