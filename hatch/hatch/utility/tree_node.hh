#ifndef HATCH_TREE_NODE_HH
#define HATCH_TREE_NODE_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_node.hh directly. include tree.hh instead."
#endif

#include <hatch/utility/container.hh>

#include <cstdint> // uint8_t, uint64_t
#include <optional> // std::optional

namespace hatch {

  template <class T>
  class tree_node : public container<T> {
  public:
    friend class tree<T>;
    friend class tree_iterator<T>;

  private:
    enum class colors : uint64_t {
      black = 0,
      red = 1,
    };

    enum class sides : uint8_t {
      prev = 0,
      next = 1,
    };

    ///////////////////////////////
    // Constructors, destructor. //
    ///////////////////////////////

  protected:
    tree_node();
    ~tree_node();

  public:
    tree_node(tree_node&& moved) noexcept;
    tree_node& operator=(tree_node&& moved) noexcept;

    tree_node(const tree_node&) = delete;
    tree_node& operator=(const tree_node&) = delete;

    ////////////
    // Color. //
    ////////////

  protected:
    colors _color;

  public:
    colors color() const;
    void make_color(colors color);

    bool is_red() const;
    void make_red();

    bool is_black() const;
    void make_black();

    ////////////////
    // Structure. //
    ////////////////

  protected:
    tree_node* _head;
    tree_node* _prev;
    tree_node* _next;

    ///////////////////////////
    // Structure: accessors. //
    ///////////////////////////

  public:
    bool alone() const;

    tree_node* minimum();
    const tree_node* minimum() const;

    tree_node* predecessor();
    const tree_node* predecessor() const;

    tree_node* root();
    const tree_node* root() const;

    tree_node* successor();
    const tree_node* successor() const;

    tree_node* maximum();
    const tree_node* maximum() const;

  protected:
    std::optional<sides> side() const;
    bool is_root() const;
    bool is_prev() const;
    bool is_next() const;

    tree_node* head();
    const tree_node* head() const;

    tree_node* child(sides side);
    const tree_node* child(sides side) const;

    tree_node* prev();
    const tree_node* prev() const;

    tree_node* next();
    const tree_node* next() const;

    //////////////////////////
    // Structure: mutators. //
    //////////////////////////

  protected:
    void make_head(tree_node* new_head, std::optional<sides> new_side = {});
    void make_child(tree_node* new_child, sides side);
    void make_next(tree_node* new_next);
    void make_prev(tree_node* new_prev);

    void detach();
    void rotate(sides side);
    void exchange(tree_node* node);
  };

} // namespace hatch

#endif // HATCH_TREE_NODE_HH
