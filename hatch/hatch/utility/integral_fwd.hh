#ifndef HATCH_INTEGRAL_FWD_HH
#define HATCH_INTEGRAL_FWD_HH

namespace hatch {

  enum class widths {
    bits8,
    bits16,
    bits32,
    bits64,
  };

  enum class signs {
    is_signed,
    is_unsigned,
  };

  template <widths Width, signs Sign>
  class integral;

} // namespace hatch

#endif