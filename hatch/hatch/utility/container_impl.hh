#ifndef HATCH_CONTAINER_IMPL_HH
#define HATCH_CONTAINER_IMPL_HH

#ifndef HATCH_CONTAINER_HH
#error "do not include hatch_container_impl.hh directly. include container.hh instead."
#endif

#include <iostream>

namespace hatch {

  template <class T>
  class inherits {
  protected:
    template <class ...Args>
    explicit inherits(Args&&...) {
    }

    template <class ...Args>
    inherits& operator=(Args&&...) {
      return *this;
    }
  };

  template <class T>
  class aggregates {
  protected:
    template <class ...Args>
    explicit aggregates(Args&&... args) :
        _value{std::forward<Args>(args)...} {
    }

    template <class ...Args>
    aggregates& operator=(Args&&... args) {
      _value.operator=(std::forward<Args>(args)...);
      return *this;
    }

    T _value;
  };

  //////////////////
  // Constructor. //
  //////////////////

  template <class T>
  template <class ...Args>
  container<T>::container(Args&&... args) :
    std::conditional_t<complete<T>, aggregates<T>, inherits<T>>{std::forward<Args>(args)...} {
  }

  template <class T>
  template <class ...Args>
  container<T>& container<T>::operator=(Args&&... args) {
    std::conditional_t<complete<T>, aggregates<T>, inherits<T>>::operator=(std::forward<Args>(args)...);
    return *this;
  }

  ////////////////
  // Container. //
  ////////////////

  template <class T>
  template <class U, std::enable_if_t<complete<U>, bool>>
  U& container<T>::get() const {
    return const_cast<U&>(this->_value);
  }

  template <class T>
  template <class U, std::enable_if_t<!complete<U>, bool>>
  U& container<T>::get() const {
    return const_cast<U&>(static_cast<const U&>(*this));
  }

} // namespace hatch

#endif // HATCH_CONTAINER_IMPL_HH