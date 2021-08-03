#include <hatch/utility/indexed.hh>
#include <gtest/gtest.h>

#include <utility> // std::pair

#include <cstdint> // uint64_t, int16_t

namespace hatch {

  class IndexedTest : public ::testing::Test {
  protected:
    using test_raw = uint64_t;
    using test_atom = int16_t;
    using test_pair = std::pair<test_atom, test_atom>;

    static constexpr auto test_count = 29u;
    static constexpr auto test_width = widths::bits32;
    static constexpr auto test_stride = sizeof(test_raw);
    static constexpr auto test_offset = (sizeof(test_raw) - sizeof(test_pair)) / 2;

    using test_indexed = indexed<test_pair, test_width, test_stride, test_offset>;

    test_raw* _data;
    std::byte* _address;
    std::aligned_storage_t<test_count * sizeof(test_raw), alignof(test_raw)> _block;

    virtual void SetUp() override {
      _data = reinterpret_cast<test_raw*>(&_block);
      _address = reinterpret_cast<std::byte*>(&_block);
      for (auto index = 0lu; index < test_count; ++index) {
        _data[index] = 0;
      }
    }
  };

  TEST_F(IndexedTest, DefaultNullTest) {
    auto guard = test_indexed::context{&_block};

    auto indexed = test_indexed{};

    EXPECT_FALSE(indexed);
  }

  TEST_F(IndexedTest, AddressConstructorTest) {
    auto guard = test_indexed::context{_address};

    constexpr uint64_t index = 13u;

    auto indexed = test_indexed{ _address + index * test_stride + test_offset};

    EXPECT_TRUE(indexed);

    constexpr test_atom first = 4987;
    constexpr test_atom second = 1171;

    indexed->first = first;
    indexed->second = second;

    auto expected = (test_raw)(first) << test_offset * CHAR_BIT;
    expected |= (test_raw)(second) << (sizeof(test_atom) + test_offset) * CHAR_BIT;

    EXPECT_EQ(_data[index], expected);
  }

  TEST_F(IndexedTest, AddressAssignmentTest) {
    auto guard = test_indexed::context{&_block};

    constexpr uint64_t index = 3u;

    auto indexed = test_indexed{};
    indexed =  _address + index * test_stride + test_offset;

    EXPECT_TRUE(indexed);

    constexpr test_atom first = 33;
    constexpr test_atom second = 749;

    indexed->first = first;
    indexed->second = second;

    auto expected = (test_raw)(first) << test_offset * CHAR_BIT;
    expected |= (test_raw)(second) << (sizeof(test_atom) + test_offset) * CHAR_BIT;

    EXPECT_EQ(_data[index], expected);
  }
  
  TEST_F(IndexedTest, MoveConstructorTest) {
    auto guard = test_indexed::context{&_block};

    constexpr uint64_t index_one = 19u;

    auto indexed_one = test_indexed{_address + index_one * test_stride + test_offset};

    EXPECT_TRUE(indexed_one);

    constexpr test_atom first_one = 4987;
    constexpr test_atom second_one = 1171;

    indexed_one->first = first_one;
    indexed_one->second = second_one;

    auto indexed_two = test_indexed{std::move(indexed_one)};

    EXPECT_FALSE(indexed_one);
    EXPECT_TRUE(indexed_two);

    EXPECT_EQ(indexed_two->first, first_one);
    EXPECT_EQ(indexed_two->second, second_one);
  }

  TEST_F(IndexedTest, MoveAssignmentTest) {
    auto guard = test_indexed::context{&_block};

    constexpr uint64_t index_one = 19u;

    auto indexed_one = test_indexed{_address + index_one * test_stride + test_offset};

    EXPECT_TRUE(indexed_one);

    constexpr test_atom first_one = 4987;
    constexpr test_atom second_one = 1171;

    indexed_one->first = first_one;
    indexed_one->second = second_one;

    constexpr uint64_t index_two = 8u;

    auto indexed_two = test_indexed{_address + index_two * test_stride + test_offset};

    EXPECT_TRUE(indexed_two);

    constexpr test_atom first_two = 97;
    constexpr test_atom second_two = 124;

    indexed_two->first = first_two;
    indexed_two->second = second_two;

    indexed_two = std::move(indexed_one);

    EXPECT_FALSE(indexed_one);
    EXPECT_TRUE(indexed_two);

    EXPECT_EQ(indexed_two->first, first_one);
    EXPECT_EQ(indexed_two->second, second_one);

    auto expected = (test_raw)(first_two) << test_offset * CHAR_BIT;
    expected |= (test_raw)(second_two) << (sizeof(test_atom) + test_offset) * CHAR_BIT;

    EXPECT_EQ(_data[index_two], expected);
  }
  
  TEST_F(IndexedTest, CopyConstructorTest) {
    auto guard = test_indexed::context{&_block};

    constexpr uint64_t index_one = 9u;

    auto indexed_one = test_indexed{_address + index_one * test_stride + test_offset};

    EXPECT_TRUE(indexed_one);

    constexpr test_atom first_one = 565;
    constexpr test_atom second_one = 3288;

    indexed_one->first = first_one;
    indexed_one->second = second_one;

    auto indexed_two = test_indexed{indexed_one};

    EXPECT_TRUE(indexed_one);
    EXPECT_TRUE(indexed_two);

    EXPECT_EQ(indexed_one->first, first_one);
    EXPECT_EQ(indexed_one->second, second_one);

    EXPECT_EQ(indexed_two->first, first_one);
    EXPECT_EQ(indexed_two->second, second_one);
  }

  TEST_F(IndexedTest, CopyAssignmentTest) {
    auto guard = test_indexed::context{&_block};

    constexpr uint64_t index_one = 12u;

    auto indexed_one = test_indexed{_address + index_one * test_stride + test_offset};

    EXPECT_TRUE(indexed_one);

    constexpr test_atom first_one = 777;
    constexpr test_atom second_one = 2732;

    indexed_one->first = first_one;
    indexed_one->second = second_one;

    constexpr uint64_t index_two = 22u;

    auto indexed_two = test_indexed{_address + index_two * test_stride + test_offset};

    EXPECT_TRUE(indexed_two);

    constexpr test_atom first_two = 122;
    constexpr test_atom second_two = 7616;

    indexed_two->first = first_two;
    indexed_two->second = second_two;

    indexed_two = indexed_one;

    EXPECT_TRUE(indexed_one);
    EXPECT_TRUE(indexed_two);

    EXPECT_EQ(indexed_one->first, first_one);
    EXPECT_EQ(indexed_one->second, second_one);

    EXPECT_EQ(indexed_two->first, first_one);
    EXPECT_EQ(indexed_two->second, second_one);

    auto expected = (test_raw)(first_two) << test_offset * CHAR_BIT;
    expected |= (test_raw)(second_two) << (sizeof(test_atom) + test_offset) * CHAR_BIT;

    EXPECT_EQ(_data[index_two], expected);
  }

} // namespace hatch