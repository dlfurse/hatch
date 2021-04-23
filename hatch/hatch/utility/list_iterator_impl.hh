#ifndef HATCH_LIST_ITERATOR_IMPL_HH
#define HATCH_LIST_ITERATOR_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_iterator_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  template <class T>
  list_node<T>* list_iterator<T>::_before =
      const_cast<list_node<T>*>(reinterpret_cast<const list_node<T>*>("list_iterator::before"));

  template <class T>
  list_node<T>* list_iterator<T>::_after =
      const_cast<list_node<T>*>(reinterpret_cast<const list_node<T>*>("list_iterator::after"));

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T>
  list_iterator<T>::list_iterator(list<T>* list, list_node<T>* node) :
      _list{list},
      _node{node} {
  }

  template <class T>
  list_iterator<T>::list_iterator(const list_iterator& copied) :
      _list{copied._list},
      _node{copied._node} {
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator=(const list_iterator& copied) {
    _list = copied._list;
    _node = copied._node;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template <class T>
  bool list_iterator<T>::operator==(const list_iterator& compared) const {
    return _node == compared._node;
  }

  template <class T>
  bool list_iterator<T>::operator!=(const list_iterator& compared) const {
    return _node != compared._node;
  }

  /////////////////////////////////////
  // Structure: get underlying data. //
  /////////////////////////////////////

  template <class T>
  T& list_iterator<T>::operator*() const {
    return _node->get();
  }

  template <class T>
  T* list_iterator<T>::operator->() const {
    return &_node->get();
  }

  ///////////////////////////////
  // Structure: move iterator. //
  ///////////////////////////////

  template <class T>
  list_iterator<T>& list_iterator<T>::operator++() {
    if (_node == _before) {
      if (_list->_head == nullptr) {
        _node = _after;
      }
      _node = _list->_head;
    } else if (_node != _after) {
      _node = &_node->next();
      if (_node == _list->_head) {
        _node = _after;
      }
    }
    return *this;
  }

  template <class T>
  const list_iterator<T>& list_iterator<T>::operator++() const {
    return const_cast<list_iterator<T>*>(this)->operator++();
  }

  template <class T>
  const list_iterator<T> list_iterator<T>::operator++(int) const {
    auto* const node = _node;
    this->operator++();
    return {_list, node};
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator--() {
    if (_node == _after) {
      if (_list->_head == nullptr) {
        _node = _before;
      }
      _node = &_list->_head->prev();
    } else if (_node != _before) {
      if (_node == _list->_head) {
        _node = _before;
      } else {
        _node = &_node->prev();
      }
    }
    return *this;
  }

  template <class T>
  const list_iterator<T>& list_iterator<T>::operator--() const {
    return const_cast<list_iterator<T>*>(this)->operator--();
  }

  template <class T>
  const list_iterator<T> list_iterator<T>::operator--(int) const {
    auto* const node = _node;
    this->operator--();
    return {_list, node};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying list. //
  ////////////////////////////////////////

  template <class T>
  list_iterator<T> list_iterator<T>::insert(list<T>& list) {
    if (_list != &list) {

      auto* const first_inserted = list._head;
      auto* const end_inserted = _node;

      if (first_inserted) {
        if (end_inserted != _before) {
          if (end_inserted != _after) {
            end_inserted->splice(*first_inserted);
          } else {
            _list->_head->splice(*first_inserted);
          }

          if (end_inserted == _list->_head) {
            _list->_head = first_inserted;
          }

          list_iterator<T> result{_list, first_inserted};
          list._head = nullptr;
          return result;
        }
      }
    }
    return {*this};
  }

  template <class T>
  list<T> list_iterator<T>::remove(list_iterator<T>& other) {
    if (_list == other._list) {

      auto* const first_removed = _node;
      auto* const end_removed = other._node;

      if (first_removed != _before && first_removed != _after && first_removed != end_removed) {
        if (end_removed != _before) {
          if (end_removed != _after) {
            auto* node = end_removed;
            do {
              if (node == first_removed) {
                return {};
              }
              node = node->_next;
            } while (node != _list->_head);

            first_removed->splice(*end_removed);

            if (first_removed == _list->_head) {
              _list->_head = end_removed;
            }
          } else {
            first_removed->splice(*_list->_head);

            if (first_removed == _list->_head) {
              _list->_head = nullptr;
            }
          }

          list<T> result{first_removed};
          _list = &result;
          return result;
        }
      }
    }
    return {nullptr};
  }

} // namespace hatch

#endif // HATCH_LIST_ITERATOR_IMPL_HH
