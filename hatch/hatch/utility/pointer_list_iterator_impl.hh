#ifndef HATCH_POINTER_LIST_ITERATOR_IMPL_HH
#define HATCH_POINTER_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_iterator_impl.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_iterator<T>::pointer_list_iterator(pointer_list_node<T>* root, pointer_list_node<T>* node) :
      _root{root},
      _node{node} {
  }

  template <class T>
  bool pointer_list_iterator<T>::operator!=(const pointer_list_iterator& compared) const {
    return _node != compared._node;
  }

  template <class T>
  T& pointer_list_iterator<T>::operator*() {
    return static_cast<T&>(*_node);
  }

  template <class T>
  std::add_const_t<T>& pointer_list_iterator<T>::operator*() const {
    return static_cast<std::add_const_t<T>&>(*_node);
  }

  template <class T>
  T* pointer_list_iterator<T>::operator->() {
    return static_cast<T*>(_node);
  }

  template <class T>
  std::add_const_t<T>* pointer_list_iterator<T>::operator->() const {
    return static_cast<std::add_const_t<T>*>(_node);
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() {
    if (_node) {
      _node = _node->_next;
      if (_node == _root) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() const {
    if (_node) {
      _node = _node->_next;
      if (_node == _root) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() {
    if (_node) {
      _node = _node->_prev;
      if (_node == _root) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() const {
    if (_node) {
      _node = _node->_prev;
      if (_node == _root) {
        _node = nullptr;
      }
    }
    return *this;
  }

}

#endif // HATCH_POINTER_LIST_ITERATOR_IMPL_HH
