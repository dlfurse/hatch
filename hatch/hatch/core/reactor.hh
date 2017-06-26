#ifndef _hatch_reactor_hh_
#define _hatch_reactor_hh_

namespace hatch {

  class reactor {
    private:
      int _epoll;

    public:
      reactor();
      ~reactor();
  };

}

#endif
