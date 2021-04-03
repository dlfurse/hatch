#ifndef HATCH_POINTER_LIST_ITERATOR_HH
#define HATCH_POINTER_LIST_ITERATOR_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_iterator.hh directly. include pointer_list.hh instead."
#endif

#include <type_traits>

namespace hatch {

  template <class T>
  class pointer_list_iterator {
  public:
    friend class pointer_list_root<T>;

  private:
    mutable pointer_list_root<T>* _root;
    mutable pointer_list_node<T>* _node;

    pointer_list_iterator(pointer_list_root<T>* root, pointer_list_node<T>* node);

  public:
    bool operator!=(const pointer_list_iterator& compared) const;

    T& operator*();
    std::add_const_t<T>& operator*() const;

    T* operator->();
    std::add_const_t<T>* operator->() const;

    pointer_list_iterator& operator++();
    const pointer_list_iterator& operator++() const;

    pointer_list_iterator& operator--();
    const pointer_list_iterator& operator--() const;
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_ITERATOR_HH
