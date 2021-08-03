#ifndef HATCH_POINTED_HH
#define HATCH_POINTED_HH

namespace hatch {

  template <class T>
  class pointed {
  public:
    pointed();
    ~pointed();

    template<class U>
    explicit pointed(U* address);
    template<class U>
    pointed& operator=(U* address);

    pointed(pointed&& moved) noexcept;
    pointed& operator=(pointed&& moved) noexcept;

    pointed(const pointed& copied);
    pointed& operator=(const pointed& copied);

  public:
    operator bool() const;

    bool operator==(const pointed& other) const;
    bool operator!=(const pointed& other) const;

    T* operator()();
    const T* operator()() const;

    T* operator->();
    const T* operator->() const;

    T& operator*();
    const T& operator*() const;

  private:
    T* _pointer;
  };

} // namespace hatch

#include <hatch/utility/pointed_impl.hh>

#endif // HATCH_POINTED_HH