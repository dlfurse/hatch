#ifndef HATCH_INDEXED_LIST_ITERATOR_IMPL_HH
#define HATCH_INDEXED_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_INDEXED_LIST_HH
#error "do not include indexed_list_iterator_impl.hh directly. include indexed_list.hh instead."
#endif

namespace hatch {

  template <class T>
  indexed_list_iterator<T>::indexed_list_iterator(T* data, int64_t root, int64_t node) :
      _data{data},
      _root{root},
      _node{node} {
  }

  template <class T>
  bool indexed_list_iterator<T>::operator!=(const indexed_list_iterator& compared) const {
    return _node != compared._node;
  }

  template <class T>
  T& indexed_list_iterator<T>::operator*() {
    return static_cast<T&>(_data[_node]);
  }

  template <class T>
  std::add_const_t<T>& indexed_list_iterator<T>::operator*() const {
    return static_cast<std::add_const_t<T>&>(_data[_node]);
  }

  template <class T>
  T* indexed_list_iterator<T>::operator->() {
    return static_cast<T*>(&_data[_node]);
  }

  template <class T>
  std::add_const_t<T>* indexed_list_iterator<T>::operator->() const {
    return static_cast<std::add_const_t<T>*>(&_data[_node]);
  }

  template <class T>
  indexed_list_iterator<T>& indexed_list_iterator<T>::operator++() {
    if (_node >= 0) {
      _node = _data[_node]._next;
      if (_node == _root) {
        _node = -1;
      }
    }
    return *this;
  }

  template <class T>
  const indexed_list_iterator<T>& indexed_list_iterator<T>::operator++() const {
    if (_node >= 0) {
      _node = _data[_node]._next;
      if (_node == _root) {
        _node = -1;
      }
    }
    return *this;
  }

  template <class T>
  indexed_list_iterator<T>& indexed_list_iterator<T>::operator--() {
    if (_node >= 0) {
      _node = _data[_node]._prev;
      if (_node == _root) {
        _node = -1;
      }
    }
    return *this;
  }

  template <class T>
  const indexed_list_iterator<T>& indexed_list_iterator<T>::operator--() const {
    if (_node >= 0) {
      _node = _data[_node]._prev;
      if (_node == _root) {
        _node = -1;
      }
    }
    return *this;
  }
  
} // namespace hatch

#endif // HATCH_INDEXED_LIST_ITERATOR_IMPL_HH
