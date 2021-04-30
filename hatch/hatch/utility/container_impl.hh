#ifndef HATCH_CONTAINER_IMPL_HH
#define HATCH_CONTAINER_IMPL_HH

#ifndef HATCH_CONTAINER_HH
#error "do not include hatch_container_impl.hh directly. include container.hh instead."
#endif

namespace hatch {

  template <class T>
  class container<T>::aggregates {
  private:
    T _instance;

  public:
    template <class ...Args>
    explicit aggregates(Args&&... args) :
        _instance{std::forward<Args>(args)...} {
    }
    ~aggregates() = default;

    explicit aggregates(T&& moved) noexcept :
        _instance{std::move(moved)} {
    }
    aggregates& operator=(T&& moved) noexcept {
      _instance = std::move(moved);
      return *this;
    }

    explicit aggregates(const T& copied) :
        _instance{copied} {
    }
    aggregates& operator=(const T& copied) {
      _instance = copied;
      return *this;
    }

    T& get(const container*) const {
      return const_cast<T&>(reinterpret_cast<const T&>(_instance));
    }
  };

  template <class T>
  class container<T>::inherits {
  public:
    inherits() = default;
    ~inherits() = default;

    explicit inherits(T&& moved) noexcept {
    }
    inherits& operator=(T&& moved) noexcept {
      return *this;
    }

    explicit inherits(const T& copied) {
    }
    inherits& operator=(const T& copied) {
      return *this;
    }

    T& get(const container* container) const {
      return const_cast<T&>(static_cast<const T&>(*container));
    }
  };

  //////////////////
  // Constructor. //
  //////////////////

  template <class T>
  template <class ...Args>
  container<T>::container(Args&&... args) :
    _policy{std::forward<Args>(args)...} {
  }

  template <class T>
  container<T>::container(T&& moved) noexcept :
      _policy{std::move(moved)} {
  }

  template <class T>
  container<T>& container<T>::operator=(T&& moved) noexcept {
    _policy = std::move(moved);
    return *this;
  }

  template <class T>
  container<T>::container(const T& moved) :
      _policy{moved} {
  }

  template <class T>
  container<T>& container<T>::operator=(const T& copied) {
    _policy = copied;
    return *this;
  }

  ////////////////
  // Container. //
  ////////////////

  template <class T>
  T& container<T>::get() const {
    return _policy.get(this);
  }

} // namespace hatch

#endif // HATCH_CONTAINER_IMPL_HH