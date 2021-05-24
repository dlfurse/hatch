#ifndef HATCH_SLABS_HH
#define HATCH_SLABS_HH

#ifndef HATCH_MEMORY_HH
#error "do not include slabs.hh directly. include memory.hh instead."
#endif

#include <cstdint> // uint64_t;

namespace hatch {

  template <uint64_t ...N>
  class slabs;

  template <uint64_t S, uint64_t ...N>
  class slabs<S, N...> {
  public:
    template <class T>
    static constexpr uint64_t slab = sizeof(T) < S ? S : slabs<N...>::template slab<T>;
  };

  template <uint64_t S>
  class slabs<S> {
  public:
    template <class T>
    static constexpr uint64_t slab = sizeof(T) < S ? S : sizeof(T);
  };

  template <class T>
  constexpr uint64_t slab = slabs<8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192>::template slab<T>;

} // namespace hatch

#endif // HATCH_SLABS_HH