#ifndef HATCH_BUFFER_HH
#define HATCH_BUFFER_HH

#include <cstddef>
#include <cstdint>

#include <memory>
#include <vector>

#include <sys/uio.h>

namespace hatch {

  class reader {
  public:
    virtual ~reader() = default;
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
  private:
    class block;
    class chunk;
    class index;
    class chain;

  public:
    using buffer = std::unique_ptr<chain>;

    streamer(size_t block_size, size_t block_count, size_t limit);
    ~streamer();

    buffer create(producer& producer);
    void destroy(buffer& buffer);

    void produce(producer& producer, buffer& buffer);
    void consume(reader& reader, buffer& buffer);

  private:
    const size_t _size;
    const size_t _count;
    const size_t _limit;

    uint8_t* _memory;
    block* _block;

    chunk* _chunk;
    chunk* _next_chunk;
    chunk* alloc_chunk();
    void free_chunk(chunk* chunk);

    index* _index;
    index* _next_index;
    index* alloc_index();
    void free_index(index* index);

    void prev_rotate(index* index);
    void next_rotate(index* index);
    void insert_fixup(index* index);
    void delete_fixup(index* index);
    void insert_index(index* index);
    void delete_index(index* index);


    chain* _chain;
    chain* _next_chain;
    chain* alloc_chain();
    void free_chain(chain* chain);

    iovec* _iovecs;
  };

}

#endif
