#include <hatch/utility/chain.hh>
#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <cstdint>

namespace hatch {

  class ChainTest : public ::testing::Test {
  protected:
    static constexpr auto test_offset = 4u;
    static constexpr auto test_stride = 16u;
    static constexpr auto test_count = 8u;
    static constexpr auto test_capacity = 12u;

    template <class T>
    using test_indexed = indexed<T, widths::bits16, test_stride, test_offset>;

    class test_indexed_chain : public chain<test_indexed_chain, test_indexed> {
    public:
      using context = indexed<test_indexed_chain, widths::bits16, test_stride, test_offset>::context;

    public:
      test_indexed_chain(uint32_t left, uint32_t right) : left{left}, right{right} {
      }

      test_indexed_chain(test_indexed_chain&& moved) noexcept :
          chain<test_indexed_chain, test_indexed>(std::move(moved)),
          left{moved.left},
          right{moved.right} {
        moved.left = 0;
        moved.right = 0;
      }

      test_indexed_chain& operator=(test_indexed_chain&& moved) noexcept {
        chain<test_indexed_chain, test_indexed>::operator=(std::move(moved));
        left = moved.left;
        right = moved.right;
        moved.left = 0;
        moved.right = 0;
        return *this;
      }

      void splice(test_indexed_chain& link) {
        chain<test_indexed_chain, test_indexed>::splice(test_indexed<test_indexed_chain>{&link});
      }

    public:
      uint32_t left;
      uint32_t right;
    };

    class test_pointed_chain : public chain<test_pointed_chain, pointed> {
    public:
      test_pointed_chain(uint16_t number) : value{number} {
      }

      test_pointed_chain(test_pointed_chain&& moved) noexcept :
          chain<test_pointed_chain, pointed>(std::move(moved)),
          value{moved.value} {
        moved.value = 0;
      }

      test_pointed_chain& operator=(test_pointed_chain&& moved) noexcept {
        chain<test_pointed_chain, pointed>::operator=(std::move(moved));
        value = moved.value;
        moved.value = 0;
        return *this;
      }

      void splice(test_pointed_chain& link) {
        chain<test_pointed_chain, pointed>::splice(pointed<test_pointed_chain>(&link));
      }

      uint16_t value;
    };

    std::aligned_storage_t<test_capacity * test_stride, alignof(test_indexed_chain)> test_indexed_memory;
    std::aligned_storage_t<test_capacity * sizeof(test_pointed_chain), alignof(test_pointed_chain)> test_pointed_memory;

    std::vector<test_indexed_chain*> test_indexed_pointers{test_count, nullptr};
    std::vector<test_pointed_chain*> test_pointed_pointers{test_count, nullptr};

    std::byte* test_indexed_address{reinterpret_cast<std::byte*>(&test_indexed_memory)};
    std::byte* test_pointed_address{reinterpret_cast<std::byte*>(&test_pointed_memory)};

    void SetUp() override {
      auto context = test_indexed_chain::context(&test_indexed_memory);
      for (auto index = 0u; index < test_count; ++index) {
        auto left = 3 * index + 2;
        auto right = 4 * index + 1;
        test_indexed_pointers[index] = new(test_indexed_address + index * test_stride + test_offset) test_indexed_chain{left, right};

        auto number = static_cast<uint16_t>(2 * index + 5);
        test_pointed_pointers[index] = new (test_pointed_address + index * sizeof(test_pointed_chain)) test_pointed_chain{number};
      }
    }
  };

  TEST_F(ChainTest, EmptyIndexedTest) {
    auto context = test_indexed_chain::context(&test_indexed_memory);
    for (auto index = 0u; index < test_count; ++index) {
      EXPECT_TRUE(test_indexed_pointers[index]->alone());
    }
  }

  TEST_F(ChainTest, EmptyPointedTest) {
    for (auto index = 0u; index < test_count; ++index) {
      EXPECT_TRUE(test_pointed_pointers[index]->alone());
    }
  }

  TEST_F(ChainTest, SimpleIndexedSpliceTest) {
    auto context = test_indexed_chain::context(&test_indexed_memory);

    auto& first = *test_indexed_pointers[0];
    auto& second = *test_indexed_pointers[1];
    auto& third = *test_indexed_pointers[2];

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

  TEST_F(ChainTest, SimplePointedSpliceTest) {
    auto& first = *test_pointed_pointers[0];
    auto& second = *test_pointed_pointers[1];
    auto& third = *test_pointed_pointers[2];

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

  TEST_F(ChainTest, MultipleIndexedSpliceTest) {
    auto context = test_indexed_chain::context(&test_indexed_memory);

    auto& first = *test_indexed_pointers[0];
    auto& second = *test_indexed_pointers[1];
    auto& third = *test_indexed_pointers[2];
    auto& fourth = *test_indexed_pointers[3];
    auto& fifth = *test_indexed_pointers[4];
    auto& sixth = *test_indexed_pointers[5];

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

  TEST_F(ChainTest, MultiplePointedSpliceTest) {
    auto& first = *test_pointed_pointers[0];
    auto& second = *test_pointed_pointers[1];
    auto& third = *test_pointed_pointers[2];
    auto& fourth = *test_pointed_pointers[3];
    auto& fifth = *test_pointed_pointers[4];
    auto& sixth = *test_pointed_pointers[5];

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

  TEST_F(ChainTest, IndexedMoveConstructorTest) {
    auto context = test_indexed_chain::context(&test_indexed_memory);

    auto& first = *test_indexed_pointers[0];
    auto& second = *test_indexed_pointers[1];
    auto& third = *test_indexed_pointers[2];
    auto& fourth = *test_indexed_pointers[3];

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

    auto* address = test_indexed_address + test_count * test_stride + test_offset;
    auto* pointer = new (address) test_indexed_chain{std::move(second)};
    auto& node = *pointer;

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

  TEST_F(ChainTest, PointedMoveConstructorTest) {
    auto& first = *test_pointed_pointers[0];
    auto& second = *test_pointed_pointers[1];
    auto& third = *test_pointed_pointers[2];
    auto& fourth = *test_pointed_pointers[3];

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

    auto node = test_pointed_chain{std::move(second)};

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

  TEST_F(ChainTest, IndexedMoveAssignmentTest) {
    auto context = test_indexed_chain::context(&test_indexed_memory);

    auto& first = *test_indexed_pointers[0];
    auto& second = *test_indexed_pointers[1];
    auto& third = *test_indexed_pointers[2];
    auto& fourth = *test_indexed_pointers[3];
    auto& fifth = *test_indexed_pointers[4];

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

  TEST_F(ChainTest, PointedMoveAssignmentTest) {
    auto& first = *test_pointed_pointers[0];
    auto& second = *test_pointed_pointers[1];
    auto& third = *test_pointed_pointers[2];
    auto& fourth = *test_pointed_pointers[3];
    auto& fifth = *test_pointed_pointers[4];

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