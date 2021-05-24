#ifndef HATCH_ALLOCATED_IMPL_HH
#define HATCH_ALLOCATED_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include allocated_impl.hh directly. include memory.hh instead."
#endif

namespace hatch {

  template <uint64_t S>
  allocated<S>::allocated() :
      owner<allocated<S>, handle<S>>{} {
  }

  template <uint64_t S>
  allocated<S>::~allocated() {
  }

  template <uint64_t S>
  allocated<S>::allocated(allocated&& moved) noexcept :
      owner<allocated<S>, handle<S>>{std::move(moved)} {
  }

  template <uint64_t S>
  allocated<S>& allocated<S>::operator=(allocated&& moved) noexcept {
    owner<allocated<S>, handle<S>>::operator=(std::move(moved));
    return *this;
  }

  template <uint64_t S>
  template <class T, class ...Args>
  void allocated<S>::create(Args&&... args) {
    new (&_data) T{std::forward<Args>(args)...};
  }

  template <uint64_t S>
  template <class T>
  void allocated<S>::destroy() {
    reinterpret_cast<T&>(_data).~T();
  }

} // namespace hatch

#endif // HATCH_ALLOCATED_IMPL_HH