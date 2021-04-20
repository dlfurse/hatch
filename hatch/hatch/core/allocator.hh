#ifndef HATCH_ALLOCATOR_HH
#define HATCH_ALLOCATOR_HH

#ifndef HATCH_MEMORY_HH
#error "do not include pointer.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/pointer_list.hh>

#include <memory> // std::aligned_storage

#include <cstddef> // std::max_align_t, size_t
#include <cstdint> // uint64_t

namespace hatch {

  template <class T>
  class allocator {
  public:
    friend class pointer<T>;

  public:
    allocator(uint64_t min_capacity = 1024, uint8_t max_doubling = 8);
    ~allocator();

  public:
    template <class ...Args>
    pointer<T> create(Args&&... args);

    void destroy(pointer<T>& destroyed);

  public:
    uint64_t allocated() const;
    uint64_t capacity() const;
    uint64_t growth_increment() const;
    uint64_t shrink_threshold() const;

  private:
    class node {
    public:
      class tree {
        uint64_t color;
        int64_t parent;
        int64_t left;
        int64_t right;
      };

      class list {
        pointer<T>* head;
        pointer<T>* tail;
      };

    public:
      union {
        tree free;
        pointer_list_root<pointer<T>> used;
      } header;
      typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
    };

    node* _data;
    uint64_t _free;

    uint64_t _allocated;
    uint64_t _capacity;
    uint64_t _growth_increment;
    uint64_t _shrink_threshold;

    const uint64_t _min_capacity;
    const uint64_t _lin_boundary;

    static constexpr uint64_t align = alignof(std::max_align_t);
    static constexpr uint64_t size = sizeof(node);
  };
}

#endif // HATCH_ALLOCATOR_HH
