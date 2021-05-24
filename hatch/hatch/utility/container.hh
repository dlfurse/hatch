#ifndef HATCH_CONTAINER_HH
#define HATCH_CONTAINER_HH

#include <hatch/utility/meta.hh>

#include <type_traits> // std::conditional_t
#include <utility> // std::forward

namespace hatch {

  template <class T>
  class inherits;

  template <class T>
  class aggregates;

  template <class T>
  class container : public std::conditional_t<complete<T>, aggregates<T>, inherits<T>> {
  private:

    //////////////////
    // Constructor. //
    //////////////////

  public:
    ~container() = default;

    template <class ...Args>
    container(Args&&... args);

    template <class ...Args>
    container& operator=(Args&&... args);

    ////////////////
    // Container. //
    ////////////////

  public:
    template <class U = T, std::enable_if_t<complete<U>, bool> = true>
    U& get() const;

    template <class U = T, std::enable_if_t<!complete<U>, bool> = true>
    U& get() const;
  };

} // namespace hatch

#include <hatch/utility/container_impl.hh>

#endif // HATCH_CONTAINER_HH