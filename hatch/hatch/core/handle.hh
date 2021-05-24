#ifndef HATCH_HANDLE_HH
#define HATCH_HANDLE_HH

#ifndef HATCH_MEMORY_HH
#error "do not include handle.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/owning.hh>

namespace hatch {

  template <uint64_t S>
  class handle : public owned<allocated<S>, handle<S>> {
  public:
    friend class allocator;

  protected:
    handle();
    ~handle();

    handle(handle&& ptr) noexcept;
    handle& operator=(handle&& ptr) noexcept;

    handle(const handle& ptr);
    handle& operator=(const handle& ptr);
  };

} // end namespace hatch

#endif // HATCH_HANDLE_HH
