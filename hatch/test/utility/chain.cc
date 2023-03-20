#include <hatch/utility/chain.hh>
#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <iostream>

namespace hatch {

  class ChainTest : public ::testing::Test {

    /**
     * Indexed.
     */

  protected:
    static constexpr auto test_offset = 4lu;
    static constexpr auto test_stride = 16lu;
    static constexpr auto test_count = 8lu;
    static constexpr auto test_capacity = 12lu;

    using test_indexed_chain = chain<indexed, widths::bits16, test_stride, test_offset>;
    using test_indexed_reference = indexed<test_indexed_chain , widths::bits16, test_stride, test_offset>;
    using test_indexed_context = test_indexed_reference::context;

    class test_indexed_node : public test_indexed_chain {
      /**
       * Construction and assignment.
       */

    public:
      test_indexed_node(uint32_t left, uint32_t right) : left{left}, right{right} {
      }

      test_indexed_node(test_indexed_node&& moved) noexcept :
          test_indexed_chain(std::move(moved)),
          left{moved.left},
          right{moved.right} {
        moved.left = 0;
        moved.right = 0;
      }

      test_indexed_node& operator=(test_indexed_node&& moved) noexcept {
        test_indexed_chain::operator=(std::move(moved));
        left = moved.left;
        right = moved.right;
        moved.left = 0;
        moved.right = 0;
        return *this;
      }

      /**
       * Accessors.
       */

    public:
      bool test_alone() const {
        return test_indexed_chain::alone();
      }

      test_indexed_reference test_next() {
        return test_indexed_chain::next();
      }

      test_indexed_reference test_prev() {
        return test_indexed_chain::prev();
      }

      /**
       * Mutators.
       */

    public:
      void test_splice(test_indexed_reference link) {
        test_indexed_chain::splice(link);
      }

      /**
       * Self: Value.
       */

    public:
      uint32_t left;
      uint32_t right;
    };

    /**
     * Pointed.
     */

  protected:
    using test_pointed_chain = chain<pointed>;
    using test_pointed_reference = pointed<chain<pointed>>;

    class test_pointed_node : public test_pointed_chain {
      /**
       * Construction and assignment.
       */

    public:
      test_pointed_node(uint16_t number) : value{number} {
      }

      test_pointed_node(test_pointed_node&& moved) noexcept :
          test_pointed_chain(std::move(moved)),
          value{moved.value} {
        moved.value = 0;
      }

      test_pointed_node& operator=(test_pointed_node&& moved) noexcept {
        test_pointed_chain::operator=(std::move(moved));
        value = moved.value;
        moved.value = 0;
        return *this;
      }

      /**
       * Accessors.
       */

    public:
      bool test_alone() const {
        return test_pointed_chain::alone();
      }

      test_pointed_reference test_next() {
        return test_pointed_chain::next();
      }

      test_pointed_reference test_prev() {
        return test_pointed_chain::prev();
      }

      /**
       * Mutators.
       */

    public:
      void test_splice(test_pointed_reference link) {
        test_pointed_chain::splice(link);
      }

      /**
       * Self: Value.
       */

    public:
      uint16_t value;
    };

    /**
     * Test initialization.
     */

    std::aligned_storage_t<test_capacity * test_stride, alignof(test_indexed_node)> test_indexed_memory{};
    std::byte* test_indexed_address{reinterpret_cast<std::byte*>(&test_indexed_memory)};
    std::array<test_indexed_node*, test_count> test_indexed_nodes{};

    std::aligned_storage_t<test_capacity * sizeof(test_pointed_node), alignof(test_pointed_node)> test_pointed_memory{};
    std::byte* test_pointed_address{reinterpret_cast<std::byte*>(&test_pointed_memory)};
    std::array<test_pointed_node*, test_count> test_pointed_nodes{};

    void SetUp() override {
      auto context = test_indexed_context(&test_indexed_memory);
      for (auto index = 0lu; index < test_count; ++index) {
        auto left = static_cast<uint32_t>(3 * index + 2);
        auto right = static_cast<uint32_t>(4 * index + 1);
        test_indexed_nodes[index] = new (test_indexed_address + index * test_stride + test_offset) test_indexed_node{left, right};

        auto number = static_cast<uint16_t>(2 * index + 5);
        test_pointed_nodes[index] = new (test_pointed_address + index * sizeof(test_pointed_node)) test_pointed_node{number};
      }
    }
    
    void TearDown() override {
      auto context = test_indexed_context(&test_indexed_memory);
      for (auto index = 0lu; index < test_count; ++index) {
        test_indexed_nodes[index]->~test_indexed_node();
        test_indexed_nodes[index] = nullptr;
        test_pointed_nodes[index]->~test_pointed_node();
        test_pointed_nodes[index] = nullptr;
      }
    }
  };

