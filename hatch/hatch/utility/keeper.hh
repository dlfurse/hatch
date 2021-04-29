#ifndef HATCH_KEEPER_HH
#define HATCH_KEEPER_HH

#ifndef HATCH_KEEP_HH
#error "do not include keeper.hh directly. include keep.hh instead."
#endif

namespace hatch {

  template <class T, class U>
  class keeper {
  public:
    friend class kept<T, U>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  public:
    keeper();
    ~keeper();

    keeper(keeper&& moved) noexcept;
    keeper& operator=(keeper&& moved) noexcept;

    keeper(const keeper& copied) = delete;
    keeper& operator=(const keeper& copied) = delete;

    ///////////
    // Keep. //
    ///////////

  protected:
    U* _kept;

    template <class V>
    void foreach(V&& callable);

    template <class V>
    void foreach(V&& callable) const;

    void acquire(keeper<T, U>& keeper);
    void release();
  };

} // namespace hatch

#endif // HATCH_KEEPER_HH