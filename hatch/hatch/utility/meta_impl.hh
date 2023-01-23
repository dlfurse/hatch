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

  /**
   * Wrapper.
   */

  template <template <class...> class Out, class ...T, template <class...> class In, class ...A, class ...B>
  class wrapper<Out<T...>, In<In<A...>, B...>> {
  public:
    using type = typename wrapper<Out<T...>, In<A..., B...>>::type;
  };

  template <template <class...> class Out, class ...T, template <class...> class In, class A, class ...B>
  class wrapper<Out<T...>, In<A, B...>> {
  public:
    using type = typename wrapper<Out<T..., A>, In<B...>>::type;
  };

  template <template <class...> class Out, template <class...> class In, class ...A, class ...B>
  class wrapper<Out<>, In<In<A...>, B...>> {
  public:
    using type = typename wrapper<Out<>, In<A..., B...>>::type;
  };

  template <template <class...> class Out, template <class...> class In, class A, class ...B>
  class wrapper<Out<>, In<A, B...>> {
  public:
    using type = typename wrapper<Out<A>, In<B...>>::type;
  };

  template <template <class...> class Out, class ...T, template <class...> class In>
  class wrapper<Out<T...>, In<>> {
  public:
    using type = Out<T...>;
  };

} // namespace hatch

#endif // HATCH_META_HH
