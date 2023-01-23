#ifndef HATCH_CHAIN_HH
#define HATCH_CHAIN_HH

#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>

namespace hatch {

  template <class T>
  class chain;

  template <class T, template <class...> class Ref, class ...Args>
  class chain<Ref<T, Args...>> {

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
    Ref<T, Args...> _prev;
    Ref<T, Args...> _next;

  ////////////////
  // Accessors. //
  ////////////////

  protected:
    bool alone() const;

    Ref<T, Args...> prev();
    const Ref<T, Args...> prev() const;

    Ref<T, Args...> next();
    const Ref<T, Args...> next() const;

  ///////////////
  // Mutators. //
  ///////////////

  protected:
    void splice(Ref<T, Args...> node);

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
