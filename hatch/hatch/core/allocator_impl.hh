#ifndef HATCH_ALLOCATOR_IMPL_HH
#define HATCH_ALLOCATOR_IMPL_HH

#ifndef HATCH_ALLOCATOR_HH
#error "do not include allocator_impl.hh directly.  include memory.hh instead."
#endif

#include <iostream>
#include <memory> // std::aligned_alloc
#include <utility> // std::forward

#include <cassert> // assert
#include <cstring> // memcpy

namespace hatch {

  template <class T>
  allocator<T>::allocator(uint64_t min_capacity, uint8_t max_doublings) :
      _allocated{0},
      _capacity{min_capacity},
      _growth_increment{min_capacity},
      _shrink_threshold{min_capacity},
      _min_capacity{min_capacity},
      _lin_boundary{min_capacity * (2 << max_doublings)}
  {
    _data = static_cast<node *>(aligned_alloc(align, _capacity * size));
    assert(_data);

    _free = 0;
    for (uint64_t index = 0; index < _capacity; index++) {
      if (index == _capacity - 1) {
        _data[index] = {index - 1, 0, nullptr};
      } else if (index == 0) {
        _data[index] = {_capacity - 1, index + 1, nullptr};
      } else {
        _data[index] = {index - 1, index + 1, nullptr};
      }
    }
  }

  template <class T>
  uint64_t allocator<T>::allocated() const {
    return _allocated;
  }

  template <class T>
  uint64_t allocator<T>::capacity() const {
    return _capacity;
  }

  template <class T>
  uint64_t allocator<T>::growth_increment() const {
    return _growth_increment;
  }

  template <class T>
  uint64_t allocator<T>::shrink_threshold() const {
    return _shrink_threshold;
  }

  template <class T>
  allocator<T>::~allocator() {
    std::free(_data);
  }

  template <class T>
  template <class ...Args>
  pointer<T> allocator<T>::create(Args&&... args) {
    if (++_allocated == _capacity) {
      auto old_capacity = _capacity;
      auto old_data = _data;

      _capacity += _growth_increment;
      if (_capacity < _lin_boundary) {
        _growth_increment *= 2;
      }
      if (_allocated == 4 * _shrink_threshold) {
        _shrink_threshold *= 2;
      }

      _data = static_cast<node*>(aligned_alloc(align, _capacity * size));
      assert(_data);

      memcpy(_data, old_data, old_capacity * size);
      std::free(old_data);

      _data[_free].next = old_capacity;
      _data[_free].prev = _capacity-1;
      for (uint64_t index = old_capacity; index < _capacity; index++) {
        if (index == _capacity - 1) {
          _data[index] = {index - 1, _free, nullptr};
        } else if (index == old_capacity) {
          _data[index] = {_free, index + 1, nullptr};
        } else {
          _data[index] = {index - 1, index + 1, nullptr};
        }
      }
    }

    const auto created_index = _free;
    auto& created_node = _data[created_index];

    const auto prev_index = created_node.prev;
    auto& prev_node = _data[prev_index];

    const auto next_index = created_node.next;
    auto& next_node = _data[next_index];

    prev_node.next = next_index;
    next_node.prev = prev_index;

    _free = next_index;
    new (&created_node.data) T{std::forward<Args>(args)...};

    return {this, created_index};
  }

  template <class T>
  void allocator<T>::destroy(pointer<T>& destroyed) {
    if (--_allocated == _shrink_threshold) {
      if (_capacity > _min_capacity) {
        auto old_capacity = _capacity;
        auto old_data = _data;

        if (_capacity < _lin_boundary) {
          _growth_increment /= 2;
        }

        _capacity = _shrink_threshold * 2;
        if (_capacity < _min_capacity) {
          _capacity = _min_capacity;
        }

        _shrink_threshold = _shrink_threshold / 2;
        if (_shrink_threshold < _min_capacity) {
          _shrink_threshold = _min_capacity;
        }

        if (_capacity < old_capacity) {
          _data = static_cast<node *>(aligned_alloc(align, _capacity * size));
          assert(_data);

          uint64_t new_index = 0;
          uint64_t old_index = 0;
          while (old_index < old_capacity) {
            auto& old_node = old_data[old_index];
            if (auto* pointer = old_node.created) {
              auto& new_node = _data[new_index];
              memcpy(&new_node, &old_node, size);
              while (pointer) {
                pointer->_index = new_index;
                pointer = pointer->_next;
              }
              ++new_index;
            }
            ++old_index;
          }
          std::free(old_data);

          _free = new_index;
          for (uint64_t index = new_index; index < _capacity; index++) {
            if (index == _capacity - 1) {
              _data[index] = {index - 1, _free, nullptr};
            } else if (index == old_capacity) {
              _data[index] = {_free, index + 1, nullptr};
            } else {
              _data[index] = {index - 1, index + 1, nullptr};
            }
          }
        }
      }
    }

    auto destroyed_index = destroyed._index;
    auto& destroyed_node = _data[destroyed_index];

    auto next_index = _free;
    auto& next_node = _data[next_index];

    auto prev_index = next_node.prev;
    auto& prev_node = _data[prev_index];

    prev_node.next = destroyed_index;
    destroyed_node.prev = prev_index;
    next_node.prev = destroyed_index;
    destroyed_node.next = next_index;

    auto* pointer  = destroyed_node.created;
    while (pointer) {
      pointer->_allocator = nullptr;
      pointer = pointer->_next;
    }
    destroyed_node.created = nullptr;
  }

  template <class T>
  void allocator<T>::attach_pointer(pointer <T>& p) {
    auto& pointers = _data[p.index].pointers;
    pointers.push_back(p);
  }

  template <class T>
  void allocator<T>::detach_pointer(pointer <T>& p) {
    auto& pointers = _data[p.index].pointers;
    if (pointers.front() == &p) {
      pointers.pop_front();
    } else {
      p.splice(p.next());
    }
  }
}

#endif // HATCH_ALLOCATOR_IMPL_HH
