#include <hatch/utility/branch.hh>
#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include <cstdint>

namespace hatch {

  class BranchTest : public ::testing::Test {

    /**
     * Pointed.
     */

  public:
    using test_pointed_branch = branch<pointed>;

    class test_pointed_node : public test_pointed_branch {
    public:
      using test_colors_t = typename test_pointed_branch::colors_t;
      using test_sides_t = typename test_pointed_branch::sides_t;
      using test_ref_t = typename test_pointed_branch::ref_t;

    public:
      test_pointed_node(uint64_t value) :
        test_pointed_branch{},
        value{value} {
      }

      /**
       * Color.
       */

    public:
      test_colors_t test_color() const {
        return test_pointed_branch::color();
      }
      void test_make_color(test_colors_t color) {
        return test_pointed_branch::make_color(color);
      }

      bool test_is_red() const {
        return test_pointed_branch::is_red();
      }
      void test_make_red() {
        return test_pointed_branch::make_red();
      }

      bool test_is_black() const {
        return test_pointed_branch::is_black();
      }
      void test_make_black() {
        return test_pointed_branch::make_black();
      }

      /**
       * Structure.
       */
      
    public:
      bool test_alone() const {
        return test_pointed_branch::alone();
      }
      test_sides_t test_side() const {
        return test_pointed_branch::side();
      }

      test_ref_t test_head() {
        return test_pointed_branch::head();
      }
      const test_ref_t test_head() const {
        return test_pointed_branch::head();
      }

      test_ref_t test_prev() {
        return test_pointed_branch::prev();
      }
      const test_ref_t test_prev() const {
        return test_pointed_branch::prev();
      }

      test_ref_t test_next() {
        return test_pointed_branch::next();
      }
      const test_ref_t test_next() const {
        return test_pointed_branch::next();
      }

      test_ref_t test_child(test_sides_t side) {
        return test_pointed_branch::child(side);
      }
      const test_ref_t test_child(test_sides_t side) const {
        return test_pointed_branch::child(side);
      }

      test_ref_t test_root() {
        return test_pointed_branch::root();
      }
      const test_ref_t test_root() const {
        return test_pointed_branch::root();
      }

      test_ref_t test_minimum() {
        return test_pointed_branch::minimum();
      }
      const test_ref_t test_minimum() const {
        return test_pointed_branch::maximum();
      }

      test_ref_t test_maximum() {
        return test_pointed_branch::maximum();
      }
      const test_ref_t test_maximum() const {
        return test_pointed_branch::maximum();
      }

      test_ref_t test_predecessor() {
        return test_pointed_branch::predecessor();
      }
      const test_ref_t test_predecessor() const {
        return test_pointed_branch::predecessor();
      }

      test_ref_t test_successor() {
        return test_pointed_branch::successor();
      }
      const test_ref_t test_successor() const {
        return test_pointed_branch::successor();
      }

      /**
       * Mutators.
       */

    public:
      void test_make_head(test_ref_t new_head, test_sides_t side) {
        return test_pointed_branch::make_head(std::forward<test_ref_t>(new_head), side);
      }
      void test_make_child(test_ref_t new_child, test_sides_t side) {
        return test_pointed_branch::make_child(std::forward<test_ref_t>(new_child), side);
      }
      void test_make_next(test_ref_t new_next) {
        return test_pointed_branch::make_next(std::forward<test_ref_t>(new_next));
      }
      void test_make_prev(test_ref_t new_prev) {
        return test_pointed_branch::make_prev(std::forward<test_ref_t>(new_prev));
      }

      void test_detach() {
        return test_pointed_branch::detach();
      }
      void test_rotate(test_sides_t side) {
        return test_pointed_branch::rotate(side);
      }
      void test_exchange(test_ref_t node) {
        return test_pointed_branch::exchange(node);
      }

      template <class F>
      void test_insert(test_ref_t node, F comp) {
        return test_pointed_branch::insert(node, comp);
      }
      void test_remove() {
        return test_pointed_branch::remove();
      }

      /**
       * Self: Label
       */

    public:
      uint64_t value;

      /**
       * Self: Structure Exposure
       */

    public:
      test_colors_t& raw_color() {
        return _color;
      }

      test_sides_t& raw_side() {
        return _side;
      }

      test_ref_t& raw_head() {
        return _head;
      }

      test_ref_t& raw_prev() {
        return _prev;
      }

      test_ref_t& raw_next() {
        return _next;
      }
    };

  protected:
    static constexpr unsigned int count = 128;

    std::vector<test_pointed_node> _test_pointed_nodes;
    test_pointed_node* _test_pointed_root;

