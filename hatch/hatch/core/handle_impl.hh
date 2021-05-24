#ifndef HATCH_HANDLE_IMPL_HH
#define HATCH_HANDLE_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include handle_impl.hh directly. include memory.hh instead."
#endif

#include <cassert>

namespace hatch {

  template <uint64_t S>
  handle<S>::handle() :
      owned<allocated<S>, handle<S>>::owned{} {
  }

  template <uint64_t S>
  handle<S>::~handle() {
  }

  template <uint64_t S>
  handle<S>::handle(handle&& moved) noexcept :
      owned<allocated<S>, handle<S>>::owned{std::move(moved)} {
  }

  template <uint64_t S>
  handle<S>& handle<S>::operator=(handle&& moved) noexcept {
    owned<allocated<S>, handle<S>>::operator=(std::move(moved));
    return *this;
  }


} // namespace hatch

#endif // HATCH_HANDLE_IMPL_HH