#ifndef HATCH_CHAIN_DECL_HH
#define HATCH_CHAIN_DECL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_decl.hh directly. include chain.hh instead."
#endif

namespace hatch {

  template <template <class> class Ref>
  class chain {

    /**
     * Construction and assignment.
     */

  protected:
    chain();
    ~chain();

    chain(chain&& moved) noexcept;
    chain& operator=(chain&& moved) noexcept;

  public:
    chain(const chain&) = delete;
    chain& operator=(const chain&) = delete;

    /**
     * Structure.
     */

  protected:
    Ref<chain> _prev;
    Ref<chain> _next;

    /**
     * Accessors.
     */

  protected:
    bool alone() const;

    Ref<chain> prev();
    const Ref<chain> prev() const;

    Ref<chain> next();
    const Ref<chain> next() const;

    /**
     * Mutators.
     */

  protected:
    void splice(Ref<chain> node);
  };

} // namespace hatch

#endif // HATCH_CHAIN_DECL_HH