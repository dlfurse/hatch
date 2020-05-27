#ifndef HATCH_INDEXED_LIST_ITERATOR_HH
#define HATCH_INDEXED_LIST_ITERATOR_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include indexed_list_iterator.hh directly. include structures.hh instead."
#endif

#include <type_traits>

#include <cstdint>

namespace hatch {
  
  template <class T>
  class indexed_list_iterator {
  private:
    mutable int64_t _here;
    mutable int64_t _root;
    mutable T* _data;

    indexed_list_iterator(int64_t here, int64_t root, T* data);

  public:
    friend class indexed_list<T>;
    static_assert(std::is_base_of_v<indexed_list<T>, T>);

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
  
}

#endif // HATCH_INDEXED_LIST_ITERATOR_HH
