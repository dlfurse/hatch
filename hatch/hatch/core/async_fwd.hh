#ifndef HATCH_ASYNC_FWD_HH
#define HATCH_ASYNC_FWD_HH

#include <tuple> // std::tuple
#include <type_traits> // std::result_of_t

#include <hatch/utility/meta.hh>

namespace hatch {
  template <class ...T>
  class promise;

  template <class F, class ...T>
  using mapped_promise = rewrapped<promise, std::tuple, std::result_of_t<F(T...)>>;


  template <class ...T>
  class future;

  template <class F, class ...T>
  using mapped_future = rewrapped<future, std::tuple, std::result_of_t<F(T...)>>;
}

#endif // HATCH_ASYNC_FWD_HH
