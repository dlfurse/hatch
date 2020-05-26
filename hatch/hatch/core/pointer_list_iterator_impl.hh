#ifndef HATCH_POINTER_LIST_ITERATOR_IMPL_HH
#define HATCH_POINTER_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_list_iterator_impl.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_iterator<T>::pointer_list_iterator(pointer_list<T>* here, pointer_list<T>* root) :
    _here{here},
    _root{root} {
  }

  template <class T>
  bool pointer_list_iterator<T>::operator!=(const pointer_list_iterator& compared) const {
    return _here != compared._here && _root != compared._root;
  }

  template <class T>
  T& pointer_list_iterator<T>::operator*() {
    return static_cast<T&>(*_here);
  }

  template <class T>
  std::add_const_t<T>& pointer_list_iterator<T>::operator*() const {
    return static_cast<std::add_const_t<T>&>(*_here);
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() {
    if (_here) {
      _here = _here->_next;
      if (_here == _root) {
        _here = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() const {
    if (_here) {
      _here = _here->_next;
      if (_here == _root) {
        _here = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() {
    if (_here) {
      _here = _here->_prev;
      if (_here == _root) {
        _here = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() const {
    if (_here) {
      _here = _here->_prev;
      if (_here == _root) {
        _here = nullptr;
      }
    }
    return *this;
  }

}

#endif // HATCH_POINTER_LIST_ITERATOR_IMPL_HH
