#ifndef HATCH_PROMISE_IMPL_HH
#define HATCH_PROMISE_IMPL_HH

#ifndef HATCH_ASYNCHRONOUS_HH
#error "do not include promise_impl.hh directly. include asynchronous.hh instead."
#endif

namespace hatch {

  template <class ...T>
  promise<T...>::promise() :
      _state{state::pending},
      _futures{},
      _continuations{},
      _recovery{} {}

  template <class ...T>
  promise<T...>::~promise() {
    for (auto* future : _futures) {
      future->_promise = nullptr;
    }
  }

  template <class ...T>
  promise<T...>::promise(promise&& moved) noexcept :
      _state{moved._state},
      _futures{std::move(moved._futures)},
      _continuations{std::move(moved._continuations)},
      _recovery{std::move(moved._recovery)} {
    moved._state = state::moved;
    for (auto* future : _futures) {
      future->_promise = this;
    }
  }

  template <class ...T>
  promise<T...>& promise<T...>::operator=(promise&& moved) noexcept {
    _state = moved._state;
    _futures = std::move(moved._futures);
    _continuations = std::move(moved._continuations);
    _recovery = std::move(moved._recovery);
    moved._state = state::moved;
    for (auto* future : _futures) {
      future->_promise = this;
    }
    return *this;
  }

  template <class ...T>
  bool promise<T...>::is_moved() const {
    return _state == state::moved;
  }

  template <class ...T>
  bool promise<T...>::is_pending() const {
    return _state == state::pending;
  }

  template <class ...T>
  bool promise<T...>::is_finished() const {
    return (_state == state::completed || _state == state::failed);
  }

  template <class ...T>
  bool promise<T...>::is_completed() const {
    return _state == state::completed;
  }

  template <class ...T>
  bool promise<T...>::is_failed() const {
    return _state == state::failed;
  }

  template <class ...T>
  std::enable_if_t<(sizeof...(T) > 1), void> promise<T...>::complete(const stored& data) {
    assert(_state == state::pending);

    _state = state::completed;

    for (const auto& future : _futures) {
      future->complete(data);
    }

    for (const auto& continuation : _continuations) {
      continuation->complete(data);
    }
  }

  template <class ...T>
  void promise<T...>::complete(const T&... data) {
    assert(_state == state::pending);

    _state = state::completed;

    for (const auto& future : _futures) {
      future->complete(data...);
    }

    for (const auto& continuation : _continuations) {
      continuation->complete(data...);
    }
  }

  template <class ...T>
  void promise<T...>::fail(const std::exception_ptr& excp) {
    assert(_state == state::pending);

    if (_recovery) {
      _recovery.release()->handle(excp, *this);
    } else {
      _state = state::failed;

      for (const auto& future : _futures) {
        future->fail(excp);
      }

      for (const auto& continuation : _continuations) {
        continuation->fail(excp);
      }
    }
  }

  template <class ...T>
  future<T...> promise<T...>::awaited() {
    return {this};
  }

  template <class ...T>
  template <class F, class P>
  promise<T...>::continued<F, P>::continued(F&& function, P&& promise) :
      _function{std::move(function)},
      _promise{std::move(promise)} {}

  template <class ...T>
  template <class F, class P>
  std::enable_if_t<(sizeof...(T) > 1), void> promise<T...>::continued<F, P>::complete(const stored& data) {
    try {
      _promise.complete(std::apply(_function, data));
    } catch (...) {
      _promise.fail(std::move(std::current_exception()));
    }
  }

  template <class ...T>
  template <class F, class P>
  void promise<T...>::continued<F, P>::complete(const T&... data) {
    try {
      _promise.complete(_function(data...));
    } catch (...) {
      _promise.fail(std::move(std::current_exception()));
    }
  }

  template <class ...T>
  template <class F, class P>
  void promise<T...>::continued<F, P>::fail(const std::exception_ptr& excp) {
    _promise.fail(excp);
  }

  template <class ...T>
  template <class F>
  promise<T...>::recovered<F>::recovered(F&& function) :
      _function{std::move(function)} {}


  template <class ...T>
  template <class F>
  void promise<T...>::recovered<F>::handle(const std::exception_ptr& excp, promise& promise) {
    try {
      promise.complete(_function(excp));
    } catch (...) {
      promise.fail(std::move(std::current_exception()));
    }
  }

  template <class ...T>
  template <class F>
  mapped_future<F, T...> promise<T...>::then(F&& function) {
    mapped_promise<F, T...> promise;
    mapped_future<F, T...> future = promise.awaited();

    auto continuation = new continued<F>(std::move(function), std::move(promise));
    _continuations.emplace_back(continuation);

    return future;
  }

  template <class ...T>
  template <class F>
  future<T...> promise<T...>::recover(F &&function) {
    assert(!_recovery);

    auto recovery = new recovered<F>(std::move(function));
    _recovery.reset(recovery);

    return this;
  }

} // namespace hatch

// HATCH_PROMISE_IMPL_HH
