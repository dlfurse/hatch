#ifndef HATCH_POINTER_LIST_ITERATOR_IMPL_HH
#define HATCH_POINTER_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_iterator_impl.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

//  template <class T>
//  pointer_list_node<T> pointer_list_iterator<T>::_before{};
//
//  template <class T>
//  pointer_list_node<T> pointer_list_iterator<T>::_after{};

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
  bool pointer_list_iterator<T>::operator==(const pointer_list_iterator& compared) const {
    return _node == compared._node;
  }

  template <class T>
  bool pointer_list_iterator<T>::operator!=(const pointer_list_iterator& compared) const {
    return _node != compared._node;
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_iterator<T>::insert(pointer_list_root<T>& list) {
    if (_root != &list) {

      auto* const first_inserted = list._head;
      auto* const end_inserted = _node;

      if (first_inserted) {
        if (end_inserted) {
          end_inserted->splice(*first_inserted);
        } else {
          _root->_head->splice(*first_inserted);
        }

        if (end_inserted == _root->_head) {
          _root->_head = first_inserted;
        }

        pointer_list_iterator<T> result{_root, first_inserted};
        list._head = nullptr;
        return result;
      }
    }
    return {*this};
  }

  template <class T>
  pointer_list_root<T> pointer_list_iterator<T>::remove(pointer_list_iterator<T>& other) {
    if (_root == other._root) {

      auto* const first_removed = _node;
      auto* const end_removed = other._node;

      if (first_removed && first_removed != end_removed) {
        if (end_removed) {
          auto* node = end_removed;
          do {
            if (node == first_removed) {
              return {};
            }
            node = node->_next;
          } while (node != _root->_head);

          first_removed->splice(*end_removed);
        } else {
          first_removed->splice(*_root->_head);
        }

        if (first_removed == _root->_head) {
          _root->_head = end_removed;
        }

        pointer_list_root<T> result{first_removed};
        _root = &result;
        return result;
      }
    }
    return {nullptr};
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

//  template <class T>
//  pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() {
//    if (_node) {
//      _node = _node->_prev;
//      if (_node == _root->_head) {
//        _node = nullptr;
//      }
//    }
//    return *this;
//  }

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

//  template <class T>
//  const pointer_list_iterator<T>& pointer_list_iterator<T>::operator--() const {
//    if (_node) {
//      _node = _node->_prev;
//      if (_node == _root->_head) {
//        _node = nullptr;
//      }
//    }
//    return *this;
//  }

  template <class T>
  const pointer_list_iterator<T> pointer_list_iterator<T>::operator++(int) const {
    auto* const node = _node;
    operator++();
    return {_root, node};
  }

//  template <class T>
//  const pointer_list_iterator<T> pointer_list_iterator<T>::operator--(int) const {
//    auto* const node = _node;
//    operator--();
//    return {_root, node};
//  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_ITERATOR_IMPL_HH
