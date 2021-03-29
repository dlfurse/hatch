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
  T* pointer_list_root<T>::front() const {
    if (_head) {
      return static_cast<T*>(_head);
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::back() const {
    if (_head) {
      if (_head->_prev) {
        return static_cast<T*>(_head->_prev);
      } else {
        return static_cast<T*>(_head);
      }
    }
    return nullptr;
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
  T* pointer_list_root<T>::pop_front() {
    if (auto* node = _head) {
      _head = node->_next;
      node->detach();
      return static_cast<T*>(node);
    }
    return nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::replace_front(pointer_list_node<T>& node) {
    auto* replaced = front();
    if (!empty()) {
      node.insert_replacing(*_head);
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
  void pointer_list_root<T>::push_front(pointer_list_node<T>& node) {
    if (!empty()) {
      node.insert_before(*_head);
    } else {
      node.detach();
    }
    _head = &node;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_iterator<T>& iterator) {
    if (iterator._root) {
      if (auto *node = iterator._head) {
        ++iterator;
        push_front(*node);
      }
    }
  }

  template <class T>
  void pointer_list_root<T>::splice_front(pointer_list_node<T>& node) {
    if (!empty()) {
      node.splice_before(*_head);
    } else {
      node.detach();
    }
    _head = &node;
  }

  template <class T>
  void pointer_list_root<T>::splice_front(pointer_list_root<T>& root) {
    if (auto* node = root._head) {
      root._head = nullptr;
      splice_front(*node);
    }
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
