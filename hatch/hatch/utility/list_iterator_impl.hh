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
  list_iterator<T>::list_iterator(list<T>* owner, list_node<T>* node) :
      owned<list < T>, list_iterator<T>>{owner},
      _node{node} {
  }

  template <class T>
  list_iterator<T>::list_iterator() :
    _node{nullptr} {
  }

  template <class T>
  list_iterator<T>::~list_iterator() {
  }

  template <class T>
  list_iterator<T>::list_iterator(list_iterator&& moved) noexcept :
      owned<list < T>, list_iterator<T>>{moved},
      _node{moved._node} {
    moved._node = nullptr;
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator=(list_iterator&& moved) noexcept {
    owned < list < T >, list_iterator < T >> ::operator=(moved);
    _node = moved._node;
    moved._node = nullptr;
    return *this;
  }

  template <class T>
  list_iterator<T>::list_iterator(const list_iterator& copied) :
      owned<list < T>, list_iterator<T>>{copied},
      _node{copied._node} {
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator=(const list_iterator& copied) {
    owned < list < T >, list_iterator < T >> ::operator=(copied);
    _node = copied._node;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template <class T>
  list_iterator<T>::operator bool() const {
    return this->_owner;
  }

  template <class T>
  bool list_iterator<T>::operator==(const list_iterator& compared) const {
    return this->_owner == compared._owner && this->_node == compared._node;
  }

  template <class T>
  bool list_iterator<T>::operator!=(const list_iterator& compared) const {
    return this->_owner != compared._owner || this->_node != compared._node;
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
    if (auto* list = this->_owner) {
      if (_node == _before) {
        if (list->_head == nullptr) {
          _node = _after;
        } else {
          _node = list->_head;
        }
      } else if (_node != _after) {
        if (_node == &list->_head->prev()) {
          _node = _after;
        } else {
          _node = &_node->next();
        }
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
    auto* const list = this->_owner;
    this->operator++();
    return list_iterator<T>{list, _node};
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator--() {
    if (auto* list = this->_owner) {
      if (_node == _after) {
        if (list->_head == nullptr) {
          _node = _before;
        } else {
          _node = &list->_head->prev();
        }
      } else if (_node != _before) {
        if (_node == list->_head) {
          _node = _before;
        } else {
          _node = &_node->prev();
        }
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
    auto* const list = this->_owner;
    this->operator--();
    return {list, _node};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying list. //
  ////////////////////////////////////////

  template <class T>
  list_iterator<T> list_iterator<T>::insert(list<T>& other) {
    if (auto* list = this->_owner) {
      if (list != &other) {
        auto* const first_inserted = other._head;
        auto* const end_inserted = _node;

        if (first_inserted) {
          if (end_inserted != _before) {
            if (end_inserted != _after) {
              end_inserted->splice(*first_inserted);
            } else {
              list->_head->splice(*first_inserted);
            }

            if (end_inserted == list->_head) {
              list->_head = first_inserted;
            }

            other._head = nullptr;
            other.disown_all();

            list->disown_all();
            return list_iterator<T>{list, first_inserted};
          }
        }
      }
    }
    return list_iterator<T>{*this};
  }

  template <class T>
  list<T> list_iterator<T>::remove(list_iterator<T>& other) {
    if (auto* owner = this->_owner) {
      if (owner == other._owner) {
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
              } while (node != owner->_head);

              first_removed->splice(*end_removed);

              if (first_removed == owner->_head) {
                owner->_head = end_removed;
              }
            } else {
              first_removed->splice(*owner->_head);

              if (first_removed == owner->_head) {
                owner->_head = nullptr;
              }
            }

            owner->disown_all();
            return list<T>{first_removed};
          }
        }
      }
    }
    return list<T>{nullptr};
  }

} // namespace hatch

#endif // HATCH_LIST_ITERATOR_IMPL_HH
