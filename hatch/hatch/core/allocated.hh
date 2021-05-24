#ifndef HATCH_ALLOCATED_HH
#define HATCH_ALLOCATED_HH

#ifndef HATCH_MEMORY_HH
#error "do not include allocated.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/owning.hh>
#include <hatch/core/handle.hh>

namespace hatch {

  template <uint64_t S>
  class allocated : public owner<allocated<S>, handle<S>> {
  public:
    friend class allocator;

  protected:
    allocated();
    ~allocated();

    allocated(allocated&& moved) noexcept;
    allocated& operator=(allocated&& moved) noexcept;

  public:
    allocated(const allocated& copied) = delete;
    allocated& operator=(const allocated& copied) = delete;

  protected:
    template <class T, class ...Args>
    void create(Args&&... args);

    template <class T>
    void destroy();

  protected:
    std::aligned_storage_t<S> _data;
  };

} // namespace hatch

#endif // HATCH_ALLOCATED_HH