#ifndef HATCH_META_DECL_HH
#define HATCH_META_DECL_HH

#ifndef HATCH_META_HH
#error "do not include meta_decl.hh directly. include meta.hh instead."
#endif

namespace hatch {

  template <class T>
  constexpr bool complete_v = complete<T>::value;

} // namespace hatch

#endif // HATCH_META_DECL_HH