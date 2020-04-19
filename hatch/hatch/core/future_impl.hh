#ifndef HATCH_FUTURE_IMPL_HH
#define HATCH_FUTURE_IMPL_HH

#ifndef HATCH_ASYNCHRONOUS_HH
#error "do not include future_impl.hh directly.  include asynchronous.hh instead."
#endif

namespace hatch {

  template <class ...T>
  future<T...>::future(promise<T...>* promise) :
      _promise{promise},
      _state{state::pending} {
    if (_promise) {
      _promise->_futures.insert(this);
    }
  }

  template <class ...T>
  future<T...>::~future() {
    if (_promise) {
      _promise->_futures.erase(this);
    }

    if (_state == state::completed) {
      _storage._value.~stored();
    } else if (_state == state::failed) {
      _storage._exception.~exception_ptr();
    }
  }

  template <class ...T>
  future<T...>::future(future&& moved) noexcept :
      _promise{moved._promise},
      _state{moved._state} {
    if (_promise) {
      _promise->_futures.insert(this);
    }

    if (_state == state::completed) {
      new (&_storage._value) stored(std::move(moved._storage._value));
      moved._storage._value.~stored();
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(std::move(moved._storage._exception));
      moved._storage._exception.~exception_ptr();
    }

    if (moved._promise) {
      moved._promise->_futures.erase(&moved);
    }
    moved._promise = nullptr;
    moved._state = state::moved;
  }

  template <class ...T>
  future<T...>& future<T...>::operator=(future&& moved) noexcept {
    if (_promise) {
      _promise->_futures.erase(this);
    }

    _promise = moved._promise;
    _state = moved._state;

    if (_promise) {
      _promise->_futures.insert(this);
    }

    if (_state == state::completed) {
      new (&_storage._value) stored(std::move(moved._storage._value));
      moved._storage._value.~stored();
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(std::move(moved._storage._exception));
      moved._storage._exception.~exception_ptr();
    }

    if (moved._promise) {
      moved._promise->_futures.erase(&moved);
    }
    moved._promise = nullptr;
    moved._state = state::moved;

    return *this;
  }

  template <class ...T>
  future<T...>::future(const future& copied) :
      _promise{copied._promise},
      _state{copied._state} {
    if (_promise) {
      _promise->_futures.insert(this);
    }

    if (_state == state::completed) {
      new (&_storage._value) stored(copied._storage._value);
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(copied._storage._exception);
    }
  }

  template <class ...T>
  future<T...>& future<T...>::operator=(const future& copied) {
    if (_promise) {
      _promise->_futures.erase(this);
    }

    _promise = copied._promise;
    _state = copied._state;

    if (_promise) {
      _promise->_futures.insert(this);
    }

    if (_state == state::completed) {
      new (&_storage._value) stored(copied._storage._value);
    } else if (_state == state::failed) {
      new (&_storage._exception) std::exception_ptr(copied._storage._exception);
    }

    return *this;
  }

  template <class ...T>
  future<T...>::future(const T&... data) :
      _promise{nullptr},
      _state{state::completed} {
    new (&_storage._value) stored(data...);
  }

  template <class ...T>
  template <class>
  future<T...>::future(const stored& data) :
      _promise{nullptr},
      _state{state::completed} {
    new (&_storage._value) stored(data);
  }

  template <class ...T>
  future<T...>::future(const std::exception_ptr& excp) :
      _promise{nullptr},
      _state{state::failed} {
    new (&_storage._exception) std::exception_ptr(excp);
  }

  template <class ...T>
  std::enable_if_t<future<T...>::complex, void> future<T...>::complete(const stored& data) {
    assert(_state == state::pending);
    new (&_storage._value) typename future<T...>::stored(data);
    _state = state::completed;
    _promise = nullptr;
  }

  template <class ...T>
  void future<T...>::complete(const T&... data) {
    assert(_state == state::pending);
    new (&_storage._value) typename future<T...>::stored(data...);
    _state = state::completed;
    _promise = nullptr;
  }

  template <class ...T>
  void future<T...>::fail(const std::exception_ptr &excp) {
    assert(_state == state::pending);
    new (&_storage._exception) std::exception_ptr(excp);
    _state = state::failed;
    _promise = nullptr;
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
  bool future<T...>::is_moved() const {
    return _state == state::moved;
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
        assert(_promise);
        return _promise->then(std::move(function));
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
    return {nullptr};
  }

  template <class ...T>
  template <class F>
  future<T...> future<T...>::recover(F&& function) {
    switch (_state) {
      case state::pending:
        assert(_promise);
        return _promise->recover(std::move(function));
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
    return {nullptr};
  }

} // namespace hatch

#endif // HATCH_FUTURE_IMPL_HH