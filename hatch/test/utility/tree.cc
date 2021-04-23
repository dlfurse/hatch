#include <hatch/utility/tree.hh>
#include <gtest/gtest.h>

#include <cstdint>

namespace hatch {

  class PointerTreeTest : public ::testing::Test {
  public:
    class test_data {
    public:
      test_data(uint64_t value) :
          value{value} {
      }

      uint64_t value;
    };

    class test_failure {
    public:
      test_failure(const test_data& self, const test_data& prev, const test_data& next) :
          self{self}, prev{prev}, next{next} {
      }

      test_data self;
      test_data prev;
      test_data next;
    };

    class test_node : public test_data, public tree_node<test_node> {
    public:
      test_node(uint64_t value) :
          test_data{value} {
      }

      bool operator<(const test_node& other) {
        return value < other.value;
      }

      bool is_red() {
        return tree_node<test_node>::is_red();
      }

      bool is_black() {
        return tree_node<test_node>::is_black();
      }

      bool is_root() {
        return tree_node<test_node>::is_root();
      }

      bool is_prev() {
        return tree_node<test_node>::is_prev();
      }

      bool is_next() {
        return tree_node<test_node>::is_next();
      }

      int black_depth() {
        int depth = is_black() ? 1 : 0;
        if (_prev && _next) {
          int prev = _prev->get().black_depth();
          int next = _next->get().black_depth();
          if (prev >= 0 && next >= 0 && prev == next) {
            depth += (prev + next) / 2;
          } else {
            throw test_failure(*this, _prev->get(), _next->get());
          }
        } else if (_prev) {
          depth += _prev->get().black_depth();
        } else if (_next) {
          depth += _next->get().black_depth();
        }

        return depth;
      }
    };

  protected:
    static constexpr unsigned int count = 128;
    std::vector<test_node> nodes;

    tree<test_node> tree;

    void SetUp() override {
      nodes.reserve(count);
      for (auto value = 0u; value < count; value++) {
        nodes.emplace_back(value);
      }
    }
  };

  std::ostream& operator<<(std::ostream& stream, const PointerTreeTest::test_data& data) {
    stream << data.value;
    return stream;
  }

  std::ostream& operator<<(std::ostream& stream, const PointerTreeTest::test_failure& failure) {
    stream << failure.self.value << " (" << failure.prev.value << ", " << failure.next.value << ")";
    return stream;
  }

  TEST_F(PointerTreeTest, EmptyTree) {
    EXPECT_TRUE(nodes[0].alone());
    EXPECT_TRUE(nodes[0].is_black());
  }

  TEST_F(PointerTreeTest, SimpleEasyTree) {
    tree.insert(nodes[3]);
    tree.insert(nodes[1]);
    tree.insert(nodes[5]);
    tree.insert(nodes[0]);
    tree.insert(nodes[2]);
    tree.insert(nodes[4]);
    tree.insert(nodes[6]);

    int height;
    try {
      height = nodes[3].black_depth();
    } catch (const test_failure& failure) {
      auto message = std::stringstream() << "rb failure: " << failure;
      FAIL() << message.str();
    }
    EXPECT_EQ(height, 2);

    EXPECT_FALSE(nodes[0].alone());
    EXPECT_TRUE(nodes[0].is_red());
    EXPECT_TRUE(nodes[0].is_prev());

    EXPECT_FALSE(nodes[1].alone());
    EXPECT_TRUE(nodes[1].is_black());
    EXPECT_TRUE(nodes[1].is_prev());

    EXPECT_FALSE(nodes[2].alone());
    EXPECT_TRUE(nodes[2].is_red());
    EXPECT_TRUE(nodes[2].is_next());

    EXPECT_FALSE(nodes[3].alone());
    EXPECT_TRUE(nodes[3].is_black());
    EXPECT_TRUE(nodes[3].is_root());

    EXPECT_FALSE(nodes[4].alone());
    EXPECT_TRUE(nodes[4].is_red());
    EXPECT_TRUE(nodes[4].is_prev());

    EXPECT_FALSE(nodes[5].alone());
    EXPECT_TRUE(nodes[5].is_black());
    EXPECT_TRUE(nodes[5].is_next());

    EXPECT_FALSE(nodes[6].alone());
    EXPECT_TRUE(nodes[6].is_red());
    EXPECT_TRUE(nodes[6].is_next());

//    EXPECT_EQ(nodes[3].minimum(), &nodes[0]);
//    EXPECT_EQ(nodes[1].minimum(), &nodes[0]);
//    EXPECT_EQ(nodes[5].minimum(), &nodes[4]);

    for (auto index = 0u; index < 6; index++) {
      EXPECT_EQ(nodes[index].successor(), &nodes[index + 1]);
//      EXPECT_EQ(nodes[index].root(), &nodes[3]);
    }

//    EXPECT_EQ(nodes[3].maximum(), &nodes[6]);
//    EXPECT_EQ(nodes[1].maximum(), &nodes[2]);
//    EXPECT_EQ(nodes[5].maximum(), &nodes[6]);

    for (auto index = 6u; index < 0; index++) {
      EXPECT_EQ(nodes[index].predecessor(), &nodes[index - 1]);
//      EXPECT_EQ(nodes[index].root(), &nodes[3]);
    }
  }

  TEST_F(PointerTreeTest, SimpleTougherTree) {
    tree.insert(nodes[0]);

    int this_depth = tree.root()->get().black_depth();

    for (auto index = 1u; index < 64; index++) {
      tree.insert(nodes[index]);
      int next_depth;
      try {
        next_depth = tree.root()->get().black_depth();
      } catch (const test_failure& failure) {
        auto message = std::stringstream() << "rb failure: " << failure;
        FAIL() << message.str();
      }
      EXPECT_LE(this_depth, next_depth);
      this_depth = next_depth;
    }

    EXPECT_EQ(this_depth, 5);

//    for (auto index = 0u; index < 63; index++) {
//      nodes[index].remove();
//      root = nodes[index + 1].root();
//      int next_depth;
//      try {
//        next_depth = root->get().black_depth();
//      } catch (const test_failure& failure) {
//        auto message = std::stringstream() << "rb failure: " << failure;
//        FAIL() << message.str();
//      }
//      EXPECT_GE(this_depth, next_depth);
//      this_depth = next_depth;
//    }
  }

} // namespace hatch