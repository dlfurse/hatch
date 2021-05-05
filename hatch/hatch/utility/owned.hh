#ifndef HATCH_OWNED_HH
#define HATCH_OWNED_HH

#ifndef HATCH_OWNING_HH
#error "do not include owned.hh directly. include owning.hh instead."
#endif

#include <hatch/utility/chain.hh>

namespace hatch {

  template <class T, class U>
  class owned : public chain<U> {
  public:
    friend class owner<T, U>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    explicit owned(T* owner = nullptr);
    ~owned();

    owned(owned&& moved) noexcept;
    owned& operator=(owned&& moved) noexcept;

    owned(const owned& copied);
    owned& operator=(const owned& copied);

    ///////////
    // Keep. //
    ///////////

  protected:
    T* _owner;

    void attach(T* owner);
    void detach();
  };

} // namespace hatch

#endif // HATCH_OWNED_HH