#ifndef HATCH_CHAIN_HH
#define HATCH_CHAIN_HH

namespace hatch {

  template <class T>
  class chain {
  public:

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  protected:
    chain();
    ~chain();

    chain(T&& moved) noexcept;
    chain& operator=(T&& moved) noexcept;

    chain(const T&) = delete;
    chain& operator=(const T&) = delete;

  ////////////
  // Chain. //
  ////////////

  private:
    T* _prev;
    T* _next;

  public:
    bool alone() const;

    T& prev();
    const T& prev() const;

    T& next();
    const T& next() const;

  protected:
    void splice(T& node);

    template <class U>
    void foreach(U&& callable);

    template <class U>
    void foreach(U&& callable) const;
};

} // namespace hatch

#include <hatch/utility/chain_impl.hh>

#endif // HATCH_CHAIN_HH
