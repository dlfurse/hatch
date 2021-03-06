#ifndef HATCH_PROMISE_IMPL_HH
#define HATCH_PROMISE_IMPL_HH

#ifndef HATCH_ASYNC_HH
#error "do not include promise_impl.hh directly. include async.hh instead."
#endif

namespace hatch {

  template <class ...T>
  promise<T...>::promise() :
      _state{state::pending},
      _future{nullptr},
      _continuations{},
      _recovery{} {
  }

  template <class ...T>
  promise<T...>::~promise() {
    dispossess_futures();
  }

  template <class ...T>
  promise<T...>::promise(promise&& moved) noexcept :
      _state{moved._state},
      _future{moved._future},
      _continuations{std::move(moved._continuations)},
      _recovery{std::move(moved._recovery)} {
    repossess_futures();
    moved._state = state::moved;
    moved._future = nullptr;
  }

  template <class ...T>
  promise<T...>& promise<T...>::operator=(promise&& moved) noexcept {
    dispossess_futures();

    _state = moved._state;
    _future = moved._future;
    _continuations = std::move(moved._continuations);
    _recovery = std::move(moved._recovery);

    repossess_futures();
    moved._state = state::moved;
    moved._future = nullptr;

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
  void promise<T...>::dispossess_futures() {
    while (_future) {
      _future->_promise = nullptr;
      _future->_state = future<T...>::state::detached;
      _future->detach();
    };
  }

  template <class ...T>
  void promise<T...>::repossess_futures() {
    auto* current = _future;
    while (current) {
      current->_promise = this;
      assert(current->_state == future<T...>::state::pending);
      current = current->_next;
    };
  }


  template <class ...T>
  template <class F, class P>
  promise<T...>::continued<F, P>::continued(F&& function, P&& promise) :
      _function{std::move(function)},
      _promise{std::move(promise)} {}

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
  template <class S, class>
  void promise<T...>::complete(const S& data) {
    assert(is_pending());
    _state = state::completed;

    while (_future) {
      auto& current = *_future;
      _future->detach();
      new (&current._storage._value) typename future<T...>::stored(data);
      current._state = future<T...>::state::completed;
      current._promise = nullptr;
    };

    for (const auto& c : _continuations) {
      std::apply([&](const T&... args){c->complete(args...);}, data);
    }
  }

  template <class ...T>
  void promise<T...>::complete(const T&... data) {
    assert(is_pending());
    _state = state::completed;

    while (_future) {
      auto& current = *_future;
      _future->detach();
      new (&current._storage._value) typename future<T...>::stored(data...);
      current._state = future<T...>::state::completed;
      current._promise = nullptr;
    };

    for (const auto& continuation : _continuations) {
      continuation->complete(data...);
    }
  }

  template <class ...T>
  void promise<T...>::fail(const std::exception_ptr& excp) {
    assert(is_pending());

    if (_recovery) {
      auto* recovery = _recovery.release();
      _recovery.reset();
      recovery->handle(excp, *this);
      delete recovery;
    } else {
      _state = state::failed;

      while (_future) {
        auto& current = *_future;
        _future->detach();
        new (&current._storage._exception) std::exception_ptr(excp);
        current._state = future<T...>::state::failed;
        current._promise = nullptr;
      };

      for (const auto& continuation : _continuations) {
        continuation->fail(excp);
      }
    }
  }

  template <class ...T>
  future<T...> promise<T...>::awaited() {
    future<T...> future(this);

    if (_future) {
      _future->before(future);
    } else {
      _future = &future;
    }

    return future;
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
  future<T...> promise<T...>::recover(F&& function) {
    assert(!_recovery);

    auto recovery = new recovered<F>(std::move(function));
    _recovery.reset(recovery);

    return awaited();
  }

} // namespace hatch

#endif // HATCH_PROMISE_IMPL_HH
