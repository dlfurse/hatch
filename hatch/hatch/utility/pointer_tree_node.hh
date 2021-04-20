#ifndef HATCH_POINTER_TREE_NODE_HH
#define HATCH_POINTER_TREE_NODE_HH

#ifndef HATCH_POINTER_TREE_HH
#error "do not include pointer_tree_node.hh directly. include pointer_tree.hh instead."
#endif

#include <cstdint>
#include <optional>

namespace hatch {

  class pointer_tree {
  private:
    enum class colors : uint64_t {
      black = 0,
      red = 1,
    };

    enum class sides : uint64_t {
      prev = 0,
      next = 1,
    };

  public:
    template <class T>
    friend class pointer_tree_node;
    friend colors operator~(const colors& color);
    friend sides operator~(const sides& type);
  };

  template <class T>
  class pointer_tree_node {
  public:
    pointer_tree_node();
    ~pointer_tree_node();

    /**
     * Children.
     */

  protected:
    pointer_tree_node* _head;
    pointer_tree_node* _prev;
    pointer_tree_node* _next;

  protected:
    [[nodiscard]] std::optional<pointer_tree::sides> side() const;
    [[nodiscard]] bool is_root() const;
    [[nodiscard]] bool is_prev() const;
    [[nodiscard]] bool is_next() const;

    [[nodiscard]] pointer_tree_node* head();
    void make_head(pointer_tree_node* new_head, std::optional<pointer_tree::sides> new_side = {});

    [[nodiscard]] pointer_tree_node* child(pointer_tree::sides side);
    void make_child(pointer_tree_node* new_child, pointer_tree::sides side);

    [[nodiscard]] pointer_tree_node* prev();
    void make_prev(pointer_tree_node* new_prev);

    [[nodiscard]] pointer_tree_node* next();
    void make_next(pointer_tree_node* new_next);

    void detach();
    void rotate(pointer_tree::sides side);
    void exchange(pointer_tree_node* node);

    /**
     * Colors.
     */

  protected:
    pointer_tree::colors _color;

  public:
    [[nodiscard]] pointer_tree::colors color() const;
    void make_color(pointer_tree::colors color);

    [[nodiscard]] bool is_red() const;
    void make_red();

    [[nodiscard]] bool is_black() const;
    void make_black();

    /**
     * Public interface.
     */

  public:
    T& get() const;
    [[nodiscard]] bool alone();

    pointer_tree_node* root();

    pointer_tree_node* minimum();
    pointer_tree_node* maximum();

    pointer_tree_node* predecessor();
    pointer_tree_node* successor();

    void insert(pointer_tree_node& node);
    void remove();
  };

  inline pointer_tree::colors operator~(const pointer_tree::colors& color) {
    switch (color) {
      case pointer_tree::colors::red:
        return pointer_tree::colors::black;
      case pointer_tree::colors::black:
        return pointer_tree::colors::red;
    }
  }

  inline pointer_tree::sides operator~(const pointer_tree::sides& type) {
    switch (type) {
      case pointer_tree::sides::prev:
        return pointer_tree::sides::next;
      case pointer_tree::sides::next:
        return pointer_tree::sides::prev;
    }
  }

} // namespace hatch

#endif // HATCH_POINTER_TREE_NODE_HH
