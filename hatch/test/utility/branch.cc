#include <hatch/utility/branch.hh>
#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include <cstdint>

namespace hatch {

  class BranchTest : public ::testing::Test {
  public:
    class test_pointed_branch : public branch<pointed> {
    public:
      test_pointed_branch(uint64_t value) :
        value{value} {
      }

      bool operator<(const test_pointed_branch& other) const {
        return value < other.value;
      }

      bool is_red() const {
        return branch<pointed>::is_red();
      }

      bool is_black() const {
        return branch<pointed>::is_black();
      }

      bool alone() const {
        return branch<pointed>::alone();
      }

      void insert(test_branch& other) {
        return branch<test_branch, pointed>::insert(other);
      }

    public:
      class rb_failure {
      public:
        rb_failure(const uint64_t& self, const uint64_t& prev, const uint64_t& next) :
        self{self}, prev{prev}, next{next} {
        }

        uint64_t self;
        uint64_t prev;
        uint64_t next;
      };

      int black_depth() const {
        int depth = is_black() ? 1 : 0;

        auto* prev_p = static_cast<const test_pointed_branch*>(_prev());
        auto* next_p = static_cast<const test_pointed_branch*>(_next());

        if (prev_p && next_p) {
          int prev = prev_p->black_depth();
          int next = next_p->black_depth();
          if (prev >= 0 && next >= 0 && prev == next) {
            depth += (prev + next) / 2;
          } else {
            throw rb_failure(value, prev_p->value, next_p->value);
          }
        } else if (prev_p) {
          depth += prev_p->black_depth();
        } else if (next_p) {
          depth += next_p->black_depth();
        }

        return depth;
      }

    public:
      uint64_t value;
    };

    void insert(test_pointed_branch& branch) {
      //_root->insert(branch);
      //_root = _root->root();
    }

  protected:
    static constexpr unsigned int count = 128;

    std::vector<test_pointed_branch> _test_pointed_branches;
    test_pointed_branch* _test_pointed_root;

    void SetUp() override {
      _test_pointed_branches.reserve(count);
      for (auto value = 0u; value < count; value++) {
        _test_pointed_branches.emplace_back(value);
      }
      _test_pointed_root = &_test_pointed_root[0];
    }
  };

  std::ostream& operator<<(std::ostream& stream, const BranchTest::test_pointed_branch& branch) {
    stream << branch.value << (branch.is_red() ? ", r" : ", b");
    return stream;
  }

  std::ostream& operator<<(std::ostream& stream, const BranchTest::test_pointed_branch::rb_failure& failure) {
    stream << failure.self << " (" << failure.prev << ", " << failure.next << ")";
    return stream;
  }

  TEST_F(BranchTest, EmptyBranch) {
    EXPECT_TRUE(_test_pointed_branches[0].alone());
    EXPECT_TRUE(_test_pointed_branches[0].is_black());
  }

  TEST_F(BranchTest, SimpleEasyBranch) {
    _root->insert(_branches[3]);
    _tree.insert(_branches[1]);
    _tree.insert(_branches[5]);
    _tree.insert(_branches[0]);
    _tree.insert(_branches[2]);
    _tree.insert(_branches[4]);
    _tree.insert(_branches[6]);

    int height;
    try {
      height = _branches[3].black_depth();
    } catch (const test_failure& failure) {
      std::stringstream message{};
      message << "rb failure: " << failure;
      FAIL() << message.str();
    }
    EXPECT_EQ(height, 2);

    EXPECT_FALSE(_branches[0].alone());
    EXPECT_TRUE(_branches[0].is_red());
    EXPECT_TRUE(_branches[0].is_prev());

    EXPECT_FALSE(_branches[1].alone());
    EXPECT_TRUE(_branches[1].is_black());
    EXPECT_TRUE(_branches[1].is_prev());

    EXPECT_FALSE(_branches[2].alone());
    EXPECT_TRUE(_branches[2].is_red());
    EXPECT_TRUE(_branches[2].is_next());

    EXPECT_FALSE(_branches[3].alone());
    EXPECT_TRUE(_branches[3].is_black());
    EXPECT_TRUE(_branches[3].is_root());

    EXPECT_FALSE(_branches[4].alone());
    EXPECT_TRUE(_branches[4].is_red());
    EXPECT_TRUE(_branches[4].is_prev());

    EXPECT_FALSE(_branches[5].alone());
    EXPECT_TRUE(_branches[5].is_black());
    EXPECT_TRUE(_branches[5].is_next());

    EXPECT_FALSE(_branches[6].alone());
    EXPECT_TRUE(_branches[6].is_red());
    EXPECT_TRUE(_branches[6].is_next());

    EXPECT_EQ(_branches[3].minimum(), &_branches[0]);
    EXPECT_EQ(_branches[1].minimum(), &_branches[0]);
    EXPECT_EQ(_branches[5].minimum(), &_branches[4]);

    for (auto index = 0u; index < 6; index++) {
      EXPECT_EQ(_branches[index].successor(), &_branches[index + 1]);
      EXPECT_EQ(_branches[index].root(), &_branches[3]);
    }

    EXPECT_EQ(_branches[3].maximum(), &_branches[6]);
    EXPECT_EQ(_branches[1].maximum(), &_branches[2]);
    EXPECT_EQ(_branches[5].maximum(), &_branches[6]);

    for (auto index = 6u; index < 0; index++) {
      EXPECT_EQ(_branches[index].predecessor(), &_branches[index - 1]);
      EXPECT_EQ(_branches[index].root(), &_branches[3]);
    }
  }

  /*

  TEST_F(BranchTest, SimpleTougherBranch) {
    _tree.insert(_branches[0]);

    int this_depth = _tree.root()->get().black_depth();

    for (auto index = 1u; index < count; index++) {
      _tree.insert(_branches[index]);
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
   */

} // namespace hatch