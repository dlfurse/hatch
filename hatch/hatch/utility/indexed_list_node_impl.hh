#ifndef HATCH_INDEXED_LIST_NODE_IMPL_HH
#define HATCH_INDEXED_LIST_NODE_IMPL_HH

#ifndef HATCH_INDEXED_LIST_HH
#error "do not include indexed_list_node_impl.hh directly. include indexed_list_impl.hh instead."
#endif

namespace hatch {

  template <class T>
  indexed_list_node<T>::indexed_list_node() :
      _prev{-1},
      _next{-1} {
  }

  template <class T>
  indexed_list_node<T>::~indexed_list_node() {
  }

  template <class T>
  indexed_list_iterator<T> indexed_list_node<T>::begin(T* data) {
    auto here = static_cast<T*>(this) - data;
    return {data, here, here};
  }

  template <class T>
  indexed_list_iterator<T> indexed_list_node<T>::end(T* data) {
    auto here = static_cast<T*>(this) - data;
    return {data, here, -1};
  }

  template <class T>
  bool indexed_list_node<T>::detached() const {
    return _prev < 0 && _next < 0;
  }

  template <class T>
  bool indexed_list_node<T>::detach(T* array) {
    if (!detached()) {
      auto prev = _prev;
      auto next = _next;

      if (prev != next) {
        array[next]._prev = prev;
        array[prev]._next = next;
      } else {
        array[next]._prev = -1;
        array[prev]._next = -1;
      }

      _prev = -1;
      _next = -1;

      return true;
    }
    return false;
  }

  template <class T>
  void indexed_list_node<T>::insert_replacing(T* data, indexed_list_node& item) {
    detach(data);

    auto here = static_cast<T*>(this) - data;
    auto prev = item._prev;
    auto next = item._next;

    if (!item.detached()) {
      item.detach(data);

      data[next]._prev = here;
      data[here]._next = next;

      data[here]._prev = prev;
      data[prev]._next = here;
    }
  }

  template <class T>
  void indexed_list_node<T>::splice_replacing(T* data, indexed_list_node& item) {
    auto here = static_cast<T*>(this) - data;
    auto prev = item._prev;
    auto next = item._next;

    if (!item.detached()) {
      item.detach(data);

      data[next]._prev = _prev;
      data[_prev]._next = next;

      data[here]._prev = prev;
      data[prev]._next = here;
    }
  }

  template <class T>
  void indexed_list_node<T>::insert_before(T* data, indexed_list_node& item) {
    detach(data);

    auto here = static_cast<T*>(this) - data;
    auto next = static_cast<T*>(&item) - data;
    auto prev = item._prev;

    if (!item.detached()) {
      data[here]._prev = prev;
      data[prev]._next = here;

      data[next]._prev = here;
      data[here]._next = next;
    } else {
      data[here]._prev = next;
      data[next]._next = here;

      data[next]._prev = here;
      data[here]._next = next;
    }
  }

  template <class T>
  void indexed_list_node<T>::splice_before(T* data, indexed_list_node& item) {
    auto here = static_cast<T*>(this) - data;
    auto next = static_cast<T*>(&item) - data;
    auto prev = item._prev;

    if (!item.detached()) {
      data[next]._prev = _prev;
      data[_prev]._next = next;

      data[here]._prev = prev;
      data[prev]._next = here;
    } else {
      data[next]._prev = _prev;
      data[_prev]._next = next;

      data[here]._prev = next;
      data[next]._next = here;
    }
  }

  template <class T>
  void indexed_list_node<T>::insert_after(T* data, indexed_list_node& item) {
    detach(data);

    auto here = static_cast<T*>(this) - data;
    auto prev = static_cast<T*>(&item) - data;
    auto next = item._next;

    if (!item.detached()) {
      data[here]._prev = prev;
      data[prev]._next = here;

      data[next]._prev = here;
      data[here]._next = next;
    } else {
      data[here]._prev = prev;
      data[prev]._next = here;

      data[prev]._prev = here;
      data[here]._next = prev;
    }
  }

  template <class T>
  void indexed_list_node<T>::splice_after(T* data, indexed_list_node& item) {
    auto here = static_cast<T*>(this) - data;
    auto prev = static_cast<T*>(&item) - data;
    auto next = item._next;

    if (!item.detached()) {
      data[next]._prev = _prev;
      data[_prev]._next = next;

      data[here]._prev = prev;
      data[prev]._next = here;
    } else {
      data[prev]._prev = here;
      data[here]._next = prev;

      data[here]._prev = prev;
      data[prev]._next = here;
    }
  }

}

#endif // HATCH_INDEXED_LIST_NODE_IMPL_HH
