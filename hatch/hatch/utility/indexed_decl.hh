#ifndef HATCH_INDEXED_DECL_HH
#define HATCH_INDEXED_DECL_HH

#ifndef HATCH_INDEXED_HH
#error "do not include indexed_decl.hh directly. include indexed.hh instead."
#endif

#include <hatch/utility/integral.hh>

#include <cstddef> // std::byte

namespace hatch {

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  class indexed {
  public:
    static constexpr auto null = unsigned_max_v<Width>;

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
    using index = unsigned_t<Width>;
    index _index;

    static __thread std::byte* _context;
  };

} // namespace hatch

#endif // HATCH_INDEXED_DECL_HH