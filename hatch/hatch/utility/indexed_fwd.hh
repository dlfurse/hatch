#ifndef HATCH_INDEXED_FWD_HH
#define HATCH_INDEXED_FWD_HH

#include <hatch/utility/integral_fwd.hh>

namespace hatch {

  template<class T, widths Width, typename integral<Width, signs::is_unsigned>::type Stride = sizeof(T), typename integral<Width, signs::is_unsigned>::type Offset = 0>
  class indexed;

} // namespace hatch

#endif // HATCH_INDEXED_FWD_HH