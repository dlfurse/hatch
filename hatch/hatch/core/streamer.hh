#ifndef HATCH_BUFFER_HH
#define HATCH_BUFFER_HH

#include <hatch/core/memory.hh>

#include <cstddef>
#include <cstdint>

#include <memory>
#include <vector>

#include <sys/uio.h>

namespace hatch {

  class block {
  public:
    uint8_t* head;
    uint8_t* writable;
    uint8_t* readable;
  };

  class chunk {
  public:
    size_t size; // number of blocks in this chunk
    block* head; // address of first block in this chunk
    pointer<chunk> prev; // next chunk in chain
    pointer<chunk> next; // previous chunk in chain
  };

  class chain {
  public:
    size_t size; // size of the chain in bytes
    pointer<chunk> head; // first chunk in the chain
    pointer<chunk> tail; // last chunk in the chain
  };

  class consumer {
  public:
    virtual ~consumer() = default;
    virtual size_t pull_from(const uint8_t* data, size_t length) = 0;
    virtual size_t pull_from(const iovec* data, size_t length) = 0;
  };

  class producer {
  public:
    virtual ~producer() = default;
    virtual size_t push_to(uint8_t* data, size_t length) = 0;
    virtual size_t push_to(iovec* data, size_t length) = 0;
  };

  class streamer {
  public:

    streamer(size_t block_size, size_t block_count, size_t limit);
    ~streamer();

    pointer<chain> produce(producer& producer, pointer<chain> buffer);
    pointer<chain> consume(consumer& consumer, pointer<chain> buffer);

  private:
    const size_t _size;
    const size_t _count;
    const size_t _limit;

    uint8_t* _memory;
    block* _block;

    class index {
    public:
      size_t color;
      pointer<index> head;
      pointer<index> prev;
      pointer<index> next;
      pointer<chunk> chunk;
    };

    pointer<index> _root;

    void prev_rotate(pointer<index> index);
    void next_rotate(pointer<index> index);
    void insert_fixup(pointer<index> index);
    void delete_fixup(pointer<index> index);
    void insert_index(pointer<index> index);
    void delete_index(pointer<index> index);

    iovec* _iovecs;
  };

}

#endif
