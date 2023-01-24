#ifndef HATCH_CONTAINER_DECL_HH
#define HATCH_CONTAINER_DECL_HH

#ifndef HATCH_CONTAINER_HH
#error "do not include container_decl.hh directly.  include container.hh instead."
#endif

#include <hatch/utility/meta.hh>

#include <type_traits> // std::conditional_t
#include <utility> // std::forward

namespace hatch {

  template <class T>
  class container : public std::conditional_t<complete_v<T>, aggregates<T>, inherits<T>> {
    /**
     * Construction and assignment.
     *
     * All arguments are forwarded directly to the containment policy.
     */

    public:
    ~container() = default;

    template <class ...Args>
    container(Args&&... args);

    template <class ...Args>
    container& operator=(Args&&... args);

    /**
     * Object retrieval.
     */

    public:
    template <class U = T, std::enable_if_t<complete_v<U>, bool> = true>
    U& get() const;

    template <class U = T, std::enable_if_t<!complete_v<U>, bool> = true>
    U& get() const;
  };

} // namespace hatch

#endif // HATCH_CONTAINER_DECL_HH