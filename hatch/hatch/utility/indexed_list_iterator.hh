#ifndef HATCH_INDEXED_LIST_ITERATOR_HH
#define HATCH_INDEXED_LIST_ITERATOR_HH

#ifndef HATCH_INDEXED_LIST_HH
#error "do not include indexed_list_iterator.hh directly. include indexed_list.hh instead."
#endif

#include <type_traits>

#include <cstdint>

namespace hatch {
  
  template <class T>
  class indexed_list_iterator {
  private:
    mutable T* _data;
    mutable int64_t _root;
    mutable int64_t _node;

    indexed_list_iterator(T* data, int64_t root, int64_t node);

  public:
    friend class indexed_list_node<T>;
    static_assert(std::is_base_of_v<indexed_list_node<T>, T>);

    bool operator!=(const indexed_list_iterator& compared) const;

    T& operator*();
    std::add_const_t<T>& operator*() const;

    T* operator->();
    std::add_const_t<T>* operator->() const;

    indexed_list_iterator& operator++();
    const indexed_list_iterator& operator++() const;

    indexed_list_iterator& operator--();
    const indexed_list_iterator& operator--() const;
  };
  
} // namespace hatch

#endif // HATCH_INDEXED_LIST_ITERATOR_HH
