#ifndef HATCH_POINTER_LIST_ROOT_IMPL_HH
#define HATCH_POINTER_LIST_ROOT_IMPL_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_root_impl.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list_root<T>::pointer_list_root(pointer_list_node<T>* head) :
      _head{head} {
  }

  template <class T>
  pointer_list_root<T>::pointer_list_root() :
      _head{nullptr} {
  }

  template <class T>
  pointer_list_root<T>::~pointer_list_root() {
    while (pop_front());
  }

  template <class T>
  pointer_list_root<T>::pointer_list_root(pointer_list_root&& moved) noexcept :
      _head{moved._head} {
    moved._head = nullptr;
  }

  template <class T>
  pointer_list_root<T>& pointer_list_root<T>::operator=(pointer_list_root&& moved) noexcept {
    _head = moved._head;
    moved._head = nullptr;
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::begin() {
    return {this, _head};
  }

  template <class T>
  const pointer_list_iterator<T> pointer_list_root<T>::begin() const {
    return {this, _head};
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::end() {
    return {this, nullptr};
  }

  template <class T>
  const pointer_list_iterator<T> pointer_list_root<T>::end() const {
    return {this, nullptr};
  }

  template <class T>
  T* pointer_list_root<T>::front() const {
    return _head ? &_head->get() : nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::back() const {
    return _head ? &_head->_prev->get() : nullptr;
  }

  template <class T>
  bool pointer_list_root<T>::empty() const {
    return _head == nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::pop_front() {
    auto* popped = front();
    if (popped) {
      if (popped->alone()) {
        _head = nullptr;
      } else {
        _head = popped->_next;
        popped->splice(*_head);
      }
    }
    return popped;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_node<T>& node) {
    auto* pushed = &node;
    pushed->splice(pushed->next());
    if (_head) {
      pushed->splice(*_head);
    }
    _head = pushed;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_root<T>& root) {
    auto* pushed = root._head;
    if (pushed) {
      if (_head) {
        pushed->splice(*_head);
      }
      _head = pushed;
    }
    root._head = nullptr;
  }

  template <class T>
  T* pointer_list_root<T>::pop_back() {
    auto* popped = back();
    if (popped) {
      if (popped->alone()) {
        _head = nullptr;
      } else {
        popped->splice(*_head);
      }
    }
    return popped;
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_node<T>& node) {
    auto* pushed = &node;
    pushed->splice(pushed->next());
    if (_head) {
      pushed->splice(*_head);
    } else {
      _head = pushed;
    }
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_root<T>& root) {
    auto* pushed = root._head;
    if (pushed) {
      if (_head) {
        pushed->splice(*_head);
      } else {
        _head = pushed;
      }
    }
    root._head = nullptr;
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_IMPL_HH
