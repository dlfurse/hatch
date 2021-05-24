#ifndef HATCH_INDEXED_HH
#define HATCH_INDEXED_HH

#include <hatch/utility/integral.hh>

#include <cstddef>

namespace hatch {

  template <class T, widths Width, nosignint<Width> Stride = sizeof(T), nosignint<Width> Offset = 0>
  class indexed {
  private:
    using index = nosignint<Width>;
    static constexpr auto null = nosignmax<Width>;

  public:
    class context;

  public:
    indexed();
    ~indexed();

    indexed(indexed&& moved) noexcept;
    indexed& operator=(indexed&& moved);

    indexed(const indexed& copied);
    indexed& operator=(const indexed& copied);

    indexed(T* address);
    indexed& operator=(T* address);

  public:
    operator bool() const;

    T& operator*();
    const T& operator*() const;

    T* operator->();
    const T* operator->() const;

  private:
    index _index;
    static __thread std::byte* _context;
  };

}

#include <hatch/utility/indexed_impl.hh>

#endif // HATCH_INDEXED_HH
