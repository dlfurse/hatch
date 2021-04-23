#ifndef HATCH_POINTER_IMPL_HH
#define HATCH_POINTER_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer_impl.hh directly. include memory.hh instead."
#endif

#include <cassert>

namespace hatch {

  template <class T>
  pointer<T>::pointer() :
      _allocator{nullptr},
      _index{0} {
  }

  template <class T>
  pointer<T>::~pointer() {
    _allocator->detach_pointer(*this);
  }

  template <class T>
  pointer<T>::pointer(const pointer& ptr) :
    _allocator{ptr._allocator},
    _index{ptr._index} {
    _allocator->attach_pointer(*this);
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(const pointer& ptr) {
    assert(_allocator == ptr._allocator);
    if (this != &ptr) {
      _allocator->detach_pointer(*this);
      _index = ptr._index;
      _allocator->attach_pointer(*this);
    }
    return *this;
  }

  template <class T>
  pointer<T>::pointer(pointer&& ptr) noexcept :
    _allocator{ptr._allocator},
    _index{ptr._index} {
    _allocator->attach_pointer(this);
    _allocator->detach_pointer(ptr);
    ptr._allocator = nullptr;
    ptr._index = 0;
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(pointer<T> &&ptr) noexcept {
    assert(_allocator == ptr._allocator);
    if (this != &ptr) {
      _allocator->detach_pointer(*this);
      _index = ptr._index;
      _allocator->attach_pointer(*this);
      _allocator->detach_pointer(ptr);
      ptr._allocator = nullptr;
      ptr._index = 0;
    }
    return *this;
  }

  template <class T>
  pointer<T>::pointer(allocator<T>* allocator, uint64_t index) :
      _allocator{allocator},
      _index{index} {
    _allocator->attach_pointer(*this);
  }

  template <class T>
  T* pointer<T>::operator->() {
    assert(_allocator);
    return reinterpret_cast<T*>(&_allocator->_data[_index].data);
  }

  template <class T>
  pointer<T>::operator bool() {
    return _allocator;
  }

} // end namespace hatch

#endif // HATCH_POINTER_IMPL_HH
