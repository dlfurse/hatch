#ifndef HATCH_INDEXED_LIST_ITERATOR_IMPL_HH
#define HATCH_INDEXED_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include indexed_list_iterator_impl.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  indexed_list_iterator<T>::indexed_list_iterator(int64_t here, int64_t root, T* data) :
      _here{here},
      _root{root},
      _data{data} {
  }

  template <class T>
  bool indexed_list_iterator<T>::operator!=(const indexed_list_iterator& compared) const {
    return _here != compared._here;
  }

  template <class T>
  T& indexed_list_iterator<T>::operator*() {
    return static_cast<T&>(_data[_here]);
  }

  template <class T>
  std::add_const_t<T>& indexed_list_iterator<T>::operator*() const {
    return static_cast<std::add_const_t<T>&>(_data[_here]);
  }

  template <class T>
  T* indexed_list_iterator<T>::operator->() {
    return static_cast<T*>(&_data[_here]);
  }

  template <class T>
  std::add_const_t<T>* indexed_list_iterator<T>::operator->() const {
    return static_cast<std::add_const_t<T>*>(&_data[_here]);
  }

  template <class T>
  indexed_list_iterator<T>& indexed_list_iterator<T>::operator++() {
    if (_here >= 0) {
      _here = _data[_here]._next;
      if (_here == _root) {
        _here = -1;
      }
    }
    return *this;
  }

  template <class T>
  const indexed_list_iterator<T>& indexed_list_iterator<T>::operator++() const {
    if (_here >= 0) {
      _here = _data[_here]._next;
      if (_here == _root) {
        _here = -1;
      }
    }
    return *this;
  }

  template <class T>
  indexed_list_iterator<T>& indexed_list_iterator<T>::operator--() {
    if (_here >= 0) {
      _here = _data[_here]._prev;
      if (_here == _root) {
        _here = -1;
      }
    }
    return *this;
  }

  template <class T>
  const indexed_list_iterator<T>& indexed_list_iterator<T>::operator--() const {
    if (_here >= 0) {
      _here = _data[_here]._prev;
      if (_here == _root) {
        _here = -1;
      }
    }
    return *this;
  }
  
}

#endif // HATCH_INDEXED_LIST_ITERATOR_IMPL_HH
