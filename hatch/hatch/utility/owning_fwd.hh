#ifndef HATCH_OWNING_FWD_HH
#define HATCH_OWNING_FWD_HH

namespace hatch {

  template <class T, class U, template <class> class Ref>
  class owner;

  template <class T, class U, template <class> class Ref>
  class owned;

} // namespace hatch

#endif // HATCH_OWNING_FWD_HH