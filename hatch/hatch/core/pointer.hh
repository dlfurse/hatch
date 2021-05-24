#ifndef HATCH_POINTER_HH
#define HATCH_POINTER_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <hatch/core/handle.hh>
#include <hatch/core/slabs.hh>

namespace hatch {

  template <class T>
  class pointer : public handle<slab<T>> {
  public:
    friend class allocator;

  private:
    pointer(allocated<slab<T>>* allocated, allocator* allocator);

  public:
    pointer();
    ~pointer();

    pointer(const pointer& ptr);
    pointer& operator=(const pointer& ptr);

    pointer(pointer&& ptr) noexcept;
    pointer& operator=(pointer&& ptr) noexcept;

  public:
    T* operator->();
    T& operator*();

    explicit operator bool();

  private:
    mutable allocator* _allocator{nullptr};
  };

} // end namespace hatch

#endif // HATCH_POINTER_HH
