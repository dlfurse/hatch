#ifndef HATCH_LIST_ROOT_IMPL_HH
#define HATCH_LIST_ROOT_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T>
  list<T>::list(list_node<T>* head) :
      _head{head} {
  }

  template <class T>
  list<T>::list() :
      _head{nullptr} {
  }

  template <class T>
  list<T>::~list() {
    while (pop_front());
  }

  template <class T>
  list<T>::list(list&& moved) noexcept :
      keeper<list<T>, list_iterator<T>>{moved},
      _head{moved._head} {
    moved._head = nullptr;
  }

  template <class T>
  list<T>& list<T>::operator=(list&& moved) noexcept {
    keeper<list<T>, list_iterator<T>>::operator=(moved);
    _head = moved._head;
    moved._head = nullptr;
    return *this;
  }

  ////////////////
  // Iterators. //
  ////////////////

  template <class T>
  list_iterator<T> list<T>::begin() {
    return {this, _head ? _head : list_iterator<T>::_after};
  }

  template <class T>
  const list_iterator<T> list<T>::begin() const {
    return {this, _head ? _head : list_iterator<T>::_after};
  }

  template <class T>
  list_iterator<T> list<T>::end() {
    return {this, list_iterator<T>::_after};
  }

  template <class T>
  const list_iterator<T> list<T>::end() const {
    return {this, list_iterator<T>::_after};
  }

  /////////////////////
  // Access content. //
  /////////////////////

  template <class T>
  bool list<T>::empty() const {
    return _head == nullptr;
  }

  template <class T>
  T* list<T>::front() const {
    return _head ? &_head->get() : nullptr;
  }

  template <class T>
  T* list<T>::back() const {
    return _head ? &_head->prev().get() : nullptr;
  }

  /////////////////////
  // Mutate content. //
  /////////////////////

  template <class T>
  T* list<T>::pop_front() {
    this->release();
    auto* popped = front();
    if (popped) {
      if (_head->alone()) {
        _head = nullptr;
      } else {
        auto* next = &_head->next();
        _head->splice(*next);
        _head = next;
      }
    }
    return popped;
  }

  template <class T>
  void list<T>::push_front(list_node<T>& node) {
    this->release();
    auto* pushed = &node;
    pushed->splice(pushed->next());
    if (_head) {
      pushed->splice(*_head);
    }
    _head = pushed;
  }

  template <class T>
  void list<T>::push_front(list<T>& list) {
    this->release();
    auto* pushed = list._head;
    if (pushed) {
      if (_head) {
        pushed->splice(*_head);
      }
      _head = pushed;
    }
    list.release();
    list._head = nullptr;
  }

  template <class T>
  T* list<T>::pop_back() {
    this->release();
    auto* popped = back();
    if (popped) {
      if (_head->prev().alone()) {
        _head = nullptr;
      } else {
        auto* prev = &_head->prev();
        prev->splice(*_head);
      }
    }
    return popped;
  }

  template <class T>
  void list<T>::push_back(list_node<T>& node) {
    this->release();
    auto* pushed = &node;
    pushed->splice(pushed->next());
    if (_head) {
      pushed->splice(*_head);
    } else {
      _head = pushed;
    }
  }

  template <class T>
  void list<T>::push_back(list<T>& list) {
    this->release();
    auto* pushed = list._head;
    if (pushed) {
      if (_head) {
        pushed->splice(*_head);
      } else {
        _head = pushed;
      }
    }
    list.release();
    list._head = nullptr;
  }

} // namespace hatch

#endif // HATCH_LIST_ROOT_IMPL_HH