    void SetUp() override {
      _test_pointed_nodes.reserve(count);
      for (auto value = 0u; value < count; value++) {
        _test_pointed_nodes.emplace_back(value);
      }
      _test_pointed_root = &_test_pointed_nodes[0];
    }

  public:
    test_pointed_node* ptr(int node_idx) {
      return &_test_pointed_nodes[node_idx];
    }

    test_pointed_node::test_sides_t root() {
      return test_pointed_node::test_sides_t::root;
    }

    test_pointed_node::test_sides_t prev() {
      return test_pointed_node::test_sides_t::prev;
    }

    test_pointed_node::test_sides_t next() {
      return test_pointed_node::test_sides_t::next;
    }

    test_pointed_node::test_ref_t ref(test_pointed_node* node_ptr) {
      return test_pointed_node::test_ref_t{node_ptr};
    }

    test_pointed_node::test_ref_t ref() {
      return test_pointed_node::test_ref_t{};
    }

    test_pointed_node* tree(
        std::optional<std::variant<int, test_pointed_node*>> prev_opt,
        std::variant<int, test_pointed_node*> head_val,
        std::optional<std::variant<int, test_pointed_node*>> next_opt
    ) {
      test_pointed_node* head = nullptr;
      if (std::holds_alternative<test_pointed_node*>(head_val)) {
        head = std::get<test_pointed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = ptr(prev_idx);
      }
      auto head_ref = test_pointed_node::test_ref_t{head};

      test_pointed_node* prev = nullptr;
      if (prev_opt) {
        auto& prev_val = prev_opt.value();
        if (std::holds_alternative<test_pointed_node*>(prev_val)) {
          prev = std::get<test_pointed_node*>(prev_val);
        } else {
          auto prev_idx = std::get<int>(prev_val);
          prev = ptr(prev_idx);
        }
      }
      auto prev_ref = test_pointed_node::test_ref_t{prev};

      test_pointed_node* next = nullptr;
      if (next_opt) {
        auto& next_val = next_opt.value();
        if (std::holds_alternative<test_pointed_node*>(next_val)) {
          next = std::get<test_pointed_node*>(next_val);
        } else {
          auto next_idx = std::get<int>(next_val);
          next = ptr(next_idx);
        }
      }
      auto next_ref = test_pointed_node::test_ref_t{next};

      if (prev && next) {
        head->raw_prev() = prev_ref;
        head->raw_next() = next_ref;

        prev->raw_head() = head_ref;
        prev->raw_side() = test_pointed_node::test_sides_t::prev;

        next->raw_head() = head_ref;
        next->raw_side() = test_pointed_node::test_sides_t::next;

//        std::cout << "head: " << head->value << " (" << head << ")" << std::endl;
//        std::cout << "  prev: " << prev->value << " (" << prev << ")" << std::endl;
//        std::cout << "  next: " << next->value << " (" << next << ")" << std::endl;
      } else if (prev) {
        head->raw_prev() = prev_ref;

        prev->raw_head() = head_ref;
        prev->raw_side() = test_pointed_node::test_sides_t::prev;

//        std::cout << "head: " << head->value << " (" << head << ")" << std::endl;
//        std::cout << "  prev: " << prev->value << " (" << prev << ")" << std::endl;
      } else if (next) {
        head->raw_next() = next_ref;

        next->raw_head() = head_ref;
        next->raw_side() = test_pointed_node::test_sides_t::next;

//        std::cout << "head: " << head->value << " (" << head << ")" << std::endl;
//        std::cout << "  next: " << next->value << " (" << next << ")" << std::endl;
      }
      return head;
    }

    int height(std::variant<int, test_pointed_node*> head_val) {
      test_pointed_node* head = nullptr;
      if (std::holds_alternative<test_pointed_node*>(head_val)) {
        head = std::get<test_pointed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = ptr(prev_idx);
      }
      auto head_ref = test_pointed_node::test_ref_t{head};

      int depth = head->test_is_black() ? 1 : 0;

      auto* prev = static_cast<test_pointed_node*>(head->test_prev()());
      auto* next = static_cast<test_pointed_node*>(head->test_next()());

      if (prev && next) {
        int prev_height = height(prev);
        int next_height = height(next);

        if (prev_height >= 0 && next_height >= 0 && prev_height == next_height) {
          depth += (prev_height + next_height) / 2;
        } else {
          std::stringstream message{};
          message << "rb error (" << head->value << "):\n";

          if (prev) {
            message << "  prev (" << prev->value << ") black height is " << prev;
          } else {
            message << "  prev (null) black height is 0";
          }

          if (next) {
            message << "  next (" << next->value << ") black height is " << next;
          } else {
            message << "  next (null) black height is 0";
          }

          throw std::logic_error(message.str());
        }
      } else if (prev) {
        depth += height(prev);
      } else if (next) {
        depth += height(next);
      }

      return depth;
    }
  };

