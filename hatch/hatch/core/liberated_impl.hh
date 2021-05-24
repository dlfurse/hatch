#ifndef HATCH_LIBERATED_IMPL_HH
#define HATCH_LIBERATED_IMPL_HH

#ifndef HATCH_MEMORY_HH
#error "do not include liberated_impl.hh directly. include memory.hh instead."
#endif

namespace hatch {

  template <uint64_t S>
  liberated<S>::liberated() :
      tree_node<liberated<S>>{} {
  }

  template <uint64_t S>
  liberated<S>::~liberated() {
  }

  template <uint64_t S>
  liberated<S>::liberated(liberated&& moved) noexcept :
      tree_node<liberated<S>>{std::move(moved)} {
  }

  template <uint64_t S>
  liberated<S>& liberated<S>::operator=(liberated&& moved) noexcept {
    tree_node<liberated<S>>::operator=(std::move(moved));
    return *this;
  }

  template <uint64_t S>
  bool liberated<S>::operator<(const liberated& other) {
    return this < &other;
  }

  template <uint64_t S>
  bool liberated<S>::operator>(const liberated& other) {
    return this > &other;
  }


} // namespace hatch

#endif // HATCH_LIBERATED_IMPL_HH