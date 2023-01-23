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
   * Enshrines an integer as a type.
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

  /**
   * Wrapper.
   *
   * Takes a type formed from template instantiations and replaces those with
   * instantiations of another templated type.
   */

  template <class Out, class In>
  class wrapper;

} // namespace hatch

#endif // HATCH_META_HH
