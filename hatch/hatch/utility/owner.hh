#ifndef HATCH_OWNER_HH
#define HATCH_OWNER_HH

#ifndef HATCH_OWNING_HH
#error "do not include owner.hh directly. include owning.hh instead."
#endif

namespace hatch {

  template <class T, class U>
  class owner {
  public:
    friend class owned<T, U>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    owner();
    ~owner();

    owner(owner&& moved) noexcept;
    owner& operator=(owner&& moved) noexcept;

  public:
    owner(const owner& copied) = delete;
    owner& operator=(const owner& copied) = delete;

    ///////////
    // Keep. //
    ///////////

  protected:
    U* _owned;

    template <class V>
    void foreach(V&& callable);

    template <class V>
    void foreach(V&& callable) const;

    void transfer_from(T&& owner);
    void disown_all();
  };

} // namespace hatch

#endif // HATCH_OWNER_HH