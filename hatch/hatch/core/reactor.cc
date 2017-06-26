#include <hatch/core/reactor.hh>

#include <sys/epoll.h>
#include <unistd.h>

namespace hatch {
  reactor::reactor() {
    _epoll = epoll_create(2048);
  }

  reactor::~reactor() {
    close(_epoll);
  }
}

