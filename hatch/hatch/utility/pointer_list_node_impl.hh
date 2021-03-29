#ifndef HATCH_POINTER_LIST_NODE_IMPL_HH
#define HATCH_POINTER_LIST_NODE_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_node_impl.hh directly. include pointer_list_node.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_node<T>::pointer_list_node() :
      _prev{nullptr},
      _next{nullptr} {
  }

  template <class T>
  pointer_list_node<T>::~pointer_list_node() {
    detach();
  }

  template <class T>
  T& pointer_list_node<T>::data() {
    static_assert(std::is_base_of_v<pointer_list_node<T>, T>);
    return static_cast<T&>(*this);
  }

  template <class T>
  const T& pointer_list_node<T>::data() const {
    static_assert(std::is_base_of_v<pointer_list_node<T>, T>);
    return static_cast<const T&>(*this);
  }

  template <class T>
  const pointer_list_node<T>* pointer_list_node<T>::prev() const {
    return _prev;
  }

  template <class T>
  const pointer_list_node<T>* pointer_list_node<T>::next() const {
    return _next;
  }

  template <class T>
  bool pointer_list_node<T>::detach() {
    if (!detached()) {
      auto* prev = _prev;
      auto* next = _next;

      if (prev != next) {
        prev->_next = next;
        next->_prev = prev;
      } else {
        prev->_next = nullptr;
        next->_prev = nullptr;
      }

      _prev = nullptr;
      _next = nullptr;

      return true;
    }
    return false;
  }

  template <class T>
  bool pointer_list_node<T>::detached() const {
    return _prev == nullptr && _next == nullptr;
  }

  template <class T>
  void pointer_list_node<T>::insert_replacing(pointer_list_node& item) {
    detach();

    auto* prev = item._prev;
    auto* next = item._next;

    if (!item.detached()) {
      item.detach();

      this->_next = next;
      next->_prev = this;

      this->_prev = prev;
      prev->_next = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::splice_replacing(pointer_list_node& list) {
    auto* prev = list._prev;
    auto* next = list._next;

    if (!list.detached()) {
      list.detach();

      _prev->_next = next;
      next->_prev = _prev;

      this->_prev = prev;
      prev->_next = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::insert_before(pointer_list_node& item) {
    detach();

    auto* prev = item._prev;

    if (!item.detached()) {
      this->_prev = prev;
      prev->_next = this;

      this->_next = &item;
      item._prev = this;
    } else {
      this->_prev = &item;
      item._next = this;

      this->_next = &item;
      item._prev = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::splice_before(pointer_list_node& list) {
    auto* prev = list._prev;

    if (!list.detached()) {
      _prev->_next = &list;
      list._prev = _prev;

      this->_prev = prev;
      prev->_next = this;
    } else {
      _prev->_next = &list;
      list._prev = _prev;

      this->_prev = &list;
      list._next = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::insert_after(pointer_list_node& item) {
    detach();

    auto* next = item._next;

    if (!item.detached()) {
      this->_prev = &item;
      item._next = this;

      this->_next = next;
      next->_prev = this;
    } else {
      this->_prev = &item;
      item._next = this;

      this->_next = &item;
      item._prev = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::splice_after(pointer_list_node& list) {
    auto* next = list._next;

    if (!list.detached()) {
      _prev->_next = next;
      next->_prev = _prev;

      this->_prev = &list;
      list._next = this;
    } else {
      _prev->_next = &list;
      list._prev = this;

      this->_prev = &list;
      list._next = this;
    }
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_NODE_IMPL_HH
