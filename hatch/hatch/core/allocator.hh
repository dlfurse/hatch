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

  template <class T>
  class allocator {
  public:
    friend class pointer<T>;

  private:
    class freenode : public tree_node<freenode> {
    public:
      uint64_t index;

      freenode(uint64_t index) : index{index}{
      }

      bool operator==(const freenode& other) {
        return index == other.index;
      }

      bool operator!=(const freenode& other) {
        return index != other.index;
      }

      bool operator<(const freenode& other) {
        return index < other.index;
      }

      bool operator>(const freenode& other) {
        return index > other.index;
      }
    };

    class node {
    public:
      union {
        freenode free;
        list<pointer<T>> pointers;
      } header;
      typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
    };

    static constexpr uint64_t align = alignof(std::max_align_t);
    static constexpr uint64_t size = sizeof(node);

  private:
    const uint64_t _min_capacity;
    const uint64_t _lin_boundary;

  public:
    explicit allocator(uint64_t min_capacity = 1024, uint8_t max_doubling = 8);
    ~allocator();

  private:
    uint64_t _allocated;
    uint64_t _capacity;
    uint64_t _growth_increment;
    uint64_t _shrink_threshold;

  public:
    [[nodiscard]] uint64_t allocated() const;
    [[nodiscard]] uint64_t capacity() const;
    [[nodiscard]] uint64_t growth_increment() const;
    [[nodiscard]] uint64_t shrink_threshold() const;

  private:
    void attach_pointer(pointer<T>& p);
    void detach_pointer(pointer<T>& p);

    node* _data;

    tree<freenode> _free;
    uint64_t _next;

  public:
    template <class ...Args>
    pointer<T> create(Args&&... args);

    void destroy(pointer<T>& destroyed);
  };
}

#endif // HATCH_ALLOCATOR_HH
