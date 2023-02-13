#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <climits>
#include <cstdint>

namespace hatch {

  class PointedTest : public ::testing::Test {
  protected:
    using test_raw = uint32_t;
    using test_atom = int16_t;
    using test_pair = std::pair<test_atom, test_atom>;

    static constexpr auto test_count = 24u;

    using test_pointed = pointed<test_pair>;

    test_raw* _data{nullptr};
    std::byte* _address{nullptr};
    std::aligned_storage_t<test_count * sizeof(test_raw), alignof(test_raw)> _memory{};

    void SetUp() override {
      _data = reinterpret_cast<test_raw*>(&_memory);
      _address = reinterpret_cast<std::byte*>(&_memory);
    }
  };

  TEST_F(PointedTest, DefaultNullTest) {
    auto pointed = test_pointed{};

    EXPECT_FALSE(pointed);
  }

  TEST_F(PointedTest, AddressConstructorTest) {
    constexpr uint64_t index = 13u;

    auto pointed = test_pointed{_address + index * sizeof(test_raw)};

    EXPECT_TRUE(pointed);

    constexpr test_atom first = 4987;
    constexpr test_atom second = 1171;

    pointed->first = first;
    pointed->second = second;

    auto expected = (test_raw)(first);
    expected |= (test_raw)(second) << sizeof(test_atom) * CHAR_BIT;

    EXPECT_EQ(_data[index], expected);
  }

  TEST_F(PointedTest, AddressAssignmentTest) {
    constexpr uint64_t index = 3u;

    auto pointed = test_pointed{};
    pointed = _address + index * sizeof(test_raw);

    EXPECT_TRUE(pointed);

    constexpr test_atom first = 33;
    constexpr test_atom second = 749;

    pointed->first = first;
    pointed->second = second;

    auto expected = (test_raw)(first);
    expected |= (test_raw)(second) << sizeof(test_atom) * CHAR_BIT;

    EXPECT_EQ(_data[index], expected);
  }

  TEST_F(PointedTest, MoveConstructorTest) {
    constexpr uint64_t index_one = 19u;

    auto pointed_one = test_pointed{_address + index_one * sizeof(test_raw)};

    EXPECT_TRUE(pointed_one);

    constexpr test_atom first_one = 4987;
    constexpr test_atom second_one = 1171;

    pointed_one->first = first_one;
    pointed_one->second = second_one;

    auto pointed_two = test_pointed{std::move(pointed_one)};

    EXPECT_FALSE(pointed_one);
    EXPECT_TRUE(pointed_two);

    EXPECT_EQ(pointed_two->first, first_one);
    EXPECT_EQ(pointed_two->second, second_one);
  }

  TEST_F(PointedTest, MoveAssignmentTest) {
    constexpr uint64_t index_one = 19u;

    auto pointed_one = test_pointed{_address + index_one * sizeof(test_raw)};

    EXPECT_TRUE(pointed_one);

    constexpr test_atom first_one = 4987;
    constexpr test_atom second_one = 1171;

    pointed_one->first = first_one;
    pointed_one->second = second_one;

    constexpr uint64_t index_two = 8u;

    auto pointed_two = test_pointed{_address + index_two * sizeof(test_raw)};

    EXPECT_TRUE(pointed_two);

    constexpr test_atom first_two = 97;
    constexpr test_atom second_two = 124;

    pointed_two->first = first_two;
    pointed_two->second = second_two;

    pointed_two = std::move(pointed_one);

    EXPECT_FALSE(pointed_one);
    EXPECT_TRUE(pointed_two);

    EXPECT_EQ(pointed_two->first, first_one);
    EXPECT_EQ(pointed_two->second, second_one);

    auto expected = (test_raw)(first_two);
    expected |= (test_raw)(second_two) << sizeof(test_atom) * CHAR_BIT;

    EXPECT_EQ(_data[index_two], expected);
  }

  TEST_F(PointedTest, CopyConstructorTest) {
    constexpr uint64_t index_one = 9u;

    auto pointed_one = test_pointed{_address + index_one * sizeof(test_raw)};

    EXPECT_TRUE(pointed_one);

    constexpr test_atom first_one = 565;
    constexpr test_atom second_one = 3288;

    pointed_one->first = first_one;
    pointed_one->second = second_one;

    auto pointed_two = test_pointed{pointed_one};

    EXPECT_TRUE(pointed_one);
    EXPECT_TRUE(pointed_two);

    EXPECT_EQ(pointed_one->first, first_one);
    EXPECT_EQ(pointed_one->second, second_one);

    EXPECT_EQ(pointed_two->first, first_one);
    EXPECT_EQ(pointed_two->second, second_one);
  }

  TEST_F(PointedTest, CopyAssignmentTest) {
    constexpr uint64_t index_one = 12u;

    auto pointed_one = test_pointed{_address + index_one * sizeof(test_raw)};

    EXPECT_TRUE(pointed_one);

    constexpr test_atom first_one = 777;
    constexpr test_atom second_one = 2732;

    pointed_one->first = first_one;
    pointed_one->second = second_one;

    constexpr uint64_t index_two = 22u;

    auto pointed_two = test_pointed{_address + index_two * sizeof(test_raw)};

    EXPECT_TRUE(pointed_two);

    constexpr test_atom first_two = 122;
    constexpr test_atom second_two = 7616;

    pointed_two->first = first_two;
    pointed_two->second = second_two;

    pointed_two = pointed_one;

    EXPECT_TRUE(pointed_one);
    EXPECT_TRUE(pointed_two);

    EXPECT_EQ(pointed_one->first, first_one);
    EXPECT_EQ(pointed_one->second, second_one);

    EXPECT_EQ(pointed_two->first, first_one);
    EXPECT_EQ(pointed_two->second, second_one);

    auto expected = (test_raw)(first_two);
    expected |= (test_raw)(second_two) << sizeof(test_atom) * CHAR_BIT;

    EXPECT_EQ(_data[index_two], expected);
  }

} // namespace hatch