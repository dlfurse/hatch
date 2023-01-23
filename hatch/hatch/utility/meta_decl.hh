#ifndef HATCH_META_DECL_HH
#define HATCH_META_DECL_HH

#ifndef HATCH_META_HH
#error "do not include meta_decl.hh directly. include meta.hh instead."
#endif

namespace hatch {

  template <class T>
  constexpr bool complete_v = complete<T>::value;

  template <template <class...> class Out, template <class...> class In, class ...T>
  using rewrapped_t = typename wrapper<Out<>, In<T...>>::type;

  template <template <class...> class Wrap, class ...T>
  using flatwrapped_t = typename wrapper<Wrap<>, Wrap<T...>>::type;

} // namespace hatch

#endif // HATCH_META_DECL_HH