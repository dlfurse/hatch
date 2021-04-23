#ifndef HATCH_CONTAINER_HH
#define HATCH_CONTAINER_HH

#include <hatch/utility/meta.hh>

#include <type_traits> // std::conditional_t
#include <utility> // std::forward

namespace hatch {

  template <class T>
  class container {
  private:
    class aggregates;

    class inherits {
    public:
      inherits() = default;

      T& get(const container* container) const {
        return const_cast<T&>(static_cast<const T&>(*container));
      }
    };

    std::conditional_t<complete<T>, aggregates, inherits> _policy;

  public:
    template <class ...Args>
    container(Args&&... args) : _policy{std::forward<Args>(args)...} {
    }

    T& get() const {
      return _policy.get(this);
    }
  };

}

#endif