  TEST_F(BranchTest, EmptyBranch) {
    EXPECT_TRUE(_test_pointed_nodes[0].test_alone());
    EXPECT_TRUE(_test_pointed_nodes[0].test_is_black());
  }

  TEST_F(BranchTest, BlackHeightWorks) {
    tree(0, 1, 2);

    ptr(0)->test_make_red();
    ptr(2)->test_make_red();

    EXPECT_EQ(height(ptr(1)), 1);

    ptr(2)->test_make_black();

    EXPECT_THROW(height(ptr(1)), std::logic_error);
  }

  TEST_F(BranchTest, MakeHeadNull) {
    tree(0, 1, tree(2, 3, tree(4, 5, {})));
    tree(tree(6, 7, 8), 9, tree({}, 10, 11));

    EXPECT_EQ(ptr(9)->test_next()(), ptr(10));
    EXPECT_EQ(ptr(9), ptr(10)->test_head()());

    ptr(10)->test_make_head(ref(), root());

    EXPECT_EQ(ptr(9)->test_next()(), nullptr);
    EXPECT_EQ(ptr(10)->test_head()(), nullptr);
  }

  TEST_F(BranchTest, MakeHeadNext) {
    tree(0, 1, tree(2, 3, tree(4, 5, {})));
    tree(tree(6, 7, 8), 9, tree({}, 10, 11));

    EXPECT_EQ(ptr(3)->test_next()(), ptr(5));
    EXPECT_EQ(ptr(3), ptr(5)->test_head()());

    EXPECT_EQ(ptr(9)->test_prev()(), ptr(7));
    EXPECT_EQ(ptr(9), ptr(7)->test_head()());

    ptr(7)->test_make_head(ref(ptr(3)), next());

    EXPECT_EQ(ptr(5)->test_head()(), nullptr);
    EXPECT_EQ(ptr(9)->test_prev()(), nullptr);

    EXPECT_EQ(ptr(3)->test_next()(), ptr(7));
    EXPECT_EQ(ptr(3), ptr(7)->test_head()());
  }

  TEST_F(BranchTest, MakeChildNull) {
    tree(0, 1, tree(2, 3, tree(4, 5, {})));
    tree(tree(6, 7, 8), 9, tree({}, 10, 11));

    EXPECT_EQ(ptr(3)->test_prev()(), ptr(2));
    EXPECT_EQ(ptr(3), ptr(2)->test_head()());

    ptr(3)->test_make_child(ref(), prev());

    EXPECT_EQ(ptr(3)->test_prev()(), nullptr);
    EXPECT_EQ(ptr(2)->test_head()(), nullptr);
  }

  TEST_F(BranchTest, MakeChildPrev) {
    tree(0, 1, tree(2, 3, tree(4, 5, {})));
    tree(tree(6, 7, 8), 9, tree({}, 10, 11));

    EXPECT_EQ(ptr(3)->test_next()(), ptr(5));
    EXPECT_EQ(ptr(3), ptr(5)->test_head()());

    EXPECT_EQ(ptr(9)->test_prev()(), ptr(7));
    EXPECT_EQ(ptr(9), ptr(7)->test_head()());

    ptr(9)->test_make_child(ref(ptr(5)), prev());

    EXPECT_EQ(ptr(3)->test_next()(), nullptr);
    EXPECT_EQ(ptr(7)->test_head()(), nullptr);

    EXPECT_EQ(ptr(9)->test_prev()(), ptr(5));
    EXPECT_EQ(ptr(9), ptr(5)->test_head()());
  }

  TEST_F(BranchTest, MakePrev) {
    tree(0, 1, tree(2, 3, tree(4, 5, {})));
    tree(tree(6, 7, 8), 9, tree({}, 10, 11));

    EXPECT_EQ(ptr(1)->test_next()(), ptr(3));
    EXPECT_EQ(ptr(1), ptr(3)->test_head()());

    EXPECT_EQ(ptr(9)->test_prev()(), ptr(7));
    EXPECT_EQ(ptr(9), ptr(7)->test_head()());

    ptr(9)->test_make_child(ref(ptr(5)), prev());

    EXPECT_EQ(ptr(3)->test_next()(), nullptr);
    EXPECT_EQ(ptr(7)->test_head()(), nullptr);

    EXPECT_EQ(ptr(9)->test_prev()(), ptr(5));
    EXPECT_EQ(ptr(9), ptr(5)->test_head()());
  }

