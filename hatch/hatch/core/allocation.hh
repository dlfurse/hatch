#ifndef HATCH_ALLOCATION_HH
#define HATCH_ALLOCATION_HH

#ifndef HATCH_MEMORY_HH
#error "do not include allocation.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/owning.hh>
#include <hatch/utility/list.hh>
#include <hatch/utility/tree.hh>

#include <memory> // std::aligned_storage

#include <cstddef> // std::max_align_t, size_t
#include <cstdint> // uint64_t

namespace hatch {

  template <uint64_t S>
  class allocation {
  public:
    friend class allocator;

  protected:
    template <class T, class ...Args>
    pointer<T> create(Args&&... args);

    destroy()

  };

}

#endif // HATCH_ALLOCATION_HH
