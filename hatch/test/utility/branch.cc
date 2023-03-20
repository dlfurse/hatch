#include <hatch/utility/branch.hh>
#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <gtest/gtest.h>

#include <array>
#include <sstream> // std::stringstream

#include <cstdint>

namespace hatch {

  class BranchTest : public ::testing::Test {

    /**
     * Indexed.
     */

  public:
    static constexpr auto test_stride = 16lu;
    static constexpr auto test_offset = 4lu;

    using test_indexed_branch = branch<indexed, widths::bits16, test_stride, test_offset>;
    using test_indexed_reference = indexed<test_indexed_branch , widths::bits16, test_stride, test_offset>;
    using test_indexed_context = test_indexed_reference::context;

    class test_indexed_node : public test_indexed_branch {
    public:
      using test_colors_t = typename test_indexed_branch::colors_t;
      using test_sides_t = typename test_indexed_branch::sides_t;
      using test_ref_t = typename test_indexed_branch::ref_t;

    public:
      test_indexed_node(uint16_t left, uint16_t right) :
          test_indexed_branch{},
          left{left},
          right{right} {
      }

      /**
       * Color.
       */

    public:
      test_colors_t test_color() const {
        return test_indexed_branch::color();
      }
      void test_make_color(test_colors_t color) {
        return test_indexed_branch::make_color(color);
      }

      bool test_is_red() const {
        return test_indexed_branch::is_red();
      }
      void test_make_red() {
        return test_indexed_branch::make_red();
      }

      bool test_is_black() const {
        return test_indexed_branch::is_black();
      }
      void test_make_black() {
        return test_indexed_branch::make_black();
      }

      /**
       * Structure.
       */

    public:
      bool test_alone() const {
        return test_indexed_branch::alone();
      }
      test_sides_t test_side() const {
        return test_indexed_branch::side();
      }

      test_ref_t test_head() {
        return test_indexed_branch::head();
      }
      test_ref_t test_head() const {
        return test_indexed_branch::head();
      }

      test_ref_t test_prev() {
        return test_indexed_branch::prev();
      }
      test_ref_t test_prev() const {
        return test_indexed_branch::prev();
      }

      test_ref_t test_next() {
        return test_indexed_branch::next();
      }
      test_ref_t test_next() const {
        return test_indexed_branch::next();
      }

      test_ref_t test_child(test_sides_t side) {
        return test_indexed_branch::child(side);
      }
      test_ref_t test_child(test_sides_t side) const {
        return test_indexed_branch::child(side);
      }

      test_ref_t test_root() {
        return test_indexed_branch::root();
      }
      test_ref_t test_root() const {
        return test_indexed_branch::root();
      }

      test_ref_t test_minimum() {
        return test_indexed_branch::minimum();
      }
      test_ref_t test_minimum() const {
        return test_indexed_branch::maximum();
      }

      test_ref_t test_maximum() {
        return test_indexed_branch::maximum();
      }
      test_ref_t test_maximum() const {
        return test_indexed_branch::maximum();
      }

      test_ref_t test_predecessor() {
        return test_indexed_branch::predecessor();
      }
      test_ref_t test_predecessor() const {
        return test_indexed_branch::predecessor();
      }

      test_ref_t test_successor() {
        return test_indexed_branch::successor();
      }
      test_ref_t test_successor() const {
        return test_indexed_branch::successor();
      }

      /**
       * Mutators.
       */

    public:
      void test_make_head(test_ref_t new_head, test_sides_t side) {
        return test_indexed_branch::make_head(std::forward<test_ref_t>(new_head), side);
      }
      void test_make_child(test_ref_t new_child, test_sides_t side) {
        return test_indexed_branch::make_child(std::forward<test_ref_t>(new_child), side);
      }
      void test_make_next(test_ref_t new_next) {
        return test_indexed_branch::make_next(std::forward<test_ref_t>(new_next));
      }
      void test_make_prev(test_ref_t new_prev) {
        return test_indexed_branch::make_prev(std::forward<test_ref_t>(new_prev));
      }

      void test_detach() {
        return test_indexed_branch::detach();
      }
      void test_rotate(test_sides_t side) {
        return test_indexed_branch::rotate(side);
      }
      void test_exchange(test_ref_t node) {
        return test_indexed_branch::exchange(node);
      }

