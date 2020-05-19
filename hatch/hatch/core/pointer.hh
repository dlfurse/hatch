#ifndef HATCH_POINTER_HH
#define HATCH_POINTER_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <cstdint> // uint64_t

namespace hatch {

  template <class T>
  class pointer {
  public:
    friend class allocator<T>;

  private:
    pointer(allocator<T>* allocator, uint64_t index);

  public:
    pointer(const pointer& ptr);
    pointer& operator=(const pointer& ptr);

    pointer(pointer&& ptr) noexcept;
    pointer& operator=(pointer&& ptr);

    ~pointer();

  public:
    T* operator->();
    bool null();

  private:
    void detach();
    void after(const pointer& ptr);
    void replace(pointer& ptr);

    mutable const pointer<T>* _next{nullptr};
    mutable const pointer<T>* _prev{nullptr};
    mutable allocator<T>* _allocator;
    mutable uint64_t _index;
  };

} // end namespace hatch

#endif // HATCH_POINTER_HH
