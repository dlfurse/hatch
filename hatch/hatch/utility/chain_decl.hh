#ifndef HATCH_CHAIN_DECL_HH
#define HATCH_CHAIN_DECL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_decl.hh directly. include chain.hh instead."
#endif

namespace hatch {

  template <template <class, auto...> class R, auto ...A>
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
    R<chain, A...> _prev;
    R<chain, A...> _next;

    /**
     * Accessors.
     */

  protected:
    bool alone() const;

    R<chain, A...> prev();
    const R<chain, A...> prev() const;

    R<chain, A...> next();
    const R<chain, A...> next() const;

    /**
     * Mutators.
     */

  protected:
    void splice(R<chain, A...> node);
  };

} // namespace hatch

#endif // HATCH_CHAIN_DECL_HH