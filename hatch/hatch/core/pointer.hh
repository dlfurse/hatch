#ifndef HATCH_POINTER_HH
#define HATCH_POINTER_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/owning.hh>

#include <cstdint> // uint64_t

namespace hatch {

  template <class T>
  class pointer {
  public:
    friend class allocator<T>;

  public:
    pointer();
    ~pointer();

    pointer(const pointer& ptr);
    pointer& operator=(const pointer& ptr);

    pointer(pointer&& ptr) noexcept;
    pointer& operator=(pointer&& ptr) noexcept;

  private:
    pointer(allocator<T>* allocator, uint64_t index);

  public:
    T* operator->();
    explicit operator bool();

  private:
    mutable allocator<T>* _allocator{nullptr};
    mutable uint64_t _index{0};
  };

} // end namespace hatch

#endif // HATCH_POINTER_HH