      template <class F>
      void test_insert(test_ref_t node, F comp) {
        return test_indexed_branch::insert(node, comp);
      }
      void test_remove() {
        return test_indexed_branch::remove();
      }

      /**
       * Self: Value
       */

    public:
      uint16_t left;
      uint16_t right;

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
      test_ref_t test_head() const {
        return test_pointed_branch::head();
      }

      test_ref_t test_prev() {
        return test_pointed_branch::prev();
      }
      test_ref_t test_prev() const {
        return test_pointed_branch::prev();
      }

      test_ref_t test_next() {
        return test_pointed_branch::next();
      }
      test_ref_t test_next() const {
        return test_pointed_branch::next();
      }

      test_ref_t test_child(test_sides_t side) {
        return test_pointed_branch::child(side);
      }
      test_ref_t test_child(test_sides_t side) const {
        return test_pointed_branch::child(side);
      }

      test_ref_t test_root() {
        return test_pointed_branch::root();
      }
      test_ref_t test_root() const {
        return test_pointed_branch::root();
      }

      test_ref_t test_minimum() {
        return test_pointed_branch::minimum();
      }
      test_ref_t test_minimum() const {
        return test_pointed_branch::maximum();
      }

      test_ref_t test_maximum() {
        return test_pointed_branch::maximum();
      }
      test_ref_t test_maximum() const {
        return test_pointed_branch::maximum();
      }

      test_ref_t test_predecessor() {
        return test_pointed_branch::predecessor();
      }
      test_ref_t test_predecessor() const {
        return test_pointed_branch::predecessor();
      }

