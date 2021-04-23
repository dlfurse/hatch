#ifndef HATCH_TREE_ITERATOR_HH
#define HATCH_TREE_ITERATOR_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_iterator.hh directly. include tree.hh instead."
#endif

#include <type_traits>

namespace hatch {

  template <class T>
  class tree_iterator {
  public:
    friend class tree<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  private:
    tree_iterator(tree<T>* tree, tree_node<T>* node);

  public:
    tree_iterator() = delete;
    ~tree_iterator() = default;

    tree_iterator(tree_iterator&& moved) = delete;
    tree_iterator& operator=(tree_iterator&& moved) = delete;

    tree_iterator(const tree_iterator& copied);
    tree_iterator& operator=(const tree_iterator& copied);

    //////////////////
    // Comparisons. //
    //////////////////
  
  public:
    bool operator==(const tree_iterator& compared) const;
    bool operator!=(const tree_iterator& compared) const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    tree<T>* _tree;
    mutable tree_node<T>* _node;

    static tree_node<T>* _before;
    static tree_node<T>* _after;

    /////////////////////////////////////
    // Structure: get underlying data. //
    /////////////////////////////////////

  public:
    T& operator*() const;
    T* operator->() const;

    ///////////////////////////////
    // Structure: move iterator. //
    ///////////////////////////////

  public:
    tree_iterator& operator++();
    const tree_iterator& operator++() const;
    const tree_iterator operator++(int) const;

    tree_iterator& operator--();
    const tree_iterator& operator--() const;
    const tree_iterator operator--(int) const;

    ////////////////////////////////////////
    // Structure: mutate underlying tree. //
    ////////////////////////////////////////

  public:
    tree<T> remove();
  };

} // namespace hatch

#endif // HATCH_TREE_ITERATOR_HH
