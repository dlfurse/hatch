#ifndef HATCH_KEPT_HH
#define HATCH_KEPT_HH

#ifndef HATCH_KEEP_HH
#error "do not include kept.hh directly. include keep.hh instead."
#endif

#include <hatch/utility/chain.hh>

namespace hatch {

  template <class T, class U>
  class kept : public chain<U> {
  public:
    friend class keeper<T, U>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    kept(T* keeper = nullptr);
    ~kept();

    kept(kept&& moved) noexcept;
    kept& operator=(kept&& moved) noexcept;

    kept(const kept& copied);
    kept& operator=(const kept& copied);

    ///////////
    // Keep. //
    ///////////

  protected:
    T* _keeper;

    void attach(T* keeper);
    void detach();
  };

} // namespace hatch

#endif // HATCH_KEPT_HH