#ifndef HATCH_INTEGRAL_HH
#define HATCH_INTEGRAL_HH

namespace hatch {

  enum class widths {
    bits8,
    bits16,
    bits32,
    bits64,
  };

  enum class signs {
    sign,
    nosign,
  };

  template <widths Width, signs Sign>
  class integrals;

  template <widths Width, signs Sign>
  using integral = typename integrals<Width, Sign>::type;

  template <widths Width>
  using signint = typename integrals<Width, signs::sign>::type;

  template <widths Width>
  using nosignint = typename integrals<Width, signs::nosign>::type;

  template <widths Width, signs Sign>
  constexpr auto maxval = integrals<Width, Sign>::max;

  template <widths Width>
  constexpr auto signmax = integrals<Width, signs::sign>::max;

  template <widths Width>
  constexpr auto nosignmax = integrals<Width, signs::nosign>::max;

  template <widths Width, signs Sign>
  constexpr auto minval = integrals<Width, Sign>::min;

  template <widths Width>
  constexpr auto signmin = integrals<Width, signs::sign>::min;

  template <widths Width>
  constexpr auto nosignmin = integrals<Width, signs::nosign>::min;

} // namespace hatch

#include <hatch/utility/integral_impl.hh>

#endif // HATCH_INTEGRAL_HH
