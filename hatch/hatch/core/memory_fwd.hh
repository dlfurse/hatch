#ifndef HATCH_MEMORY_FWD_HH
#define HATCH_MEMORY_FWD_HH

#include <cstdint>

namespace hatch {

  template <class T>
  class pointer;

  class allocator;

  template <uint64_t S>
  class allocated;

  template <uint64_t S>
  class liberated;

}

#endif // HATCH_ASYNC_FWD_HH
