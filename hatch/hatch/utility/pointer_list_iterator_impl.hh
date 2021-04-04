#ifndef HATCH_POINTER_LIST_ITERATOR_IMPL_HH
#define HATCH_POINTER_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_iterator_impl.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_iterator<T>::pointer_list_iterator(pointer_list_root<T>* root, pointer_list_node<T>* node) :
      _root{root},
      _node{node} {
  }

  template <class T>
  pointer_list_iterator<T>::pointer_list_iterator(const pointer_list_iterator& copied) :
      _root{copied._root},
      _node{copied._node} {
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator=(const pointer_list_iterator& copied) {
    _root = copied._root;
    _node = copied._node;
    return *this;
  }

  template <class T>
  bool pointer_list_iterator<T>::operator!=(const pointer_list_iterator& compared) const {
    return _node != compared._node;
  }


  template <class T>
  pointer_list_iterator<T> pointer_list_iterator<T>::insert(pointer_list_root<T>& list) {
    auto* target = list._head;
    if (target) {
      if (_node) {
        _node->splice(*target);
      } else {
        _root->_head->splice(*target);
      }
      list._head = nullptr;
    }
    return {_root, target};
  }

  template <class T>
  pointer_list_root<T> pointer_list_iterator<T>::slice(pointer_list_iterator<T>& other) {
    if (_root == other._root) {
      // this iterator and the other point to the same list.

      if (_node != other._node) {
        // this iterator and the other do not point to the same place in this list.

        if (_node) {
          // this iterator does not point past the end of this list.
          pointer_list_root<T> list{_node};
          pointer_list_node<T>* target;

          if (other._node) {
            // the other iterator points does not point past the end of this list.
            target = other._node;
            if (_node == _root->_head) {
              _root->_head = target;
            }

            // this iterator may lie between the other and the end of the list. if so, return an empty list.
            for (auto* node = target; node != _root->_head; node = node->_next) {
              if (node = _node) {
                return {};
              }
            }
          } else {
            // the other iterator points to the end of this list, so point the target at the head.
            target = _root->_head;
            if (_node == _root->_head) {
              _root->_head = nullptr;
            }
          }

          _node->splice(*target);
          _root = &list;
          return list;
        }
      }
    }
    return {};
  }

  template <class T>
  T& pointer_list_iterator<T>::operator*() const {
    return _node->get();
  }

  template <class T>
  T* pointer_list_iterator<T>::operator->() const {
    return &_node->get();
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() {
    if (_node) {
      _node = _node->_next;
      if (_node == _root->_head) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() {
    if (_node) {
      _node = _node->_prev;
      if (_node == _root->_head) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator++() const {
    if (_node) {
      _node = _node->_next;
      if (_node == _root->_head) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() const {
    if (_node) {
      _node = _node->_prev;
      if (_node == _root->_head) {
        _node = nullptr;
      }
    }
    return *this;
  }

  template <class T>
  const pointer_list_iterator<T> pointer_list_iterator<T>::operator++(int) const {
    const auto* node = _node;
    operator++();
    return {_root, node};
  }

  template <class T>
  const pointer_list_iterator<T> pointer_list_iterator<T>::operator--(int) const {
    const auto* node = _node;
    operator--();
    return {_root, node};
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_ITERATOR_IMPL_HH
