#ifndef HATCH_META_DEFS_HH
#define HATCH_META_DEFS_HH

#ifndef HATCH_META_HH
#error "do not include meta_decl.hh directly. include meta.hh instead."
#endif

namespace hatch {

  template <class T>
  using voided_t = typename voided<T>::type;

  template <auto V>
  using typed_t = typed<V>;

  template <auto V>
  constexpr typed_t<V> typed_v = {};

  template <class T>
  constexpr bool complete_v = complete<T>::value;

} // namespace hatch

#endif // HATCH_META_DEFS_HH