  TEST_F(ChainTest, EmptyIndexedTest) {
    auto context = test_indexed_context(test_indexed_address);
    for (auto index = 0lu; index < test_count; ++index) {
      EXPECT_TRUE(test_indexed_nodes[index]->test_alone());
    }
  }

  TEST_F(ChainTest, EmptyPointedTest) {
    for (auto index = 0lu; index < test_count; ++index) {
      EXPECT_TRUE(test_pointed_nodes[index]->test_alone());
    }
  }

  TEST_F(ChainTest, SimpleIndexedSpliceTest) {
    auto context = test_indexed_context(test_indexed_address);

    auto* first = test_indexed_nodes[0];
    auto* second = test_indexed_nodes[1];
    auto* third = test_indexed_nodes[2];

    EXPECT_TRUE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_TRUE(third->test_alone());

    second->test_splice(test_indexed_reference{third});

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_TRUE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    first->test_splice(test_indexed_reference{second});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(second->test_prev()(), first);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_FALSE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    second->test_splice(second->test_next());

    EXPECT_EQ(first->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), first);

    EXPECT_FALSE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    first->test_splice(test_indexed_reference{first});

    EXPECT_EQ(first->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), first);

    EXPECT_FALSE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_FALSE(third->test_alone());
  }

  TEST_F(ChainTest, SimplePointedSpliceTest) {
    auto* first = test_pointed_nodes[0];
    auto* second = test_pointed_nodes[1];
    auto* third = test_pointed_nodes[2];

    EXPECT_TRUE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_TRUE(third->test_alone());

    second->test_splice(test_pointed_reference{third});

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_TRUE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    first->test_splice(test_pointed_reference{second});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(second->test_prev()(), first);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_FALSE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    second->test_splice(second->test_next());

    EXPECT_EQ(first->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), first);

    EXPECT_FALSE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    first->test_splice(test_pointed_reference{first});

    EXPECT_EQ(first->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(third->test_prev()(), first);

    EXPECT_FALSE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_FALSE(third->test_alone());
  }

  TEST_F(ChainTest, MultipleIndexedSpliceTest) {
    auto context = test_indexed_context(test_indexed_address);

    auto* first = test_indexed_nodes[0];
    auto* second = test_indexed_nodes[1];
    auto* third = test_indexed_nodes[2];
    auto* fourth = test_indexed_nodes[3];
    auto* fifth = test_indexed_nodes[4];
    auto* sixth = test_indexed_nodes[5];

    EXPECT_TRUE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_TRUE(third->test_alone());

    EXPECT_TRUE(fourth->test_alone());
    EXPECT_TRUE(fifth->test_alone());
    EXPECT_TRUE(sixth->test_alone());

    second->test_splice(test_indexed_reference{third});
    first->test_splice(test_indexed_reference{second});

    fifth->test_splice(test_indexed_reference{sixth});
    fourth->test_splice(test_indexed_reference{fifth});

    EXPECT_FALSE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    EXPECT_FALSE(fourth->test_alone());
    EXPECT_FALSE(fifth->test_alone());
    EXPECT_FALSE(sixth->test_alone());

    second->test_splice(test_indexed_reference{fifth});

    EXPECT_EQ(fourth->test_next()(), second);
    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_next()(), fifth);
    EXPECT_EQ(fifth->test_next()(), sixth);
    EXPECT_EQ(sixth->test_next()(), fourth);
    EXPECT_EQ(fourth->test_prev()(), sixth);
    EXPECT_EQ(second->test_prev()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(fifth->test_prev()(), first);
    EXPECT_EQ(sixth->test_prev()(), fifth);
  }

  TEST_F(ChainTest, MultiplePointedSpliceTest) {
    auto* first = test_pointed_nodes[0];
    auto* second = test_pointed_nodes[1];
    auto* third = test_pointed_nodes[2];
    auto* fourth = test_pointed_nodes[3];
    auto* fifth = test_pointed_nodes[4];
    auto* sixth = test_pointed_nodes[5];

    EXPECT_TRUE(first->test_alone());
    EXPECT_TRUE(second->test_alone());
    EXPECT_TRUE(third->test_alone());

    EXPECT_TRUE(fourth->test_alone());
    EXPECT_TRUE(fifth->test_alone());
    EXPECT_TRUE(sixth->test_alone());

    second->test_splice(test_pointed_reference{third});
    first->test_splice(test_pointed_reference{second});

    fifth->test_splice(test_pointed_reference{sixth});
    fourth->test_splice(test_pointed_reference{fifth});

    EXPECT_FALSE(first->test_alone());
    EXPECT_FALSE(second->test_alone());
    EXPECT_FALSE(third->test_alone());

    EXPECT_FALSE(fourth->test_alone());
    EXPECT_FALSE(fifth->test_alone());
    EXPECT_FALSE(sixth->test_alone());

    second->test_splice(test_pointed_reference{fifth});

    EXPECT_EQ(fourth->test_next()(), second);
    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(third->test_next()(), first);
    EXPECT_EQ(first->test_next()(), fifth);
    EXPECT_EQ(fifth->test_next()(), sixth);
    EXPECT_EQ(sixth->test_next()(), fourth);
    EXPECT_EQ(fourth->test_prev()(), sixth);
    EXPECT_EQ(second->test_prev()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);
    EXPECT_EQ(first->test_prev()(), third);
    EXPECT_EQ(fifth->test_prev()(), first);
    EXPECT_EQ(sixth->test_prev()(), fifth);
  }

  TEST_F(ChainTest, IndexedMoveConstructorTest) {
    auto context = test_indexed_context(test_indexed_address);

    auto* first = test_indexed_nodes[0];
    auto* second = test_indexed_nodes[1];
    auto* third = test_indexed_nodes[2];
    auto* fourth = test_indexed_nodes[3];

    first->test_splice(test_indexed_reference{second});
    first->test_splice(test_indexed_reference{third});
    first->test_splice(test_indexed_reference{fourth});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(second->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    auto* address = test_indexed_address + test_count * test_stride + test_offset;
    auto* node = new (address) test_indexed_node{std::move(*second)};
    
    EXPECT_EQ(first->test_next()(), node);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(node->test_next()(), third);
    EXPECT_EQ(node->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), node);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    EXPECT_EQ(second->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), second);
  }

  TEST_F(ChainTest, PointedMoveConstructorTest) {
    auto* first = test_pointed_nodes[0];
    auto* second = test_pointed_nodes[1];
    auto* third = test_pointed_nodes[2];
    auto* fourth = test_pointed_nodes[3];

    first->test_splice(test_pointed_reference{second});
    first->test_splice(test_pointed_reference{third});
    first->test_splice(test_pointed_reference{fourth});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(second->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    auto* address = test_pointed_address + test_count * sizeof(test_pointed_node);
    auto* node = new (address) test_pointed_node{std::move(*second)};

    EXPECT_EQ(first->test_next()(), node);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(node->test_next()(), third);
    EXPECT_EQ(node->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), node);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    EXPECT_EQ(second->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), second);
  }

  TEST_F(ChainTest, IndexedMoveAssignmentTest) {
    auto context = test_indexed_context(test_indexed_address);

    auto* first = test_indexed_nodes[0];
    auto* second = test_indexed_nodes[1];
    auto* third = test_indexed_nodes[2];
    auto* fourth = test_indexed_nodes[3];
    auto* fifth = test_indexed_nodes[4];

    first->test_splice(test_indexed_reference{second});
    first->test_splice(test_indexed_reference{third});
    first->test_splice(test_indexed_reference{fourth});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(second->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    *fifth = std::move(*second);

    EXPECT_EQ(first->test_next()(), fifth);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(fifth->test_next()(), third);
    EXPECT_EQ(fifth->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), fifth);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    EXPECT_EQ(second->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), second);
  }

  TEST_F(ChainTest, PointedMoveAssignmentTest) {
    auto* first = test_pointed_nodes[0];
    auto* second = test_pointed_nodes[1];
    auto* third = test_pointed_nodes[2];
    auto* fourth = test_pointed_nodes[3];
    auto* fifth = test_pointed_nodes[4];

    first->test_splice(test_pointed_reference{second});
    first->test_splice(test_pointed_reference{third});
    first->test_splice(test_pointed_reference{fourth});

    EXPECT_EQ(first->test_next()(), second);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(second->test_next()(), third);
    EXPECT_EQ(second->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), second);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    *fifth = std::move(*second);

    EXPECT_EQ(first->test_next()(), fifth);
    EXPECT_EQ(first->test_prev()(), fourth);

    EXPECT_EQ(fifth->test_next()(), third);
    EXPECT_EQ(fifth->test_prev()(), first);

    EXPECT_EQ(third->test_next()(), fourth);
    EXPECT_EQ(third->test_prev()(), fifth);

    EXPECT_EQ(fourth->test_next()(), first);
    EXPECT_EQ(fourth->test_prev()(), third);

    EXPECT_EQ(second->test_next()(), second);
    EXPECT_EQ(second->test_prev()(), second);
  }
}