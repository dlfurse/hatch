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

  class allocator;

  template <class T>
  class allocation;

  template <class T>
  class pointer : public owned<allocated<T>, pointer<T>> {
  public:
    ~pointer();

    pointer& operator=(nullptr_t) {

      detach();
    }

    T* operator->() const {

    }

    T& operator*() const {

    }

  private;
    allocation<T>* _allocation;
  };

  template <class T>
  class allocated : public owner<allocated<T>, pointer<T>> {
  public:
    template <class ...Args>
    allocated(Args&&... args) {
      new (&_data) T{std::forward<Args>(args)...};
    }

  private:
    std::aligned_storage_t<sizeof(T), alignof(T)> _data;
  };

  template <class T>
  class freed : public tree_node<freed<T>> {
  public:
    explicit freed(uint64_t index) : _index{index} {
    }

    bool operator<(const freed& other) {
      return _index < other._index;
    }

    bool operator>(const freed& other) {
      return _index > other._index;
    }

  private:
    uint64_t _index;
  };

  template <class T>
  class allocation {
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
      std::aligned_storage_t<sizeof(T), alignof(T)> data;
    };

    static constexpr uint64_t align = alignof(std::max_align_t);
    static constexpr uint64_t size = sizeof(node);

  private:
    const uint64_t _min_capacity;
    const uint64_t _lin_boundary;

  public:
    explicit allocation(uint64_t min_capacity = 1024, uint8_t max_doubling = 8);
    ~allocation();

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

#endif // HATCH_ALLOCATION_HH
