#ifndef HATCH_POINTED_HH
#define HATCH_POINTED_HH

#include <cstddef>

namespace hatch {

  template <class T = void>
  class pointed {
  public:
    using datatype = void;

  public:
    pointed();
    ~pointed();

    pointed(pointed&& moved) noexcept;
    pointed& operator=(pointed&& moved);

    pointed(const pointed& copied);
    pointed& operator=(const pointed& copied);

    pointed(T* address);
    pointed& operator=(T* address);

  public:
    operator bool() const;

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

  private:
    T* _pointer;
  };

} // namespace hatch

#include <hatch/utility/pointed_impl.hh>

#endif // HATCH_POINTED_HH