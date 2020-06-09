#ifndef HATCH_POINTER_LIST_ITERATOR_HH
#define HATCH_POINTER_LIST_ITERATOR_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_iterator.hh directly. include pointer_list.hh instead."
#endif

#include <type_traits>

namespace hatch {

  template <class T>
  class pointer_list_iterator {
  private:
    mutable pointer_list_node<T>* _root;
    mutable pointer_list_node<T>* _node;

    pointer_list_iterator(pointer_list_node<T>* root, pointer_list_node<T>* node);

  public:
    friend class pointer_list_node<T>;
    static_assert(std::is_base_of_v<pointer_list_node<T>, T>);

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

}

#endif // HATCH_POINTER_LIST_ITERATOR_HH
