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
