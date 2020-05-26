#ifndef HATCH_POINTER_LIST_ITERATOR_HH
#define HATCH_POINTER_LIST_ITERATOR_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_list_iterator.hh directly. include structures.hh instead."
#endif

#include <type_traits>

namespace hatch {

  template <class T>
  class pointer_list_iterator {
  private:
    mutable pointer_list<T>* _here;
    mutable pointer_list<T>* _root;

    pointer_list_iterator(pointer_list<T>* here, pointer_list<T>* root);

  public:
    friend class pointer_list<T>;
    static_assert(std::is_base_of_v<pointer_list<T>, T>);

    bool operator!=(const pointer_list_iterator& compared) const;

    T& operator*();
    std::add_const_t<T>& operator*() const;

    pointer_list_iterator& operator++();
    const pointer_list_iterator& operator++() const;

    pointer_list_iterator& operator--();
    const pointer_list_iterator& operator--() const;
  };

}

#endif // HATCH_POINTER_LIST_ITERATOR_HH
