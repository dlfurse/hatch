#include <hatch/utility/tree.hh>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include <cstdint>

namespace hatch {

  class TreeTest : public ::testing::Test {
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

      bool operator<(const test_node& other) const {
        return value < other.value;
      }

      bool is_red() const {
        return tree_node<test_node>::is_red();
      }

      bool is_black() const {
        return tree_node<test_node>::is_black();
      }

      bool is_root() const {
        return tree_node<test_node>::is_root();
      }

      bool is_prev() const {
        return tree_node<test_node>::is_prev();
      }

      bool is_next() const {
        return tree_node<test_node>::is_next();
      }

      int black_depth() const {
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

    std::vector<test_node> _nodes;
    tree<test_node> _tree;

    void SetUp() override {
      _nodes.reserve(count);
      for (auto value = 0u; value < count; value++) {
        _nodes.emplace_back(value);
      }
    }
  };

  std::ostream& operator<<(std::ostream& stream, const TreeTest::test_node& node) {
    stream << node.value << (node.is_red() ? ", r" : ", b");
    return stream;
  }

  std::ostream& operator<<(std::ostream& stream, const TreeTest::test_failure& failure) {
    stream << failure.self.value << " (" << failure.prev.value << ", " << failure.next.value << ")";
    return stream;
  }

  TEST_F(TreeTest, EmptyTree) {
    EXPECT_TRUE(_nodes[0].alone());
    EXPECT_TRUE(_nodes[0].is_black());
  }

  TEST_F(TreeTest, SimpleEasyTree) {
    _tree.insert(_nodes[3]);
    _tree.insert(_nodes[1]);
    _tree.insert(_nodes[5]);
    _tree.insert(_nodes[0]);
    _tree.insert(_nodes[2]);
    _tree.insert(_nodes[4]);
    _tree.insert(_nodes[6]);

    int height;
    try {
      height = _nodes[3].black_depth();
    } catch (const test_failure& failure) {
      std::stringstream message{};
      message << "rb failure: " << failure;
      FAIL() << message.str();
    }
    EXPECT_EQ(height, 2);

    EXPECT_FALSE(_nodes[0].alone());
    EXPECT_TRUE(_nodes[0].is_red());
    EXPECT_TRUE(_nodes[0].is_prev());

    EXPECT_FALSE(_nodes[1].alone());
    EXPECT_TRUE(_nodes[1].is_black());
    EXPECT_TRUE(_nodes[1].is_prev());

    EXPECT_FALSE(_nodes[2].alone());
    EXPECT_TRUE(_nodes[2].is_red());
    EXPECT_TRUE(_nodes[2].is_next());

    EXPECT_FALSE(_nodes[3].alone());
    EXPECT_TRUE(_nodes[3].is_black());
    EXPECT_TRUE(_nodes[3].is_root());

    EXPECT_FALSE(_nodes[4].alone());
    EXPECT_TRUE(_nodes[4].is_red());
    EXPECT_TRUE(_nodes[4].is_prev());

    EXPECT_FALSE(_nodes[5].alone());
    EXPECT_TRUE(_nodes[5].is_black());
    EXPECT_TRUE(_nodes[5].is_next());

    EXPECT_FALSE(_nodes[6].alone());
    EXPECT_TRUE(_nodes[6].is_red());
    EXPECT_TRUE(_nodes[6].is_next());

    EXPECT_EQ(_nodes[3].minimum(), &_nodes[0]);
    EXPECT_EQ(_nodes[1].minimum(), &_nodes[0]);
    EXPECT_EQ(_nodes[5].minimum(), &_nodes[4]);

    for (auto index = 0u; index < 6; index++) {
      EXPECT_EQ(_nodes[index].successor(), &_nodes[index + 1]);
      EXPECT_EQ(_nodes[index].root(), &_nodes[3]);
    }

    EXPECT_EQ(_nodes[3].maximum(), &_nodes[6]);
    EXPECT_EQ(_nodes[1].maximum(), &_nodes[2]);
    EXPECT_EQ(_nodes[5].maximum(), &_nodes[6]);

    for (auto index = 6u; index < 0; index++) {
      EXPECT_EQ(_nodes[index].predecessor(), &_nodes[index - 1]);
      EXPECT_EQ(_nodes[index].root(), &_nodes[3]);
    }
  }

  TEST_F(TreeTest, SimpleTougherTree) {
    _tree.insert(_nodes[0]);

    int this_depth = _tree.root()->get().black_depth();

    for (auto index = 1u; index < count; index++) {
      _tree.insert(_nodes[index]);
      int next_depth;
      try {
        next_depth = _tree.root()->get().black_depth();
      } catch (const test_failure& failure) {
        std::stringstream message{};
        message << "rb failure: " << failure;
        FAIL() << message.str();
      }
      EXPECT_LE(this_depth, next_depth);
      this_depth = next_depth;
    }

    EXPECT_EQ(this_depth, 6);

    for (auto index = 1u; index < count; index++) {
      _tree.begin().remove();
      int next_depth;
      try {
        next_depth = _tree.root()->get().black_depth();
      } catch (const test_failure& failure) {
        std::stringstream message{};
        message << "rb failure: " << failure;
        FAIL() << message.str();
      }
      EXPECT_GE(this_depth, next_depth);
      this_depth = next_depth;
    }
  }

} // namespace hatch