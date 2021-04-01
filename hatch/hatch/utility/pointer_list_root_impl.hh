#ifndef HATCH_POINTER_LIST_ROOT_IMPL_HH
#define HATCH_POINTER_LIST_ROOT_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_root_impl.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_root<T>::pointer_list_root() :
      _head{nullptr} {
  }

  template <class T>
  pointer_list_root<T>::~pointer_list_root() {
    while (pop_front());
  }

  template <class T>
  bool pointer_list_root<T>::empty() const {
    return _head == nullptr;
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::begin() {
    return {this, _head};
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::end() {
    return {this, nullptr};
  }

  template <class T>
  T* pointer_list_root<T>::get_front() const {
    if (_head) {
      return static_cast<T*>(_head);
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::pop_front() {
    auto* popped = _head;
    if (popped) {
      _head = popped->next();
      popped->detach();
    }
    return popped;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_node<T>& node) {
    auto* pusher = &node;
    if (auto* pushed = _head) {
      pusher->insert_before(*pushed);
    } else {
      pusher->detach();
    }
    _head = pusher;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_root<T>& root) {
    if (auto* pusher = root._head) {
      if (auto* pushed = _head) {
        pusher->splice_before(*pushed);
      }
      _head = pusher;
    }
    root._head = nullptr;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_iterator<T>& iterator) {
    if (auto* root = iterator._root) {
      if (auto* pusher = iterator._head) {
        if (auto* pushed = _head) {
          pusher->insert_before(*pushed);
        } else {
          pusher->detach();
        }
        _head = pusher;
      }
    }
    iterator._head = nullptr;
    iterator._root = nullptr;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after) {
    if (auto* root = start._root) {
      if (auto* first = start._head) {
        auto* last = root->_prev;
        if (after._head) {
          if (after._root == start._root) {
            while (last->_next != after._head && last->_next != back) {
              last = last->_next;
            }
          }
        }
        //TODO finish implementation
      }
    }
    start._head = nullptr;
    start._root = nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::replace_front(pointer_list_node<T>& node) {
    auto* replaced = get_front();
    if (replaced) {
      node.insert_replacing(*replaced);
    } else {
      node.detach();
    }
    _head = &node;
    return replaced;
  }

  template <class T>
  T* pointer_list_root<T>::replace_front(pointer_list_root<T>& root) {
    auto* replaced = get_front();
    if (replaced) {
      auto* head = root._head;
      if (head) {

      }
      node.insert_replacing(*replaced);
    } else {
      node.detach();
    }
    _head = &node;
    return replaced;
  }

  template <class T>
  T* pointer_list_root<T>::replace_front(pointer_list_iterator<T>& iterator) {
    if (iterator._root) {
      if (auto *node = iterator._head) {
        ++iterator;
        return replace_front(*node);
      }
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::replace_front(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after) {
    auto* replaced = get_front();
    if (replaced) {

    }
    return replaced;
  }



  template <class T>
  T* pointer_list_root<T>::get_back() const {
    if (_head) {
      return static_cast<T*>(_head->_prev);
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::pop_back() {
    if (auto* node = _head) {
      if (node->_prev) {
        node = node->_prev;
      } else {
        _head = nullptr;
      }
      node->detach();
      return static_cast<T*>(node);
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::replace_back(pointer_list_node<T>& node) {
    auto* replaced = back();
    if (_head) {
      if (_head->_prev) {
        node.insert_replacing(*_head->_prev);
      } else {
        node.insert_replacing(*_head);
        _head = &node;
      }
    } else {
      node.detach();
      _head = &node;
    }
    return replaced;
  }

  template <class T>
  T* pointer_list_root<T>::replace_back(pointer_list_iterator<T>& iterator) {
    if (auto* root = iterator._root) {
      if (root != this) {
        if (auto *node = iterator._head) {
          ++iterator;
          return replace_back(*node);
        }
      }
    }
    return nullptr;
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_node<T>& node) {
    if (_head) {
      if (_head->_prev) {
        node.insert_after(*_head->_prev);
      } else {
        node.insert_after(*_head);
      }
    } else {
      node.detach();
      _head = &node;
    }
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_iterator<T>& iterator) {
    if (auto* root = iterator._root) {
      if (root != this) {
        if (auto *node = iterator._head) {
          ++iterator;
          push_back(*node);
        }
      }
    }
  }

  template <class T>
  void pointer_list_root<T>::splice_back(pointer_list_node<T>& node) {
    if (!empty()) {
      if (_head->_prev) {
        node.splice_after(*_head->_prev);
      } else {
        node.splice_after(*_head);
      }
    } else {
      _head = &node;
    }
  }

  template <class T>
  void pointer_list_root<T>::splice_back(pointer_list_root<T>& root) {
    if (auto* node = root._head) {
      root._head = nullptr;
      splice_back(*node);
    }
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_IMPL_HH
