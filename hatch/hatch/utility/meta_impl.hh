#ifndef HATCH_META_IMPL_HH
#define HATCH_META_IMPL_HH

#ifndef HATCH_META_HH
#error "do not include meta_impl.hh directly. include meta.hh instead."
#endif

namespace hatch {

  /**
   * Voided.
   */

  template <class ...T>
  class voided {
  public:
    using type = void;
  };

  /**
   * Typed.
   */

  template <auto V>
  class typed {;
  public:
    bool operator==(const typed<V>&) const {
      return true;
    }
    template <class T>
    bool operator==(const T&) const {
      return false;
    }

    bool operator!=(const typed<V>&) const {
      return false;
    }
    template <class T>
    bool operator!=(const T&) const {
      return true;
    }
  };

  /**
   * Complete.
   */

  template <class, class>
  class complete {
  public:
    static constexpr bool value = false;
  };

  template <class T>
  class complete<T, typename voided<decltype(sizeof(T))>::type> {
  public:
    static constexpr bool value = true;
  };

} // namespace hatch

#endif // HATCH_META_HH
