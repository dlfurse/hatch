#ifndef HATCH_INTEGRAL_IMPL_HH
#define HATCH_INTEGRAL_IMPL_HH

#ifndef HATCH_INTEGRAL_HH
#error "do not include integral_impl.hh directly. include integral.hh instead."
#endif

#include <cstdint> // int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t

namespace hatch {

  template <>
  class integrals<widths::bits8, signs::sign> {
  public:
    using type = int8_t;
    static type constexpr min = INT8_MIN;
    static type constexpr max = INT8_MAX;
  };

  template <>
  class integrals<widths::bits8, signs::nosign> {
  public:
    using type = uint8_t;
    static type constexpr min = 0;
    static type constexpr max = UINT8_MAX;
  };

  template <>
  class integrals<widths::bits16, signs::sign> {
  public:
    using type = int16_t;
    static type constexpr min = INT16_MIN;
    static type constexpr max = INT16_MAX;
  };

  template <>
  class integrals<widths::bits16, signs::nosign> {
  public:
    using type = uint16_t;
    static type constexpr min = 0;
    static type constexpr max = UINT16_MAX;
  };

  template <>
  class integrals<widths::bits32, signs::sign> {
  public:
    using type = int32_t;
    static type constexpr min = INT32_MIN;
    static type constexpr max = INT32_MAX;
  };

  template <>
  class integrals<widths::bits32, signs::nosign> {
  public:
    using type = uint32_t;
    static type constexpr min = 0;
    static type constexpr max = UINT32_MAX;
  };

  template <>
  class integrals<widths::bits64, signs::sign> {
  public:
    using type = int64_t;
    static type constexpr min = INT64_MIN;
    static type constexpr max = INT64_MAX;
  };

  template <>
  class integrals<widths::bits64, signs::nosign> {
  public:
    using type = uint64_t;
    static type constexpr min = 0;
    static type constexpr max = UINT64_MAX;
  };

} // namespace hatch

#endif // HATCH_INTEGRAL_IMPL_HH