#ifndef HATCH_POINTER_LIST_NODE_IMPL_HH
#define HATCH_POINTER_LIST_NODE_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_node_impl.hh directly. include pointer_list_node.hh instead."
#endif

#include <type_traits> // is_base_of_v

namespace hatch {

  template <class T>
  pointer_list_node<T>::pointer_list_node() :
      _prev{this},
      _next{this} {
    static_assert(std::is_base_of_v<pointer_list_node<T>, T>);
  }

  template <class T>
  pointer_list_node<T>::~pointer_list_node() {
    detach();
  }

  template <class T>
  T& pointer_list_node<T>::data() {
    return static_cast<T&>(*this);
  }

  template <class T>
  const T& pointer_list_node<T>::data() const {
    return static_cast<const T&>(*this);
  }

  template <class T>
  bool pointer_list_node<T>::detached() const {
    return _prev == this && _next == this;
  }

  template <class T>
  pointer_list_node<T>* pointer_list_node<T>::prev() {
    return _prev;
  }

  template <class T>
  const pointer_list_node<T>* pointer_list_node<T>::prev() const {
    return _prev;
  }

  template <class T>
  pointer_list_node<T>* pointer_list_node<T>::next() {
    return _next;
  }

  template <class T>
  const pointer_list_node<T>* pointer_list_node<T>::next() const {
    return _next;
  }


  template <class T>
  pointer_list_node<T>& pointer_list_node<T>::detach() {
    if (!detached()) {
      auto* prev = _prev;
      auto* next = _next;

      prev->_next = next;
      next->_prev = prev;

      _prev = this;
      _next = this;
    }
    return *this;
  }

  template <class T>
  void pointer_list_node<T>::splice_before(pointer_list_node& node) {
    auto* prev = node._prev;
    auto* next = &node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = this;
  }

  template <class T>
  void pointer_list_node<T>::splice_replacing(pointer_list_node& node) {
    auto* prev = node._prev;
    auto* next = node._next;

    if (!node.detached()) {
      node.detach();

      _prev->_next = next;
      next->_prev = _prev;

      this->_prev = prev;
      prev->_next = this;
    }
  }

  template <class T>
  void pointer_list_node<T>::splice_after(pointer_list_node& node) {
    auto* prev = &node;
    auto* next = node._next;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = this;
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_NODE_IMPL_HH
