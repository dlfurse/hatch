#ifndef HATCH_INDEXED_HH
#define HATCH_INDEXED_HH

#include <hatch/utility/integral.hh>

#include <cstddef> // std::byte

namespace hatch {

  template <class T, widths Width, nosignint<Width> Stride = sizeof(T), nosignint<Width> Offset = 0>
  class indexed {
  public:
    class context {
    public:
      context() = delete;

      context(context&&) = delete;
      context& operator=(context&&) = delete;

      context(const context&) = delete;
      context& operator=(const context&) = delete;

    public:
      template <class U>
      explicit context(U* address);

      ~context();
    };

  public:
    static constexpr auto null = nosignmax<Width>;

  public:
    indexed();
    ~indexed();

    indexed(indexed&& moved) noexcept;
    indexed& operator=(indexed&& moved) noexcept;

    indexed(const indexed& copied);
    indexed& operator=(const indexed& copied);

    template <class U>
    explicit indexed(U* address);
    template <class U>
    indexed& operator=(U* address);

  public:
    operator bool() const;

    bool operator==(const indexed& other) const;
    bool operator!=(const indexed& other) const;

    T* operator()();
    const T* operator()() const;

    T* operator->();
    const T* operator->() const;

    T& operator*();
    const T& operator*() const;

  private:
    using index = nosignint<Width>;
    index _index;

    static __thread std::byte* _context;
  };

} // namespace hatch

#include <hatch/utility/indexed_impl.hh>

#endif // HATCH_INDEXED_HH
