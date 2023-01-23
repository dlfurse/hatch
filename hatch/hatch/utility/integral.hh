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
    is_signed,
    is_unsigned,
  };

  template <widths Width, signs Sign>
  class integral;

} // namespace hatch

#include <hatch/utility/integral_impl.hh>

namespace hatch {
  /**
   * Types
   */

  template <widths Width, signs Sign>
  using integral_t = typename integral<Width, Sign>::type;

  template <widths Width>
  using signed_t = typename integral<Width, signs::is_signed>::type;

  using i8 = signed_t<widths::bits8>;
  using i16 = signed_t<widths::bits16>;
  using i32 = signed_t<widths::bits32>;
  using i64 = signed_t<widths::bits64>;

  template <widths Width>
  using unsigned_t = typename integral<Width, signs::is_unsigned>::type;

  using u8 = unsigned_t<widths::bits8>;
  using u16 = unsigned_t<widths::bits16>;
  using u32 = unsigned_t<widths::bits32>;
  using u64 = unsigned_t<widths::bits64>;

  /**
   * Bytes
   */

  template <widths Width, signs Sign>
  constexpr auto integral_bytes = integral<Width, Sign>::bytes;

  template <widths Width>
  constexpr auto signed_bytes = integral<Width, signs::is_signed>::bytes;

  template <widths Width>
  constexpr auto unsigned_bytes = integral<Width, signs::is_unsigned>::bytes;

  /**
   * Bits
   */

  template <widths Width, signs Sign>
  constexpr auto integral_bits = integral<Width, Sign>::bits;

  template <widths Width>
  constexpr auto signed_bits = integral<Width, signs::is_signed>::bits;

  template <widths Width>
  constexpr auto unsigned_bits = integral<Width, signs::is_unsigned>::bits;

  /**
   * Maxima
   */

  template <widths Width, signs Sign>
  constexpr auto integral_max = integral<Width, Sign>::max;

  template <widths Width>
  constexpr auto signed_max = integral<Width, signs::is_signed>::max;

  template <widths Width>
  constexpr auto unsigned_max = integral<Width, signs::is_unsigned>::max;

  /**
   * Minima
   */

  template <widths Width, signs Sign>
  constexpr auto integral_min = integral<Width, Sign>::min;

  template <widths Width>
  constexpr auto signed_min = integral<Width, signs::is_signed>::min;

  template <widths Width>
  constexpr auto unsigned_min = integral<Width, signs::is_unsigned>::min;
} // namespace hatch

#endif // HATCH_INTEGRAL_HH
