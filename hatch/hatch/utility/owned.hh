#ifndef HATCH_OWNED_HH
#define HATCH_OWNED_HH

#ifndef HATCH_OWNING_HH
#error "do not include owned.hh directly. include owning.hh instead."
#endif

#include <hatch/utility/chain.hh>

namespace hatch {

  template <class T, class U, template <class> class Ref>
  class owned : public chain<U, Ref> {
  public:
    friend class owner<T, U, Ref>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    explicit owned(Ref<T> owner = nullptr);
    ~owned();

    owned(owned&& moved) noexcept;
    owned& operator=(owned&& moved) noexcept;

    owned(const owned& copied);
    owned& operator=(const owned& copied);

    /////////////
    // Owning. //
    /////////////

  protected:
    Ref<T> _owner;

    void attach(Ref<T> owner);
    void detach();
  };

} // namespace hatch

#endif // HATCH_OWNED_HH