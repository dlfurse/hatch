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

    void rotate_prev(pointer_tree* here);
    void rotate_next(pointer_tree* here);

    void insert_binary(pointer_tree* here);
    void insert_fix(pointer_tree* here);

    int black_depth();

  public:
    T& operator*();
    const T& operator*() const;

    void insert(pointer_tree& tree);
    void detach();
  };

} // namespace hatch

#endif // HATCH_POINTER_TREE_HH
