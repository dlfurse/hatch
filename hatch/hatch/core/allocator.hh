#ifndef HATCH_ALLOCATOR_HH
#define HATCH_ALLOCATOR_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/list.hh>
#include <hatch/utility/tree.hh>

#include <memory> // std::aligned_storage

#include <cstddef> // std::max_align_t, size_t
#include <cstdint> // uint64_t

namespace hatch {

  class allocator {
  public:
    static constexpr auto pagesize = 16384lu;

  public:
    allocator();
    ~allocator();

  public:
    template <class T, class ...Args>
    pointer<T> create(Args&&... args);

    template <class T>
    void destroy(pointer<T>& ptr);

  private:
    template <uint16_t ...Sizes>
    class allocations;

    template <uint64_t S>
    class allocation {
    public:
      union node {
      public:
        allocated<S> _allocated;
        liberated<S> _liberated;
      };

      tree<liberated<S>> _free;
      liberated<S>* _next;
    };

    //using bucketlist = buckets<4, 8, 12, 16, 24, 32, 48, 64, 96, 128, 192, 256, 384, 512, 768, 1024, 1536, 2048, 3072, 4096, 6144, 8192, 12288, 16384>;


  };

}

#endif // HATCH_ALLOCATOR_HH
