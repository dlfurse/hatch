#ifndef HATCH_LIST_NODE_IMPL_HH
#define HATCH_LIST_NODE_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_node_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  template <class T>
  list_node<T>::list_node() :
      _prev{this},
      _next{this} {
  }

  template <class T>
  list_node<T>::~list_node() {
    if (!alone()) {
      splice(next());
    }
  }

  template <class T>
  list_node<T>::list_node(list_node&& moved) noexcept :
      _prev{moved._prev},
      _next{moved._next} {
    _prev->_next = this;
    _next->_prev = this;
    moved._prev = &moved;
    moved._next = &moved;
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(list_node&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    _prev->_next = this;
    _next->_prev = this;
    moved._prev = &moved;
    moved._next = &moved;
    return *this;
  }

  //////////////////////////
  // Structure: accessors //
  //////////////////////////

  template <class T>
  bool list_node<T>::alone() const {
    return _prev == this && _next == this;
  }

  template <class T>
  list_node<T>& list_node<T>::prev() {
    return *_prev;
  }

  template <class T>
  const list_node<T>& list_node<T>::prev() const {
    return *_prev;
  }

  template <class T>
  list_node<T>& list_node<T>::next() {
    return *_next;
  }

  template <class T>
  const list_node<T>& list_node<T>::next() const {
    return *_next;
  }

  /////////////////////////
  // Structure: mutators //
  /////////////////////////

  template <class T>
  void list_node<T>::splice(list_node& node) {
    auto prev = node._prev;
    auto next = &node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = this;
  }

} // namespace hatch

#endif // HATCH_LIST_NODE_IMPL_HH
