#ifndef HATCH_ASYNCHRONOUS_HH
#define HATCH_ASYNCHRONOUS_HH

#include <cassert>
#include <functional>
#include <list>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_set>

#include <hatch/utility/rewrapper.hh>

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

#include <hatch/core/promise.hh>
#include <hatch/core/future.hh>
#include <hatch/core/promise_impl.hh>
#include <hatch/core/future_impl.hh>

#endif // HATCH_FUTURE_HH