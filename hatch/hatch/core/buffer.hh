#ifndef HATCH_BUFFER_HH
#define HATCH_BUFFER_HH

namespace hatch {

  template <size_t S>
  class block final {
  private:
    std::aligned_storage<S, alignof(size_t)> _storage{0};
    uint8_t* _read{0};
    uint8_t* _write{&_storage};

  public:
    block() = default;
    ~block() = default;

    const uint8_t* read_ptr() const {
      return _end ? &storage + _begin : nullptr;
    }

    size_t read_size() const {
      return _end ? _end - _begin : 0;
    }

    uint8_t* write_ptr() const {
      return
    }

    void fill(size_t size) {
      _end += size;
      if (_end > S + 1) {
        throw std::runtime_error("tried to fill block beyond capacity");
      }
    }

    void drain(size_t size) {
      _begin += size;
      if (_begin == _end) {
        _begin = 0;
        _end = 0;
      }
      if (_begin + 1 > _end) {
        throw std::runtime_error("tried to drain block beyond content")
      }
    }
  };



  template <size_t N, size_t S>
  class ring {
  public:


  private:
    std::aligned_storage<N * S, max_align_t> _slab;
    std::array<block, N> _blocks;
  };

}

#endif
