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
    explicit pointer_list_iterator(pointer_list_root<T>* root, pointer_list_node<T>* node);

  public:
    pointer_list_iterator() = delete;
    ~pointer_list_iterator() = default;

    pointer_list_iterator(pointer_list_iterator&& moved) = delete;
    pointer_list_iterator& operator=(pointer_list_iterator&& moved) = delete;

    pointer_list_iterator(const pointer_list_iterator& copied);
    pointer_list_iterator& operator=(const pointer_list_iterator& copied);

  private:
    pointer_list_root<T>* _root;
    mutable pointer_list_node<T>* _node;

  public:
    pointer_list_iterator insert(pointer_list_root<T>& list);
    pointer_list_root<T> slice(pointer_list_iterator& end);

  public:
    [[nodiscard]] bool operator!=(const pointer_list_iterator& compared) const;

    T& operator*() const;
    T* operator->() const;

    pointer_list_iterator& operator++();
    pointer_list_iterator& operator--();

    const pointer_list_iterator& operator++() const;
    const pointer_list_iterator& operator--() const;

    const pointer_list_iterator operator++(int) const;
    const pointer_list_iterator operator--(int) const;
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_ITERATOR_HH
