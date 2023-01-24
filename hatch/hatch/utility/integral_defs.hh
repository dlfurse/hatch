#ifndef HATCH_INTEGRAL_DEFS_HH
#define HATCH_INTEGRAL_DEFS_HH

#ifndef HATCH_INTEGRAL_HH
#error "do not include integral_impl.hh directly. include integral.hh instead."
#endif

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
  constexpr auto integral_bytes_v = integral<Width, Sign>::bytes;

  template <widths Width>
  constexpr auto signed_bytes_v = integral<Width, signs::is_signed>::bytes;

  template <widths Width>
  constexpr auto unsigned_bytes_v = integral<Width, signs::is_unsigned>::bytes;

  /**
   * Bits
   */

  template <widths Width, signs Sign>
  constexpr auto integral_bits_v = integral<Width, Sign>::bits;

  template <widths Width>
  constexpr auto signed_bits_v = integral<Width, signs::is_signed>::bits;

  template <widths Width>
  constexpr auto unsigned_bits_v = integral<Width, signs::is_unsigned>::bits;

  /**
   * Maxima
   */

  template <widths Width, signs Sign>
  constexpr auto integral_max_v = integral<Width, Sign>::max;

  template <widths Width>
  constexpr auto signed_max_v = integral<Width, signs::is_signed>::max;

  template <widths Width>
  constexpr auto unsigned_max_v = integral<Width, signs::is_unsigned>::max;

  /**
   * Minima
   */

  template <widths Width, signs Sign>
  constexpr auto integral_min_v = integral<Width, Sign>::min;

  template <widths Width>
  constexpr auto signed_min_v = integral<Width, signs::is_signed>::min;

  template <widths Width>
  constexpr auto unsigned_min_v = integral<Width, signs::is_unsigned>::min;

} // namespace hatch

#endif // HATCH_INTEGRAL_DEFS_HH