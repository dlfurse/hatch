#ifndef HATCH_CONTAINER_IMPL_HH
#define HATCH_CONTAINER_IMPL_HH

#ifndef HATCH_CONTAINER_HH
#error "do not include hatch_container_impl.hh directly. include container.hh instead."
#endif

namespace hatch {

//  template <class T>
//  class container {
//  private:
    class aggregates {
    private:
      T _data;

    public:
      template <class ...Args>
      aggregates(Args&&... args) : _data{std::forward<Args>(args)...} {
      }

      T& get(const container*) const {
        return const_cast<T&>(reinterpret_cast<const T&>(_data));
      }
    };
//
//    class inherits {
//    public:
//      inherits() = default;
//
//      T& get(const container* container) const {
//        return const_cast<T&>(static_cast<const T&>(*container));
//      }
//    };
//
//    std::conditional_t<complete<T>, aggregates, inherits> _policy;
//
//  public:
//    template <class ...Args>
//    container(Args&&... args) : _policy{std::forward<Args>(args)...} {
//    }
//
//    T& get() const {
//      return _policy.get(this);
//    }
//  };

} // namespace hatch

#endif // HATCH_CONTAINER_IMPL_HH