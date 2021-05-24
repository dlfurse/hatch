#ifndef HATCH_REWRAPPER_HH
#define HATCH_REWRAPPER_HH

#include <cstdint> // uint8_t

namespace hatch {

  /**
   * Converters
   */

  template <class ...T>
  using voided = void;

  /**
   * Complete
   */

  template<class, class = void>
  constexpr bool complete = false;

  template<class T>
  constexpr bool complete<T, voided<decltype(sizeof(T))>> = true;

  /**
   * Wrappers
   */

  template <class Out, class In>
  class rewrapper;

  template <template <class...> class Out, class ...T, template <class...> class In, class ...A, class ...B>
  class rewrapper<Out<T...>, In<In<A...>, B...>> {
  public:
    using type = typename rewrapper<Out<T...>, In<A..., B...>>::type;
  };

  template <template <class...> class Out, class ...T, template <class...> class In, class A, class ...B>
  class rewrapper<Out<T...>, In<A, B...>> {
  public:
    using type = typename rewrapper<Out<T..., A>, In<B...>>::type;
  };

  template <template <class...> class Out, template <class...> class In, class ...A, class ...B>
  class rewrapper<Out<>, In<In<A...>, B...>> {
  public:
    using type = typename rewrapper<Out<>, In<A..., B...>>::type;
  };

  template <template <class...> class Out, template <class...> class In, class A, class ...B>
  class rewrapper<Out<>, In<A, B...>> {
  public:
    using type = typename rewrapper<Out<A>, In<B...>>::type;
  };

  template <template <class...> class Out, class ...T, template <class...> class In>
  class rewrapper<Out<T...>, In<>> {
  public:
    using type = Out<T...>;
  };

  template <template <class...> class Out, template <class...> class In, class ...T>
  using rewrapped = typename rewrapper<Out<>, In<T...>>::type;

  template <template <class...> class Wrap, class ...T>
  using flatwrapped = typename rewrapper<Wrap<>, Wrap<T...>>::type;

  ///////////////
  // Typechain //
  ///////////////

}

#endif // HATCH_REWRAPPER_HH
