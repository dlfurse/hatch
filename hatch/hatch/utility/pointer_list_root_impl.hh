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
  pointer_list_root<T>::pointer_list_root(pointer_list_root&& moved) :
      _head{moved._head} {
    moved._head = nullptr;
  }

  template <class T>
  pointer_list_root<T>& pointer_list_root<T>::operator=(pointer_list_root&& moved) {
    _head = moved._head;
    moved._head = nullptr;
  }

  template <class T>
  bool pointer_list_root<T>::empty() const {
    return _head == nullptr;
  }


  template <class T>
  T* pointer_list_root<T>::front() {
    return _head ? static_cast<T*>(_head) : nullptr;
  }

  template <class T>
  const T* pointer_list_root<T>::front() const {
    return _head ? static_cast<const T*>(_head) : nullptr;
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::begin() {
    return {this, _head};
  }


  template <class T>
  T* pointer_list_root<T>::pop_front() {
    auto* popped = front();
    if (popped) {
      if (popped->detached()) {
        _head = nullptr;
      } else {
        _head = popped->next();
        popped->detach();
      }
    }
    return popped;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_node<T>& node) {
    auto* pushed = &node.detach();
    if (_head) {
      pushed->splice_before(*_head);
    }
    _head = pushed;
  }

  template <class T>
  void pointer_list_root<T>::push_front(pointer_list_root<T>& root) {
    auto* pushed = root._head;
    if (pushed) {
      if (_head) {
        pushed->splice_before(*_head);
      }
      _head = pushed;
    }
    root._head = nullptr;
  }


  template <class T>
  T* pointer_list_root<T>::back() {
    return _head ? static_cast<T*>(_head->_prev) : nullptr;
  }

  template <class T>
  const T* pointer_list_root<T>::back() const {
    return _head ? static_cast<const T*>(_head->_prev) : nullptr;
  }

  template <class T>
  pointer_list_iterator<T> pointer_list_root<T>::end() {
    return {this, nullptr};
  }


  template <class T>
  T* pointer_list_root<T>::pop_back() {
    auto* popped = back();
    if (popped) {
      if (popped->detached()) {
        _head = nullptr;
      } else {
        popped->detach();
      }
    }
    return popped;
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_node<T>& node) {
    auto* pushed = &node.detach();
    if (_head) {
      pushed->splice_before(*_head);
    } else {
      _head = pushed;
    }
  }

  template <class T>
  void pointer_list_root<T>::push_back(pointer_list_root<T>& root) {
    auto* pushed = root._head;
    if (pushed) {
      if (_head) {
        pushed->splice_before(*_head);
      } else {
        _head = pushed;
      }
    }
    root._head = nullptr;
  }

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_IMPL_HH