      test_ref_t test_successor() {
        return test_pointed_branch::successor();
      }
      test_ref_t test_successor() const {
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
       * Self: Value
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

    /**
     * Test initialization.
     */

  protected:
    static constexpr unsigned int test_count = 128;

    std::aligned_storage_t<test_count * test_stride, alignof(test_indexed_node)> test_indexed_memory{};
    std::byte* test_indexed_address{reinterpret_cast<std::byte*>(&test_indexed_memory)};
    std::array<test_indexed_node*, test_count> test_indexed_nodes{nullptr};
    test_indexed_node* test_indexed_root{nullptr};

    std::aligned_storage_t<test_count * sizeof(test_pointed_node), alignof(test_pointed_node)> test_pointed_memory{};
    std::byte* test_pointed_address{reinterpret_cast<std::byte*>(&test_pointed_memory)};
    std::array<test_pointed_node*, test_count> test_pointed_nodes{nullptr};
    test_pointed_node* test_pointed_root{nullptr};

    void SetUp() override {
      auto context = test_indexed_context(&test_indexed_memory);
      for (auto index = 0lu; index < test_count; ++index) {
        auto left = static_cast<uint16_t>(3 * index + 2);
        auto right = static_cast<uint16_t>(4 * index + 1);
        test_indexed_nodes[index] = new (test_indexed_address + index * test_stride + test_offset) test_indexed_node{left, right};

        auto value = static_cast<uint64_t>(2 * index + 5);
        test_pointed_nodes[index] = new (test_pointed_address + index * sizeof(test_pointed_node)) test_pointed_node{value};
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

  public:
    test_indexed_node::test_ref_t indexed_ref(int index) {
      return test_indexed_node::test_ref_t{test_indexed_nodes[index]};
    }

    test_indexed_node::test_ref_t indexed_ref() {
      return test_indexed_node::test_ref_t{};
    }

    test_pointed_node::test_ref_t pointed_ref(int index) {
      return test_pointed_node::test_ref_t{test_pointed_nodes[index]};
    }

    test_pointed_node::test_ref_t pointed_ref() {
      return test_pointed_node::test_ref_t{};
    }

    test_indexed_node* indexed_ptr(int index) {
      return test_indexed_nodes[index];
    }

    test_pointed_node* pointed_ptr(int index) {
      return test_pointed_nodes[index];
    }

    test_indexed_node* indexed_tree(
        std::optional<std::variant<int, test_indexed_node*>> prev_opt,
        std::variant<int, test_indexed_node*> head_val,
        std::optional<std::variant<int, test_indexed_node*>> next_opt
    ) {
      test_indexed_node* head = nullptr;
      if (std::holds_alternative<test_indexed_node*>(head_val)) {
        head = std::get<test_indexed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = indexed_ptr(prev_idx);
      }
      auto head_ref = test_indexed_node::test_ref_t{head};

      test_indexed_node* prev = nullptr;
      if (prev_opt) {
        auto& prev_val = prev_opt.value();
        if (std::holds_alternative<test_indexed_node*>(prev_val)) {
          prev = std::get<test_indexed_node*>(prev_val);
        } else {
          auto prev_idx = std::get<int>(prev_val);
          prev = indexed_ptr(prev_idx);
        }
      }
      auto prev_ref = test_indexed_node::test_ref_t{prev};

      test_indexed_node* next = nullptr;
      if (next_opt) {
        auto& next_val = next_opt.value();
        if (std::holds_alternative<test_indexed_node*>(next_val)) {
          next = std::get<test_indexed_node*>(next_val);
        } else {
          auto next_idx = std::get<int>(next_val);
          next = indexed_ptr(next_idx);
        }
      }
      auto next_ref = test_indexed_node::test_ref_t{next};

      if (prev && next) {
        head->raw_prev() = prev_ref;
        head->raw_next() = next_ref;

        prev->raw_head() = head_ref;
        prev->raw_side() = test_indexed_node::test_sides_t::prev;

        next->raw_head() = head_ref;
        next->raw_side() = test_indexed_node::test_sides_t::next;
      } else if (prev) {
        head->raw_prev() = prev_ref;

        prev->raw_head() = head_ref;
        prev->raw_side() = test_indexed_node::test_sides_t::prev;
      } else if (next) {
        head->raw_next() = next_ref;

        next->raw_head() = head_ref;
        next->raw_side() = test_indexed_node::test_sides_t::next;
      }
      return head;
    }

    test_pointed_node* pointed_tree(
        std::optional<std::variant<int, test_pointed_node*>> prev_opt,
        std::variant<int, test_pointed_node*> head_val,
        std::optional<std::variant<int, test_pointed_node*>> next_opt
    ) {
      test_pointed_node* head = nullptr;
      if (std::holds_alternative<test_pointed_node*>(head_val)) {
        head = std::get<test_pointed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = pointed_ptr(prev_idx);
      }
      auto head_ref = test_pointed_node::test_ref_t{head};

      test_pointed_node* prev = nullptr;
      if (prev_opt) {
        auto& prev_val = prev_opt.value();
        if (std::holds_alternative<test_pointed_node*>(prev_val)) {
          prev = std::get<test_pointed_node*>(prev_val);
        } else {
          auto prev_idx = std::get<int>(prev_val);
          prev = pointed_ptr(prev_idx);
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
          next = pointed_ptr(next_idx);
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
      } else if (prev) {
        head->raw_prev() = prev_ref;

        prev->raw_head() = head_ref;
        prev->raw_side() = test_pointed_node::test_sides_t::prev;
      } else if (next) {
        head->raw_next() = next_ref;

        next->raw_head() = head_ref;
        next->raw_side() = test_pointed_node::test_sides_t::next;
      }
      return head;
    }

    int indexed_height(std::variant<int, test_indexed_node*> head_val) {
      test_indexed_node* head = nullptr;
      if (std::holds_alternative<test_indexed_node*>(head_val)) {
        head = std::get<test_indexed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = indexed_ptr(prev_idx);
      }
      auto head_ref = test_indexed_node::test_ref_t{head};

      int depth = head->test_is_black() ? 1 : 0;

      if (head->test_prev() && head->test_next()) {
        auto* prev = static_cast<test_indexed_node*>(head->test_prev()());
        auto* next = static_cast<test_indexed_node*>(head->test_next()());

        int prev_height = indexed_height(prev);
        int next_height = indexed_height(next);

        if (prev_height >= 0 && next_height >= 0 && prev_height == next_height) {
          depth += (prev_height + next_height) / 2;
        } else {
          std::stringstream message{};
          message << "rb error (" << head->left << ", " << head->right << "):\n";

          if (prev) {
            message << "  prev (" << prev->left << ", " << prev->right << ") black height is " << prev;
          } else {
            message << "  prev (null) black height is 0";
          }

          if (next) {
            message << "  next (" << next->left << ", " << next->right << ") black height is " << next;
          } else {
            message << "  next (null) black height is 0";
          }

          throw std::logic_error(message.str());
        }
      } else if (head->test_prev()) {
        auto* prev = static_cast<test_indexed_node*>(head->test_prev()());
        depth += indexed_height(prev);
      } else if (head->test_next()) {
        auto* next = static_cast<test_indexed_node*>(head->test_next()());
        depth += indexed_height(next);
      }

      return depth;
    }

    int pointed_height(std::variant<int, test_pointed_node*> head_val) {
      test_pointed_node* head = nullptr;
      if (std::holds_alternative<test_pointed_node*>(head_val)) {
        head = std::get<test_pointed_node*>(head_val);
      } else {
        auto prev_idx = std::get<int>(head_val);
        head = pointed_ptr(prev_idx);
      }
      auto head_ref = test_pointed_node::test_ref_t{head};

      int depth = head->test_is_black() ? 1 : 0;

      if (head->test_prev() && head->test_next()) {
        auto* prev = static_cast<test_pointed_node*>(head->test_prev()());
        auto* next = static_cast<test_pointed_node*>(head->test_next()());

        int prev_height = pointed_height(prev);
        int next_height = pointed_height(next);

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
      } else if (head->test_prev()) {
        auto* prev = static_cast<test_pointed_node*>(head->test_prev()());
        depth += pointed_height(prev);
      } else if (head->test_next()) {
        auto* next = static_cast<test_pointed_node*>(head->test_next()());
        depth += pointed_height(next);
      }

      return depth;
    }
  };

  TEST_F(BranchTest, EmptyIndexedTest) {
    auto context = test_indexed_context(test_indexed_address);
    for (auto index = 0lu; index < test_count; ++index) {
      EXPECT_TRUE(test_indexed_nodes[index]->test_alone());
      EXPECT_TRUE(test_indexed_nodes[index]->test_is_black());
    }
  }

  TEST_F(BranchTest, EmptyPointedBranch) {
    for (auto index = 0lu; index < test_count; ++index) {
      EXPECT_TRUE(test_indexed_nodes[0]->test_alone());
      EXPECT_TRUE(test_pointed_nodes[0]->test_is_black());
    }
  }

  TEST_F(BranchTest, IndexedBlackHeightWorks) {
    auto context = test_indexed_context(test_indexed_address);
    indexed_tree(0, 1, 2);

    indexed_ptr(0)->test_make_red();
    indexed_ptr(2)->test_make_red();

    EXPECT_EQ(indexed_height(indexed_ptr(1)), 1);

    indexed_ptr(2)->test_make_black();

    EXPECT_THROW(indexed_height(indexed_ptr(1)), std::logic_error);
  }

  TEST_F(BranchTest, PointedBlackHeightWorks) {
    pointed_tree(0, 1, 2);

    pointed_ptr(0)->test_make_red();
    pointed_ptr(2)->test_make_red();

    EXPECT_EQ(pointed_height(pointed_ptr(1)), 1);

    pointed_ptr(2)->test_make_black();

    EXPECT_THROW(pointed_height(pointed_ptr(1)), std::logic_error);
  }

  TEST_F(BranchTest, IndexedMakeHeadNull) {
    auto context = test_indexed_context(test_indexed_address);
    indexed_tree(0, 1, indexed_tree(2, 3, indexed_tree(4, 5, {})));
    indexed_tree(indexed_tree(6, 7, 8), 9, indexed_tree({}, 10, 11));

    EXPECT_EQ(indexed_ptr(9)->test_next(), indexed_ref(10));
    EXPECT_EQ(indexed_ref(9), indexed_ptr(10)->test_head());

    indexed_ptr(10)->test_make_head(indexed_ref(), test_indexed_node::test_sides_t::root);

    EXPECT_EQ(indexed_ptr(9)->test_next(), indexed_ref());
    EXPECT_EQ(indexed_ptr(10)->test_head(), indexed_ref());
  }

  TEST_F(BranchTest, PointedMakeHeadNull) {
    pointed_tree(0, 1, pointed_tree(2, 3, pointed_tree(4, 5, {})));
    pointed_tree(pointed_tree(6, 7, 8), 9, pointed_tree({}, 10, 11));

    EXPECT_EQ(pointed_ptr(9)->test_next(), pointed_ref(10));
    EXPECT_EQ(pointed_ref(9), pointed_ptr(10)->test_head());

    pointed_ptr(10)->test_make_head(pointed_ref(), test_pointed_node::test_sides_t::root);

    EXPECT_EQ(pointed_ptr(9)->test_next(), pointed_ref());
    EXPECT_EQ(pointed_ptr(10)->test_head(), pointed_ref());
  }

  TEST_F(BranchTest, IndexedMakeHeadNext) {
    auto context = test_indexed_context(test_indexed_address);
    indexed_tree(0, 1, indexed_tree(2, 3, indexed_tree(4, 5, {})));
    indexed_tree(indexed_tree(6, 7, 8), 9, indexed_tree({}, 10, 11));

    EXPECT_EQ(indexed_ptr(3)->test_next(), indexed_ref(5));
    EXPECT_EQ(indexed_ref(3), indexed_ptr(5)->test_head());

    EXPECT_EQ(indexed_ptr(9)->test_prev(), indexed_ref(7));
    EXPECT_EQ(indexed_ref(9), indexed_ptr(7)->test_head());

    indexed_ptr(7)->test_make_head(indexed_ref(3), test_indexed_node::test_sides_t::next);

    EXPECT_EQ(indexed_ptr(5)->test_head(), indexed_ref());
    EXPECT_EQ(indexed_ptr(9)->test_prev(), indexed_ref());

    EXPECT_EQ(indexed_ptr(3)->test_next(), indexed_ref(7));
    EXPECT_EQ(indexed_ref(3), indexed_ptr(7)->test_head());
  }

  TEST_F(BranchTest, PointedMakeHeadNext) {
    pointed_tree(0, 1, pointed_tree(2, 3, pointed_tree(4, 5, {})));
    pointed_tree(pointed_tree(6, 7, 8), 9, pointed_tree({}, 10, 11));

    EXPECT_EQ(pointed_ptr(3)->test_next(), pointed_ref(5));
    EXPECT_EQ(pointed_ref(3), pointed_ptr(5)->test_head());

    EXPECT_EQ(pointed_ptr(9)->test_prev(), pointed_ref(7));
    EXPECT_EQ(pointed_ref(9), pointed_ptr(7)->test_head());

    pointed_ptr(7)->test_make_head(pointed_ref(3), test_pointed_node::test_sides_t::next);

    EXPECT_EQ(pointed_ptr(5)->test_head(), pointed_ref());
    EXPECT_EQ(pointed_ptr(9)->test_prev(), pointed_ref());

    EXPECT_EQ(pointed_ptr(3)->test_next(), pointed_ref(7));
    EXPECT_EQ(pointed_ref(3), pointed_ptr(7)->test_head());
  }

  TEST_F(BranchTest, IndexedMakeChildNull) {
    auto context = test_indexed_context(test_indexed_address);
    indexed_tree(0, 1, indexed_tree(2, 3, indexed_tree(4, 5, {})));
    indexed_tree(indexed_tree(6, 7, 8), 9, indexed_tree({}, 10, 11));

    EXPECT_EQ(indexed_ptr(3)->test_prev(), indexed_ref(2));
    EXPECT_EQ(indexed_ref(3), indexed_ptr(2)->test_head());

    indexed_ptr(3)->test_make_child(indexed_ref(), test_indexed_node::test_sides_t::prev);

    EXPECT_EQ(indexed_ptr(3)->test_prev(), indexed_ref());
    EXPECT_EQ(indexed_ptr(2)->test_head(), indexed_ref());
  }

  TEST_F(BranchTest, PointedMakeChildNull) {
    pointed_tree(0, 1, pointed_tree(2, 3, pointed_tree(4, 5, {})));
    pointed_tree(pointed_tree(6, 7, 8), 9, pointed_tree({}, 10, 11));

    EXPECT_EQ(pointed_ptr(3)->test_prev(), pointed_ref(2));
    EXPECT_EQ(pointed_ref(3), pointed_ptr(2)->test_head());

    pointed_ptr(3)->test_make_child(pointed_ref(), test_pointed_node::test_sides_t::prev);

    EXPECT_EQ(pointed_ptr(3)->test_prev(), pointed_ref());
    EXPECT_EQ(pointed_ptr(2)->test_head(), pointed_ref());
  }

  TEST_F(BranchTest, IndexedMakeChildPrev) {
    auto context = test_indexed_context(test_indexed_address);
    indexed_tree(0, 1, indexed_tree(2, 3, indexed_tree(4, 5, {})));
    indexed_tree(indexed_tree(6, 7, 8), 9, indexed_tree({}, 10, 11));

    EXPECT_EQ(indexed_ptr(3)->test_next(), indexed_ref(5));
    EXPECT_EQ(indexed_ref(3), indexed_ptr(5)->test_head());

    EXPECT_EQ(indexed_ptr(9)->test_prev(), indexed_ref(7));
    EXPECT_EQ(indexed_ref(9), indexed_ptr(7)->test_head());

    indexed_ptr(9)->test_make_child(indexed_ref(5), test_indexed_node::test_sides_t::prev);

    EXPECT_EQ(indexed_ptr(3)->test_next(), indexed_ref());
    EXPECT_EQ(indexed_ptr(7)->test_head(), indexed_ref());

    EXPECT_EQ(indexed_ptr(9)->test_prev(), indexed_ref(5));
    EXPECT_EQ(indexed_ref(9), indexed_ptr(5)->test_head());
  }

  TEST_F(BranchTest, PointedMakeChildPrev) {
    pointed_tree(0, 1, pointed_tree(2, 3, pointed_tree(4, 5, {})));
    pointed_tree(pointed_tree(6, 7, 8), 9, pointed_tree({}, 10, 11));

    EXPECT_EQ(pointed_ptr(3)->test_next(), pointed_ref(5));
    EXPECT_EQ(pointed_ref(3), pointed_ptr(5)->test_head());

    EXPECT_EQ(pointed_ptr(9)->test_prev(), pointed_ref(7));
    EXPECT_EQ(pointed_ref(9), pointed_ptr(7)->test_head());

    pointed_ptr(9)->test_make_child(pointed_ref(5), test_pointed_node::test_sides_t::prev);

    EXPECT_EQ(pointed_ptr(3)->test_next(), pointed_ref());
    EXPECT_EQ(pointed_ptr(7)->test_head(), pointed_ref());

    EXPECT_EQ(pointed_ptr(9)->test_prev(), pointed_ref(5));
    EXPECT_EQ(pointed_ref(9), pointed_ptr(5)->test_head());
  }

  TEST_F(BranchTest, PointedEasyBranch) {
    auto comp = [](test_pointed_branch* left, test_pointed_branch* right) {
      auto* left_ptr = static_cast<test_pointed_node*>(left);
      auto* right_ptr = static_cast<test_pointed_node*>(right);

      return left_ptr->value < right_ptr->value;
    };

    test_pointed_root = pointed_ptr(3);
    test_pointed_root->test_insert(pointed_ref(1), comp);
    test_pointed_root->test_insert(pointed_ref(5), comp);
    test_pointed_root->test_insert(pointed_ref(0), comp);
    test_pointed_root->test_insert(pointed_ref(2), comp);
    test_pointed_root->test_insert(pointed_ref(4), comp);
    test_pointed_root->test_insert(pointed_ref(6), comp);

    try {
      EXPECT_EQ(pointed_height(3), 2);
    } catch (const std::logic_error& failure) {
      FAIL() << failure.what();
    }

    EXPECT_FALSE(pointed_ptr(0)->test_alone());
    EXPECT_TRUE(pointed_ptr(0)->test_is_red());
    EXPECT_EQ(pointed_ptr(0)->test_side(), test_pointed_node::test_sides_t::prev);

    EXPECT_FALSE(pointed_ptr(1)->test_alone());
    EXPECT_TRUE(pointed_ptr(1)->test_is_black());
    EXPECT_EQ(pointed_ptr(1)->test_side(), test_pointed_node::test_sides_t::prev);

    EXPECT_FALSE(pointed_ptr(2)->test_alone());
    EXPECT_TRUE(pointed_ptr(2)->test_is_red());
    EXPECT_EQ(pointed_ptr(2)->test_side(), test_pointed_node::test_sides_t::next);

    EXPECT_FALSE(pointed_ptr(3)->test_alone());
    EXPECT_TRUE(pointed_ptr(3)->test_is_black());
    EXPECT_EQ(pointed_ptr(3)->test_side(), test_pointed_node::test_sides_t::root);

    EXPECT_FALSE(pointed_ptr(4)->test_alone());
    EXPECT_TRUE(pointed_ptr(4)->test_is_red());
    EXPECT_EQ(pointed_ptr(4)->test_side(), test_pointed_node::test_sides_t::prev);

    EXPECT_FALSE(pointed_ptr(5)->test_alone());
    EXPECT_TRUE(pointed_ptr(5)->test_is_black());
    EXPECT_EQ(pointed_ptr(5)->test_side(), test_pointed_node::test_sides_t::next);

    EXPECT_FALSE(pointed_ptr(6)->test_alone());
    EXPECT_TRUE(pointed_ptr(6)->test_is_red());
    EXPECT_EQ(pointed_ptr(6)->test_side(), test_pointed_node::test_sides_t::next);

    EXPECT_EQ(pointed_ptr(3)->test_minimum()(), pointed_ptr(0));
    EXPECT_EQ(pointed_ptr(1)->test_minimum()(), pointed_ptr(0));
    EXPECT_EQ(pointed_ptr(5)->test_minimum()(), pointed_ptr(4));

    for (auto index = 0u; index < 6; index++) {
      EXPECT_EQ(pointed_ptr(index)->test_successor()(), pointed_ptr(index + 1));
      EXPECT_EQ(pointed_ptr(index)->test_root()(), pointed_ptr(3));
    }

    EXPECT_EQ(pointed_ptr(3)->test_maximum()(), pointed_ptr(6));
    EXPECT_EQ(pointed_ptr(1)->test_maximum()(), pointed_ptr(2));
    EXPECT_EQ(pointed_ptr(5)->test_maximum()(), pointed_ptr(6));

    for (auto index = 6u; index < 0; index++) {
      EXPECT_EQ(pointed_ptr(index)->test_predecessor()(), pointed_ptr(index - 1));
      EXPECT_EQ(pointed_ptr(index)->test_root()(), pointed_ptr(3));
    }
  }

  TEST_F(BranchTest, IndexedTougherBranch) {
    auto context = test_indexed_context(test_indexed_address);
    auto comp = [](test_indexed_branch* left, test_indexed_branch* right) {
      auto* left_ptr = static_cast<test_indexed_node*>(left);
      auto* right_ptr = static_cast<test_indexed_node*>(right);

      return (left_ptr->left + left_ptr->right) < (right_ptr->left + right_ptr->right);
    };

    test_indexed_root = indexed_ptr(0);

    int this_height = indexed_height(test_indexed_root);
    for (auto index = 1u; index < test_count; index++) {
      test_indexed_root->test_insert(indexed_ref(index), comp);
      test_indexed_root = static_cast<test_indexed_node*>(test_indexed_root->test_root()());
      int next_height;
      try {
        next_height = indexed_height(test_indexed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_LE(this_height, next_height);
      this_height = next_height;
    }

    EXPECT_EQ(this_height, 6);

    for (auto index = 1u; index < test_count; index++) {
      auto* minimum = static_cast<test_indexed_node*>(test_indexed_root->test_minimum()());
      minimum->test_remove();
      test_indexed_root = static_cast<test_indexed_node*>(test_indexed_root->test_root()());
      int next_height;
      try {
        next_height = indexed_height(test_indexed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_GE(this_height, next_height);
      this_height = next_height;
    }
  }

  TEST_F(BranchTest, PointedTougherBranch) {
    auto comp = [](test_pointed_branch* left, test_pointed_branch* right) {
      auto* left_ptr = static_cast<test_pointed_node*>(left);
      auto* right_ptr = static_cast<test_pointed_node*>(right);

      return left_ptr->value < right_ptr->value;
    };

    test_pointed_root = pointed_ptr(0);

    int this_height = pointed_height(test_pointed_root);
    for (auto index = 1u; index < test_count; index++) {
      test_pointed_root->test_insert(pointed_ref(index), comp);
      test_pointed_root = static_cast<test_pointed_node*>(test_pointed_root->test_root()());
      int next_height;
      try {
        next_height = pointed_height(test_pointed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_LE(this_height, next_height);
      this_height = next_height;
    }

    EXPECT_EQ(this_height, 6);

    for (auto index = 1u; index < test_count; index++) {
      auto* minimum = static_cast<test_pointed_node*>(test_pointed_root->test_minimum()());
      minimum->test_remove();
      test_pointed_root = static_cast<test_pointed_node*>(test_pointed_root->test_root()());
      int next_height;
      try {
        next_height = pointed_height(test_pointed_root);
      } catch (const std::logic_error& failure) {
        FAIL() << failure.what();
      }
      EXPECT_GE(this_height, next_height);
      this_height = next_height;
    }
  }

} // namespace hatch