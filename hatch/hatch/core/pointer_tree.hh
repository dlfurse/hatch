#ifndef HATCH_POINTER_TREE_HH
#define HATCH_POINTER_TREE_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_tree.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  class pointer_tree {
  protected:
    pointer_tree* _head;
    pointer_tree* _prev;
    pointer_tree* _next;

    enum class color : uint64_t {
      black = 0,
      red = 1,
    } _color;

  protected:
    pointer_tree();
    ~pointer_tree();

    void rotate_prev();
    void rotate_next();

    pointer_tree* predecessor();
    pointer_tree* successor();

    int black_depth();

  public:
    T& operator*();
    const T& operator*() const;

    bool detached();

    void detach();

    void insert(pointer_tree& node);
  };

} // namespace hatch

#endif // HATCH_POINTER_TREE_HH
