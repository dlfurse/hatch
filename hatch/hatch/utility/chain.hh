#ifndef HATCH_CHAIN_HH
#define HATCH_CHAIN_HH

#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>

namespace hatch {

  template <class T, template <class> class Ref>
  class chain_data;

  template <class T>
  class chain_data<T, pointed> {
    Ref<T> _prev;
    Ref<T> _next;
  };

  template <class T, template <class> class Ref>
  class chain {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  protected:
    chain();
    ~chain();

    chain(chain&& moved) noexcept;
    chain& operator=(chain&& moved) noexcept;

  public:
    chain(const chain&) = delete;
    chain& operator=(const chain&) = delete;

  ////////////////
  // Structure. //
  ////////////////

  protected:
    const Ref<T> _self;
    Ref<T> _prev;
    Ref<T> _next;

  ////////////////
  // Accessors. //
  ////////////////

  protected:
    bool alone() const;

    Ref<T> prev();
    const Ref<T> prev() const;

    Ref<T> next();
    const Ref<T> next() const;

  ///////////////
  // Mutators. //
  ///////////////

  protected:
    void splice(Ref<T> node);

  ///////////////////
  // Accumulators. //
  ///////////////////

  protected:
    template <class U>
    void foreach(U&& callable);

    template <class U>
    void foreach(U&& callable) const;
  };

} // namespace hatch

#include <hatch/utility/chain_impl.hh>

#endif // HATCH_CHAIN_HH
