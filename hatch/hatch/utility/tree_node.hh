#ifndef HATCH_TREE_NODE_HH
#define HATCH_TREE_NODE_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_node.hh directly. include tree.hh instead."
#endif

#include <hatch/utility/container.hh>

#include <cstdint> // uint8_t, uint64_t
#include <optional> // std::optional

namespace hatch {

  template <class T, template <class> class Ref>
  class tree_node : public container<T> {
  public:
    friend class tree<T, Ref>;
    friend class tree_iterator<T, Ref>;

  protected:
    enum class colors : bool {
      black,
      red,
    };

    enum class sides : bool {
      prev,
      next,
    };

    sides swap(sides side);

    ///////////////////////////////
    // Constructors, destructor. //
    ///////////////////////////////

  public:
    template <class ...Args>
    explicit tree_node(Args&&... args);
    ~tree_node();

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
    Ref<tree_node<T, Ref>> _head;
    Ref<tree_node<T, Ref>> _prev;
    Ref<tree_node<T, Ref>> _next;

    ///////////////////////////
    // Structure: accessors. //
    ///////////////////////////

  public:
    bool alone() const;

    tree_node<T, Ref>* minimum();
    const tree_node<T, Ref>* minimum() const;

    tree_node<T, Ref>* predecessor();
    const tree_node<T, Ref>* predecessor() const;

    tree_node<T, Ref>* root();
    const tree_node<T, Ref>* root() const;

    tree_node<T, Ref>* successor();
    const tree_node<T, Ref>* successor() const;

    tree_node<T, Ref>* maximum();
    const tree_node<T, Ref>* maximum() const;

  protected:
    std::optional<sides> side() const;
    bool is_root() const;
    bool is_prev() const;
    bool is_next() const;

    tree_node<T, Ref>* head();
    const tree_node<T, Ref>* head() const;

    tree_node<T, Ref>* child(sides side);
    const tree_node<T, Ref>* child(sides side) const;

    tree_node<T, Ref>* prev();
    const tree_node<T, Ref>* prev() const;

    tree_node<T, Ref>* next();
    const tree_node<T, Ref>* next() const;

    //////////////////////////
    // Structure: mutators. //
    //////////////////////////

  protected:
    void make_head(tree_node<T, Ref>* new_head, std::optional<sides> new_side);
    void make_child(tree_node<T, Ref>* new_child, sides side);
    void make_next(tree_node<T, Ref>* new_next);
    void make_prev(tree_node<T, Ref>* new_prev);

    void detach();
    void rotate(sides side);
    void exchange(tree_node<T, Ref>* node);

  protected:
    void insert(tree_node& node);
    void remove();
  };

} // namespace hatch

#endif // HATCH_TREE_NODE_HH
