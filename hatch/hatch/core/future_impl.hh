#ifndef HATCH_FUTURE_IMPL_HH
#define HATCH_FUTURE_IMPL_HH

#ifndef HATCH_ASYNC_HH
#error "do not include future_impl.hh directly.  include async.hh instead."
#endif // HATCH_ASYNC_HH

namespace hatch {

  template <class ...T>
  void future<T...>::detach() {
    if (_prev) {
      _prev->_next = _next;
      if (_next) {
        _next->_prev = _prev;
      }
    } else {
      if (_next) {
        _promise->_future = _next;
        _next->_prev = _prev;
      } else {
        _promise->_future = nullptr;
      }
    }
  }

  template <class ...T>
  void future<T...>::before(future& future) {
    _prev = future._prev;
    if (_prev) {
      _prev->_next = this;
    }
    _next = &future;
    future._prev = this;
  }

  template <class ...T>
  void future<T...>::after(future& future) {
    _next = future._next;
    if (_next) {
      _next->_prev = this;
    }
    _prev = &future;
    future._next = this;
  }

  template <class ...T>
  void future<T...>::replace(future& future) {
    _promise = future._promise;
    if (_promise) {
      _state = future._state;
      _next = future._next;
      if (_next) {
        _next->_prev = this;
      }
      _prev = future._prev;
      if (_prev) {
        _prev->_next = this;
      } else {
        _promise->_future = this;
      }
    }
    future._promise = nullptr;
  }

  template <class ...T>
  future<T...>::future(promise<T...>* promise) :
      _prev{nullptr},
      _next{nullptr},
      _promise{promise},
      _state{state::pending} {
  }

  template <class ...T>
  future<T...>::future() :
      _prev{nullptr},
      _next{nullptr},
      _promise{nullptr},
      _state{state::detached} {
  }

  template <class ...T>
  future<T...>::~future() {
    if (_state == state::pending) {
      detach();
    } else if (_state == state::completed) {
      _storage._value.~stored();
    } else if (_state == state::failed) {
      _storage._exception.~exception_ptr();
    }
  }

  template <class ...T>
  future<T...>::future(future&& moved) noexcept :
      _prev{nullptr},
      _next{nullptr},
      _promise{moved._promise},
      _state{moved._state} {

    if (_state == state::pending) {
      replace(moved);
    } else if (_state == state::completed) {
      new (&_storage._value) stored(std::move(moved._storage._value));
      moved._storage._value.~stored();
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(std::move(moved._storage._exception));
      moved._storage._exception.~exception_ptr();
    }

    moved._promise = nullptr;
    moved._state = state::detached;
  }

  template <class ...T>
  future<T...>& future<T...>::operator=(future&& moved) noexcept {
    if (_state == state::pending) {
      detach();
    }

    _promise = moved._promise;
    _state = moved._state;

    if (_state == state::pending) {
      replace(moved);
    } else if (_state == state::completed) {
      new (&_storage._value) stored(std::move(moved._storage._value));
      moved._storage._value.~stored();
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(std::move(moved._storage._exception));
      moved._storage._exception.~exception_ptr();
    }

    moved._promise = nullptr;
    moved._state = state::detached;

    return *this;
  }

  template <class ...T>
  future<T...>::future(const future& copied) :
      _prev{nullptr},
      _next{nullptr},
      _promise{copied._promise},
      _state{copied._state} {
    if (_state == state::pending) {
      after(const_cast<future&>(copied));
    } else if (_state == state::completed) {
      new (&_storage._value) stored(copied._storage._value);
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(copied._storage._exception);
    }
  }

  template <class ...T>
  future<T...>& future<T...>::operator=(const future& copied) {
    if (_state == state::pending) {
      detach();
    }

    _promise = copied._promise;
    _state = copied._state;

    if (_state == state::pending) {
      after(copied);
    } else if (_state == state::completed) {
      new (&_storage._value) stored(copied._storage._value);
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(copied._storage._exception);
    }

    return *this;
  }

  template <class ...T>
  future<T...>::future(const T&... data) :
      _state{state::completed} {
    new (&_storage._value) stored(data...);
  }

  template <class ...T>
  template <class S, class>
  future<T...>::future(const S& data) :
      _state{state::completed} {
    new (&_storage._value) stored(data);
  }

  template <class ...T>
  future<T...>::future(const std::exception_ptr& excp) :
      _state{state::failed} {
    new (&_storage._exception) std::exception_ptr(excp);
  }

  template <class ...T>
  bool future<T...>::is_detached() const {
    return _state == state::detached;
  }

  template <class ...T>
  bool future<T...>::is_pending() const {
    return _state == state::pending;
  }

  template <class ...T>
  bool future<T...>::is_finished() const {
    return (_state == state::completed || _state == state::failed);
  }

  template <class ...T>
  bool future<T...>::is_completed() const {
    return _state == state::completed;
  }

  template <class ...T>
  bool future<T...>::is_failed() const {
    return _state == state::failed;
  }

  template <class ...T>
  const typename future<T...>::stored& future<T...>::get() const& {
    if (_state == state::failed) {
      std::rethrow_exception(_storage._exception);
    }
    assert(_state == state::completed);
    return _storage._value;
  }

  template <class ...T>
  typename future<T...>::stored&& future<T...>::get() && {
    if (_state == state::failed) {
      auto e = std::move(_storage._exception);

      _state = state::invalid;
      _storage._exception.~exception_ptr();

      std::rethrow_exception(std::move(e));
    }
    assert(_state == state::completed);
    return std::move(_storage._value);
  }

  template <class ...T>
  const typename future<T...>::stored& future<T...>::value() const& {
    assert(_state == state::completed);
    return _storage._value;
  }

  template <class ...T>
  typename future<T...>::stored&& future<T...>::value() && {
    assert(_state == state::completed);
    return std::move(_storage._value);
  }

  template <class ...T>
  std::exception_ptr future<T...>::exception() const {
    assert(_state == state::failed);
    return _storage._exception;
  }

  template <class ...T>
  template <class F>
  mapped_future<F, T...> future<T...>::then(F&& function) {
    switch (_state) {
      case state::pending:
        return _promise->then(std::forward<F&&>(function));
      case state::completed:
        try {
          return mapped_future<F, T...>(apply(function, _storage._value));
        } catch (...) {
          return mapped_future<F, T...>(std::current_exception());
        }
      case state::failed:
        return mapped_future<F, T...>(_storage._exception);
      default:
        break;
    }

    assert(false);
    return {};
  }

  template <class ...T>
  template <class F>
  future<T...> future<T...>::recover(F&& function) {
    switch (_state) {
      case state::pending:
        return _promise->recover(std::forward<F&&>(function));
      case state::completed:
        return future<T...>(*this);
      case state::failed:
        try {
          return future<T...>(function(_storage._exception));
        } catch (...) {
          return future<T...>(std::current_exception());
        }
      default:
        break;
    }

    assert(false);
    return {};
  }

} // namespace hatch

#endif // HATCH_FUTURE_IMPL_HH
