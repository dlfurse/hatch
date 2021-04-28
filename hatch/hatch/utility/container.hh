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
    class inherits;

    //////////////////
    // Constructor. //
    //////////////////

  public:
    template <class ...Args>
    container(Args&&... args);

    ////////////////
    // Container. //
    ////////////////

  private:
    std::conditional_t<complete<T>, aggregates, inherits> _policy;

  public:
    T& get() const;
  };

} // namespace hatch

#include <hatch/utility/container_impl.hh>

#endif // HATCH_CONTAINER_HH