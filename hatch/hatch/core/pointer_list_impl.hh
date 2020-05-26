#ifndef HATCH_POINTER_LIST_IMPL_HH
#define HATCH_POINTER_LIST_IMPL_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_list_impl.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_list<T>::pointer_list() :
      _prev{nullptr},
      _next{nullptr} {
  }

  template <class T>
  pointer_list<T>::~pointer_list() {
    detach();
  }

  template <class T>
  pointer_list_iterator<T> pointer_list<T>::begin() {
    return {this, this};
  }

  template <class T>
  pointer_list_iterator<T> pointer_list<T>::end() {
    return {nullptr, this};
  }

  template <class T>
  bool pointer_list<T>::detached() const {
    return _prev == nullptr && _next == nullptr;
  }

  template <class T>
  bool pointer_list<T>::detach() {
    if (!detached()) {
      auto* prev = _prev;
      auto* next = _next;

      if (prev != next) {
        prev->_next = next;
        next->_prev = prev;
      } else {
        prev->_next = nullptr;
        next->_prev = nullptr;
      }

      _prev = nullptr;
      _next = nullptr;

      return true;
    }
    return false;
  }

  template <class T>
  void pointer_list<T>::replace_item(pointer_list& item) {
    detach();

    auto* prev = item._prev;
    auto* next = item._next;

    if (!item.detached()) {
      item.detach();

      this->_next = next;
      next->_prev = this;

      this->_prev = prev;
      prev->_next = this;
    }
  }

  template <class T>
  void pointer_list<T>::replace_list(pointer_list& list) {
    auto* prev = list._prev;
    auto* next = list._next;

    if (!list.detached()) {
      list.detach();

      _prev->_next = next;
      next->_prev = _prev;

      this->_prev = prev;
      prev->_next = this;
    }
  }

  template <class T>
  void pointer_list<T>::insert_before(pointer_list& item) {
    detach();

    auto* prev = item._prev;

    if (!item.detached()) {
      this->_prev = prev;
      prev->_next = this;

      this->_next = &item;
      item._prev = this;
    } else {
      this->_prev = &item;
      item._next = this;

      this->_next = &item;
      item._prev = this;
    }
  }

  template <class T>
  void pointer_list<T>::splice_before(pointer_list& list) {
    detach();

    auto* prev = list._prev;

    if (!list.detached()) {
      _prev->_next = &list;
      list._prev = _prev;

      this->_prev = prev;
      prev->_next = this;
    } else {
      _prev->_next = &list;
      list._prev = _prev;

      this->_prev = &list;
      list._next = this;
    }
  }

  template <class T>
  void pointer_list<T>::insert_after(pointer_list& item) {
    detach();

    auto* next = item._next;

    if (!item.detached()) {
      this->_prev = &item;
      item._next = this;

      this->_next = next;
      next->_prev = this;
    } else {
      this->_prev = &item;
      item._next = this;

      this->_next = &item;
      item._prev = this;
    }
  }

  template <class T>
  void pointer_list<T>::splice_after(pointer_list& list) {
    detach();

    auto* next = list._next;

    if (!list.detached()) {
      _prev->_next = next;
      next->_prev = _prev;

      this->_prev = &list;
      list._next = this;
    } else {
      _prev->_next = &list;
      list._prev = this;

      this->_prev = &list;
      list._next = this;
    }
  }
}

#endif // HATCH_POINTER_LIST_IMPL_HH