  TEST_F(BranchTest, SimpleEasyBranch) {
    auto comp = [](test_pointed_branch* left, test_pointed_branch* right) {
      auto* left_ptr = static_cast<test_pointed_node*>(left);
      auto* right_ptr = static_cast<test_pointed_node*>(right);

      return left_ptr->value < right_ptr->value;
    };

    _test_pointed_root = ptr(3);
    _test_pointed_root->test_insert(ref(ptr(1)), comp);
    _test_pointed_root->test_insert(ref(ptr(5)), comp);
    _test_pointed_root->test_insert(ref(ptr(0)), comp);
    _test_pointed_root->test_insert(ref(ptr(2)), comp);
    _test_pointed_root->test_insert(ref(ptr(4)), comp);
    _test_pointed_root->test_insert(ref(ptr(6)), comp);

    try {
      EXPECT_EQ(height(3), 2);
    } catch (const std::logic_error& failure) {
      FAIL() << failure.what();
    }

    EXPECT_FALSE(ptr(0)->test_alone());
    EXPECT_TRUE(ptr(0)->test_is_red());
    EXPECT_EQ(ptr(0)->test_side(), prev());

    EXPECT_FALSE(ptr(1)->test_alone());
    EXPECT_TRUE(ptr(1)->test_is_black());
    EXPECT_EQ(ptr(1)->test_side(), prev());

    EXPECT_FALSE(ptr(2)->test_alone());
    EXPECT_TRUE(ptr(2)->test_is_red());
    EXPECT_EQ(ptr(2)->test_side(), next());

    EXPECT_FALSE(ptr(3)->test_alone());
    EXPECT_TRUE(ptr(3)->test_is_black());
    EXPECT_EQ(ptr(3)->test_side(), root());

    EXPECT_FALSE(ptr(4)->test_alone());
    EXPECT_TRUE(ptr(4)->test_is_red());
    EXPECT_EQ(ptr(4)->test_side(), prev());

    EXPECT_FALSE(ptr(5)->test_alone());
    EXPECT_TRUE(ptr(5)->test_is_black());
    EXPECT_EQ(ptr(5)->test_side(), next());

    EXPECT_FALSE(ptr(6)->test_alone());
    EXPECT_TRUE(ptr(6)->test_is_red());
    EXPECT_EQ(ptr(6)->test_side(), next());

    EXPECT_EQ(ptr(3)->test_minimum()(), ptr(0));
    EXPECT_EQ(ptr(1)->test_minimum()(), ptr(0));
    EXPECT_EQ(ptr(5)->test_minimum()(), ptr(4));

    for (auto index = 0u; index < 6; index++) {
      EXPECT_EQ(ptr(index)->test_successor()(), ptr(index + 1));
      EXPECT_EQ(ptr(index)->test_root()(), ptr(3));
    }

    EXPECT_EQ(ptr(3)->test_maximum()(), ptr(6));
    EXPECT_EQ(ptr(1)->test_maximum()(), ptr(2));
    EXPECT_EQ(ptr(5)->test_maximum()(), ptr(6));

    for (auto index = 6u; index < 0; index++) {
      EXPECT_EQ(ptr(index)->test_predecessor()(), ptr(index - 1));
      EXPECT_EQ(ptr(index)->test_root()(), ptr(3));
    }
  }

  TEST_F(BranchTest, SimpleTougherBranch) {
    auto comp = [](test_pointed_branch* left, test_pointed_branch* right) {
      auto* left_ptr = static_cast<test_pointed_node*>(left);
      auto* right_ptr = static_cast<test_pointed_node*>(right);

      return left_ptr->value < right_ptr->value;
    };

    int this_height = height(_test_pointed_root);
    for (auto index = 1u; index < count; index++) {
      _test_pointed_root->test_insert(ref(ptr(index)), comp);
      _test_pointed_root = static_cast<test_pointed_node*>(_test_pointed_root->test_root()());
      int next_height;
      try {
        next_height = height(_test_pointed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_LE(this_height, next_height);
      this_height = next_height;
    }

    EXPECT_EQ(this_height, 6);

    for (auto index = 1u; index < count; index++) {
      auto* minimum = static_cast<test_pointed_node*>(_test_pointed_root->test_minimum()());
      minimum->test_remove();
      _test_pointed_root = static_cast<test_pointed_node*>(_test_pointed_root->test_root()());
      int next_height;
      try {
        next_height = height(_test_pointed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_GE(this_height, next_height);
      this_height = next_height;
    }
  }

} // namespace hatch