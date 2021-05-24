#include <hatch/utility/indexed.hh>
#include <gtest/gtest.h>

#include <utility> // std::pair

#include <cstdint> // uint64_t, int16_t

namespace hatch {

  class IndexedTest : public ::testing::Test {
  protected:
    using test_underlying = uint64_t;
    using test_overlay = std::pair<int16_t, int16_t>;
    static constexpr auto test_width = widths::bits32;
    static constexpr auto test_stride = sizeof(test_underlying);
    static constexpr auto test_offset = (sizeof(test_underlying) - sizeof(test_overlay)) / 2;

    using test_indexed = indexed<test_overlay, test_width, test_stride, test_offset>;

    test_underlying* _array;
    std::byte* _bytes;
    std::aligned_storage_t<256 * sizeof(test_underlying), alignof(test_underlying)> _memory;

    virtual void SetUp() override {
      _array = reinterpret_cast<test_underlying*>(&_memory);
      _bytes = reinterpret_cast<std::byte*>(&_memory);
    }
  };

  TEST_F(IndexedTest, SanityTest) {
    auto guard = test_indexed::context{&_memory};

    constexpr nosignint<test_width> index = 115;
    constexpr int16_t first = 55;
    constexpr int16_t second = 22;

    auto indexed = test_indexed{reinterpret_cast<test_overlay*>(index * test_stride + test_offset + _bytes)};

    indexed->first = first;
    indexed->second = second;

    EXPECT_EQ(_array[index], ((uint64_t)(second) << 32 | (uint64_t)(first) << 16));
  }

}