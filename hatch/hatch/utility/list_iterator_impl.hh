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
  list_iterator<T>::list_iterator(list_node<T>* node, list<T>* list) :
      kept<list_node<T>, list_iterator<T>>{node},
      _list{list} {
  }

  template <class T>
  list_iterator<T>::list_iterator() :
    _list{nullptr} {
  }

  template <class T>
  list_iterator<T>::~list_iterator() {
  }

  template <class T>
  list_iterator<T>::list_iterator(list_iterator&& moved) noexcept :
      kept<list_node<T>, list_iterator<T>>{moved},
      _list{moved._list} {
    moved._list = nullptr;
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator=(list_iterator&& moved) noexcept {
    kept<list_node<T>, list_iterator<T>>::operator=(moved);
    _list = moved._list;
    moved._node = nullptr;
    return *this;
  }

  template <class T>
  list_iterator<T>::list_iterator(const list_iterator& copied) :
      kept<list_node<T>, list_iterator<T>>{copied},
      _list{copied._list} {
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator=(const list_iterator& copied) {
    kept<list_node<T>, list_iterator<T>>::operator=(copied);
    _list = copied._list;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template <class T>
  bool list_iterator<T>::operator==(const list_iterator& compared) const {
    return this->_keeper == compared._keeper;
  }

  template <class T>
  bool list_iterator<T>::operator!=(const list_iterator& compared) const {
    return this->_keeper != compared._keeper;
  }

  /////////////////////////////////////
  // Structure: get underlying data. //
  /////////////////////////////////////

  template <class T>
  T& list_iterator<T>::operator*() const {
    return this->_keeper->get();
  }

  template <class T>
  T* list_iterator<T>::operator->() const {
    return &this->_keeper->get();
  }

  ///////////////////////////////
  // Structure: move iterator. //
  ///////////////////////////////

  template <class T>
  list_iterator<T>& list_iterator<T>::operator++() {
    if (this->_keeper == _before) {
      if (_list->_head == nullptr) {
        this->_keeper = _after;
      } else {
        this->attach(_list->_head);
      }
    } else if (this->_keeper != _after) {
      if (this->_keeper == _list->_head->_prev) {
        this->detach();
        this->_keeper = _after;
      } else {
        this->attach(this->_keeper->_next);
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
    auto* const node = this->_keeper;
    this->operator++();
    return {node, _list};
  }

  template <class T>
  list_iterator<T>& list_iterator<T>::operator--() {
    if (this->_keeper == _after) {
      if (_list->_head == nullptr) {
        this->_keeper = _before;
      } else {
        this->attach(_list->_head->_prev);
      }
    } else if (this->_keeper != _before) {
      if (this->_keeper == _list->_head) {
        this->detach();
        this->_keeper = _before;
      } else {
        this->attach(this->_keeper->_prev);
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
    auto* const node = this->_keeper;
    this->operator--();
    return {node, _list};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying list. //
  ////////////////////////////////////////

  template <class T>
  list_iterator<T> list_iterator<T>::insert(list<T>& list) {
    if (_list != &list) {

      auto* const first_inserted = list._head;
      auto* const end_inserted = this->_keeper;

      if (first_inserted) {
        if (end_inserted != _before) {
          if (end_inserted != _after) {
            end_inserted->splice(*first_inserted);
          } else {
            _list->_head->splice(*first_inserted);
          }

          if (end_inserted == this->_keeper->_head) {
            _list->_head = first_inserted;
          }

          auto* node = list._head;
          do {
            auto* iter = node->_kept;
            if (iter) {
              do {
                iter->_list = _list;
                iter = iter->_next;
              } while (iter != node->_kept);
            }
            node = node->_next;
          } while (node != list._head);

          list._head = nullptr;

          return {_list, first_inserted};
        }
      }
    }
    return {*this};
  }

  template <class T>
  list<T> list_iterator<T>::remove(list_iterator<T>& other) {
    if (_list == other._keeper) {

      auto* const first_removed = this->_keeper;
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

          return {first_removed};
        }
      }
    }
    return {nullptr};
  }

} // namespace hatch

#endif // HATCH_LIST_ITERATOR_IMPL_HH
