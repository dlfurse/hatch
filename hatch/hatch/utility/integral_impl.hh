#ifndef HATCH_INTEGRAL_IMPL_HH
#define HATCH_INTEGRAL_IMPL_HH

#ifndef HATCH_INTEGRAL_HH
#error "do not include integral_impl.hh directly. include integral.hh instead."
#endif

#include <climits> // CHAR_BIT
#include <cstddef> // size_t
#include <cstdint> // int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t

namespace hatch {

  template <>
  class integral<widths::bits8, signs::is_signed> {
  public:
    using type = int8_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = INT8_MIN;
    static type constexpr max = INT8_MAX;
  };

  template <>
  class integral<widths::bits8, signs::is_unsigned> {
  public:
    using type = uint8_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = 0;
    static type constexpr max = UINT8_MAX;
  };

  template <>
  class integral<widths::bits16, signs::is_signed> {
  public:
    using type = int16_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = INT16_MIN;
    static type constexpr max = INT16_MAX;
  };

  template <>
  class integral<widths::bits16, signs::is_unsigned> {
  public:
    using type = uint16_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = 0;
    static type constexpr max = UINT16_MAX;
  };

  template <>
  class integral<widths::bits32, signs::is_signed> {
  public:
    using type = int32_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = INT32_MIN;
    static type constexpr max = INT32_MAX;
  };

  template <>
  class integral<widths::bits32, signs::is_unsigned> {
  public:
    using type = uint32_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = 0;
    static type constexpr max = UINT32_MAX;
  };

  template <>
  class integral<widths::bits64, signs::is_signed> {
  public:
    using type = int64_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = INT64_MIN;
    static type constexpr max = INT64_MAX;
  };

  template <>
  class integral<widths::bits64, signs::is_unsigned> {
  public:
    using type = uint64_t;
    static size_t constexpr bytes = sizeof(type);
    static size_t constexpr bits = CHAR_BIT * bytes;
    static type constexpr min = 0;
    static type constexpr max = UINT64_MAX;
  };

} // namespace hatch

#endif // HATCH_INTEGRAL_IMPL_HH