#ifndef HATCH_INDEXED_DEFS_HH
#define HATCH_INDEXED_DEFS_HH

#ifndef HATCH_INDEXED_HH
#error "do not include indexed_defs.hh directly. include indexed.hh instead."
#endif

namespace hatch {

  template <widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  class index_system {
  public:
    template <class T>
    using type = indexed<T, Width, Stride, Offset>;
  };

  template <widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  using index_system_t = typename index_system<Width, Stride, Offset>::type;

} // namespace hatch

#endif // HATCH_INDEXED_DEFS_HH