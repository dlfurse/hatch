#ifndef HATCH_POINTER_IMPL_HH
#define HATCH_POINTER_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer_impl.hh directly. include memory.hh instead."
#endif

#include <cassert>

namespace hatch {

  template <class T>
  pointer<T>::pointer(allocator<T>* allocator, uint64_t index) :
      _allocator{allocator},
      _index{index} {
    _allocator->_data[index].created = this;
  }


  template <class T>
  pointer<T>::pointer() :
      _allocator{nullptr},
      _index{0} {
  }

  template <class T>
  pointer<T>::~pointer() {
    detach();
  }

  template <class T>
  pointer<T>::pointer(const pointer& ptr) {
    after(ptr);
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(const pointer& ptr) {
    if (this != &ptr) {
      detach();
      after(ptr);
    }
    return *this;
  }

  template <class T>
  pointer<T>::pointer(pointer&& ptr) noexcept {
    replace(ptr);
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(pointer<T> &&ptr) noexcept {
    if (this != &ptr) {
      detach();
      replace(ptr);
    }
    return *this;
  }

  template <class T>
  T* pointer<T>::operator->() {
    assert(_allocator);
    return reinterpret_cast<T*>(&_allocator->_data[_index].data);
  }

  template <class T>
  bool pointer<T>::null() {
    return _allocator == nullptr;
  }

  template <class T>
  void pointer<T>::detach() {
    if (_allocator) {
      if (_prev) {
        _prev->_next = _next;
        if (_next) {
          _next->_prev = _prev;
        }
      } else {
        if (_next) {
          _allocator->_data[_index].created = _next;
          _next->_prev = _prev;
        } else {
          _allocator->destroy(*this);
        }
      }
    }
    _allocator = nullptr;
  }

  template <class T>
  void pointer<T>::after(const pointer& ptr) {
    _allocator = ptr._allocator;
    if (_allocator) {
      _index = ptr._index;
      _next = ptr._next;
      if (_next) {
        _next->_prev = this;
      }
      _prev = &ptr;
      ptr._next = this;
    }
  }

  template <class T>
  void pointer<T>::replace(pointer& ptr) {
    _allocator = ptr._allocator;
    if (_allocator) {
      _index = ptr._index;
      _next = ptr._next;
      if (_next) {
        _next->_prev = this;
      }
      _prev = ptr._prev;
      if (_prev) {
        _prev->_next = this;
      } else {
        _allocator->_data[_index].created = this;
      }
    }
    ptr._allocator = nullptr;
  }

} // end namespace hatch

#endif // HATCH_POINTER_IMPL_HH
