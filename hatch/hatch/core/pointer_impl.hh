#ifndef HATCH_POINTER_IMPL_HH
#define HATCH_POINTER_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer_impl.hh directly. include memory.hh instead."
#endif

#include <cassert>

namespace hatch {

  template <class T>
  pointer<T>::pointer(allocated<slab<T>>* owner, allocator* allocator) :
      handle<slab<T>>::handle{owner},
      _allocator{allocator} {
  }

  template <class T>
  pointer<T>::pointer() :
      _allocator{nullptr} {
  }

  template <class T>
  pointer<T>::~pointer() {
    if (_allocator && owned<allocated<slab<T>>, pointer<T>>::alone()) {
      _allocator->destroy(*this);
    }
  }

  template <class T>
  pointer<T>::pointer(pointer&& ptr) noexcept :
      handle<slab<T>>::owned{std::move(ptr)},
      _allocator{ptr._allocator} {
    ptr._allocator = nullptr;
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(pointer<T> &&ptr) noexcept {
    handle<slab<T>>::operator=(std::move(ptr));
    ptr._allocator = nullptr;
    return *this;
  }

  template <class T>
  pointer<T>::pointer(const pointer& ptr) :
      handle<slab<T>>{ptr},
  _allocator{ptr._allocator} {
  }

  template <class T>
  pointer<T>& pointer<T>::operator=(const pointer& ptr) {
    handle<slab<T>>::operator=(ptr);
    return *this;
  }

  template <class T>
  pointer<T>::operator bool() {
    return _allocator;
  }

  template <class T>
  T* pointer<T>::operator->() {
    return reinterpret_cast<T*>(&this->_owner->_data);
  }

  template <class T>
  T& pointer<T>::operator*() {
    return reinterpret_cast<T&>(this->_owner->_data);
  }

} // end namespace hatch

#endif // HATCH_POINTER_IMPL_HH
