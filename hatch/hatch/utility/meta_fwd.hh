#ifndef HATCH_META_FWD_HH
#define HATCH_META_FWD_HH

namespace hatch {

  /**
   * Voided.
   *
   * Returns the void type for any argument given.
   * Useful as a type shunt in applying SFINAE techniques.
   */

  template <class ...T>
  class voided;

  /**
   * Typed.
   *
   * Returns
   */

  template <auto V>
  class typed;

  /**
   * Complete.
   *
   * Detects if the given type is complete or not.
   */

  template <class T, class U = void>
  class complete;

} // namespace hatch

#endif // HATCH_META_HH
