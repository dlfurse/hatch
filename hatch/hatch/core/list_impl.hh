#ifndef HATCH_LIST_IMPL_HH
#define HATCH_LIST_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_impl.hh directly. include list.hh instead."
#endif

#include <iostream>

namespace hatch {

  template <class T>
  list_base<T>::list_base() :
      _prev{nullptr},
      _next{nullptr} {
  }

  template <class T>
  bool list_base<T>::operator!=(const list_base& compared) const {
    std::cout << "checking if " << this << " is the same as " << &compared << std::endl;
    return this != &compared;
  }

  template <class T>
  list_base<T>& list_base<T>::operator++() {
    std::cout << "incrementing " << this << " will yield " << _next << std::endl;
    return *_next;
  }

  template <class T>
  const list_base<T>& list_base<T>::operator++() const {
    return *_next;
  }

  template <class T>
  list_base<T>& list_base<T>::operator--() {
    return *_prev;
  }

  template <class T>
  const list_base<T>& list_base<T>::operator--() const {
    return *_prev;
  }

  template <class T>
  T& list_base<T>::operator*() {
    auto* val = static_cast<T*>(this);
    std::cout << "static casting " << this << " to a derived type yielded " << val << std::endl;
    return static_cast<T&>(*this);
  }

  template <class T>
  const T& list_base<T>::operator*() const {
    return static_cast<const T&>(*this);
  }


  template <class T>
  list_node<T>::~list_node() {
    detach();
  }

  template <class T>
  void list_node<T>::detach() {
    if (this->_prev) {
      this->_prev->_next = this->_next;
    }

    if (this->_next) {
      this->_next->_prev = this->_prev;
    }

    this->_prev = nullptr;
    this->_next = nullptr;
  }

  template <class T>
  void list_node<T>::detach_and_exchange_with(list_node& node) {
    auto* prev = node._prev;
    auto* next = node._next;

    detach();
    node.detach();

    if (prev) {
      prev->_next = this;
    }
    this->_prev = prev;

    if (next) {
      next->_prev = this;
    }
    this->_next = next;
  }

  template <class T>
  void list_node<T>::detach_and_insert_after(list_node& prev) {
    auto* next = prev._next;

    detach();

    prev._next = this;
    this->_prev = &prev;

    if (next) {
      next->_prev = this;
    }
    this->_next = next;
  }

  template <class T>
  void list_node<T>::detach_and_insert_before(list_node& next) {
    auto* prev = next._prev;

    detach();

    next._prev = this;
    this->_next = &next;

    if (prev) {
      prev->_next = this;
    }
    this->_prev = prev;
  }

  template <class T>
  list<T>::list() {
    this->_prev = this;
    this->_next = this;
  }

  template <class T>
  list<T>::~list() {
    clear();
  }

  template <class T>
  list<T>::list(list&& moved) noexcept {
    if (moved.empty()) {
      this->_prev = this;
      this->_next = this;
    } else {
      this->_next = moved._next;
      this->_prev = moved._prev;
      this->_next->_prev = this;
      this->_prev->_next = this;
      moved._next = &moved;
      moved._prev = &moved;
    }
  }

  template <class T>
  list<T>& list<T>::operator=(list&& moved) noexcept {
    this->_next = moved._next;
    this->_prev = moved._prev;
    moved._next = nullptr;
    moved._prev = nullptr;
    return *this;
  }

  template <class T>
  bool list<T>::empty() const {
    return this->_prev == this && this->_next == this;
  }

  template <class T>
  void list<T>::clear() {
    while (pop_front());
  }

  template <class T>
  list_base<T>& list<T>::begin() {
    return *this->_next;
  }

  template <class T>
  const list_base<T>& list<T>::begin() const {
    return *this->_next;
  }

  template <class T>
  list_base<T>& list<T>::end() {
    return *this;
  }

  template <class T>
  const list_base<T>& list<T>::end() const {
    return *this;
  }

  template <class T>
  void list<T>::push_front(T& node) {
    if (empty()) {
      this->_next = &node;
      this->_prev = &node;
      node._next = this;
      node._prev = this;
    } else {
      auto* front = static_cast<T*>(this->_next);
      node.detach_and_insert_before(*front);
    }
  }

  template <class T>
  T* list<T>::pop_front() {
    if (empty()) {
      return nullptr;
    } else {
      auto* front = static_cast<T*>(this->_next);
      front->detach();
      return front;
    }
  }

  template <class T>
  void list<T>::push_back(T& node) {
    if (empty()) {
      this->_next = &node;
      this->_prev = &node;
      node._next = this;
      node._prev = this;
    } else {
      auto* back = static_cast<T*>(this->_prev);
      node.detach_and_insert_after(*back);
    }
  }

  template <class T>
  T* list<T>::pop_back() {
    if (empty()) {
      return nullptr;
    } else {
      auto* back = static_cast<T*>(this->_prev);
      back->detach();
      return back;
    }
  }

//  template <class T>
//  template <class F>
//  void list<T>::foreach_forward(F&& function) {
//    auto* next = this->_next;
//    while (next != this) {
//      auto* node = static_cast<T*>(next);
//      next = node->_next;
//      function(*node);
//    }
//  }
//
//  template <class T>
//  template <class F>
//  void list<T>::foreach_reverse(F&& function) {
//    auto* prev = this->_prev;
//    while (prev != this) {
//      auto* node = static_cast<T*>(prev);
//      prev = node->_prev;
//      function(*node);
//    }
//  }
}

#endif // HATCH_LIST_IMPL_HH
