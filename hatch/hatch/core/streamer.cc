#include <hatch/core/streamer.hh>

#include <hatch/core/memory.hh>

#include <cassert>

namespace hatch {

  void streamer::prev_rotate(index* here) {
    auto head = here->head;
    auto next = here->next;
    auto hook = next->prev;

    next->head = head;
    next->prev = here;
    here->head = next;
    here->next = hook;
    if (hook) {
      hook->head = here;
    }
  }

  void streamer::next_rotate(index* here) {
    auto head = here->head;
    auto prev = here->prev;
    auto hook = prev->next;

    prev->head = head;
    prev->next = here;
    here->head = prev;
    here->prev = hook;
    if (hook) {
      hook->head = here;
    }
  }

  void streamer::insert_fixup(index *here) {
    while (here->head && here->head->color == 1) {
      auto head = here->head;
      auto gran = head->head;
      if (head == gran->prev) {
        auto aunt = gran->next;
        if (aunt && aunt->color == 1) {
          gran->color = 1;
          head->color = 0;
          aunt->color = 0;
          here = gran;
        } else {
          if (here == head->next) {
            here = head;
            head = here->head;
            gran = head->head;
            prev_rotate(here);
          }
          gran->color = 1;
          head->color = 0;
          next_rotate(gran);
        }
      } else {
        auto aunt = gran->prev;
        if (aunt && aunt->color == 1) {
          gran->color = 1;
          head->color = 0;
          aunt->color = 0;
          here = gran;
        } else {
          if (here == head->prev) {
            here = head;
            head = here->head;
            gran = head->head;
            next_rotate(here);
          }
          gran->color = 1;
          head->color = 0;
          prev_rotate(gran);
        }
      }
    }
  }

  void streamer::insert_index(index* index) {
    auto current = _index;
    while (true) {
      if (index->chunk < current->chunk) {
        if (current->prev) {
          current = current->prev;
        } else {
          index->head = current;
          current->prev = index;
          break;
        }
      } else {
        if (current->next) {
          current = current->next;
        } else {
          index->head = current;
          current->next = index;
          break;
        }
      }
    }
    index->color = 1;
    insert_fixup(index);
    return;
  }



  streamer::streamer(size_t size, size_t count, size_t limit) :
      _size{size},
      _count{count},
      _limit{limit} {

    // allocate actual stream memory
    _memory = static_cast<uint8_t *>(std::aligned_alloc(alignof(std::max_align_t),_size * _count));
    if (!_memory) {
      throw std::runtime_error("streamer cannot allocate memory");
    }

    _block = new block[_count];
    for (size_t index = 0; index < _count; index++) {
      uint8_t *address = _memory + index * _size;
      block *block = &_block[index];
      block->head = address;
      block->writable = address;
      block->readable = nullptr;
    }

    // TODO: replace the crap below with allocators

    _chunk = new chunk[_count];
    _next_chunk = _chunk;
    for (size_t i = 0; i < _count; i++) {
      chunk* chunk = &_chunk[i];
      chunk->size = 0;
      chunk->head = nullptr;
      chunk->prec = nullptr;
      chunk->succ = nullptr;
      chunk->next = i == 0 ? &_chunk[_count - 1] : &_chunk[i - 1];
      chunk->prev = i == _count - 1 ? &_chunk[0] : &_chunk[i + 1];
    }

    _index = new index[_count + 1];
    _next_index = _index;
    for (size_t i = 0; i < _count; i++) {
      index* index = &_index[i];
      index->color = 0;
      index->chunk = nullptr;
      index->head = nullptr;
      index->next = i == 0 ? &_index[_count - 1] : &_index[index - 1];
      index->prev = i == _count - 1 ? &_index[0] : &_index[index + 1];
    }
    _empty = &_index[_count];
    _empty->color = 0;


    _chain = new chain[_count];
    _next_chain = _chain;
    for (size_t i = 0; i < _count; i++) {
      chain* chain = &_chain[i];
      chain->size = 0;
      chain->head = nullptr;
      chain->next = i == 0 ? &_chain[_count - 1] : &_chain[i - 1];
      chain->prev = i == _count - 1 ? &_chain[0] : &_chain[i + 1];
    }

    _iovecs = new iovec[_count];

    auto chunk = alloc_chunk();
    chunk->size = _size * _count;
    chunk->head = _block;
    chunk->succ = chunk;
    chunk->prec = chunk;
    chunk->next = chunk;
    chunk->prev = chunk;

    auto index = alloc_index();
    index->color = 0;
    index->chunk = chunk;


  }

  streamer::~streamer() {
    std::free(_memory);
    delete [] _block;
    delete [] _chunk;
    delete [] _index;
    delete [] _chain;
    delete [] _iovecs;
  }


  streamer::buffer streamer::create(producer& producer) {
    iovec* iovec = _iovecs;
    size_t count = 0;

    chunk* chunk = _next_chunk;
    size_t bytes = 0;

    do {
      iovec->iov_base = chunk->_head->_head;
      iovec->iov_len = chunk->_size;
      ++count;
      ++iovec;

      chain = chain->_next_link;
      bytes += chain->_size;
    } while (chain != _free && bytes < _limit);

    auto written = producer.push_to(iovec, count);

    while (bytes > written) {
      chain = chain->_prev_link;
      bytes -= chain->_size;
    }

    buffer result;
    result._head = _free;
    result._tail = chain;
    _free = chain->_next_link;

    return result;
  }
}

