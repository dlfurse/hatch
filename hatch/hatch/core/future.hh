#ifndef HATCH_FUTURE_HH
#define HATCH_FUTURE_HH

#ifndef HATCH_ASYNC_HH
#error "do not include future.hh directly.  include async.hh instead."
#endif

#include <cassert> // std::assert

#include <exception> // std::exception_ptr
#include <tuple> // std::apply, std::tuple, std::tuple_element_t
#include <type_traits> // std::conditional_t, std::enable_if_t
#include <utility> // std::forward, std::move

namespace hatch {

  template <class ...T>
  class future {
    friend class promise<T...>;

    static constexpr bool simple = sizeof...(T) == 1;
    static constexpr bool complex = sizeof...(T) > 1;

    using reduced = flatwrapped<std::tuple, T...>;
    using stored = std::conditional_t<simple, std::tuple_element_t<0, reduced>, reduced>;

    /**
     * Construction.
     *
     * Futures may be both copied and moved. They are read-only handles to values backed by
     * promises.
     */

  public:
    ~future();
    future(promise<T...>* promise = nullptr);

    future(const future& copied);
    future& operator=(const future& copied);

    future(future&& moved) noexcept;
    future& operator=(future&& moved) noexcept;

    template <class S, class = std::enable_if_t<complex && std::is_same_v<S, stored>>>
    future(const S& data);
    future(const T&... data);
    future(const std::exception_ptr& excp);

  private:
    promise<T...>* _promise;

  public:
    template<class F>
    mapped_future<F, T...> then(F&& function);

    template <class F>
    future<T...> recover(F&& function);

  private:
    template <class F, class ...A>
    static std::enable_if_t<simple, std::result_of_t<F(T...)>> apply(F&& function, A&&... arguments) {
      return function(arguments...);
    }

    template <class F, class ...A>
    static std::enable_if_t<complex, std::result_of_t<F(T...)>> apply(F&& function, A&&... arguments) {
      return std::apply(function, arguments...);
    }

    enum class state {
      moved = 0,
      pending = 1,
      completed = 2,
      failed = 3,
    } _state;

    union storage {
      storage() {};
      ~storage() {};

      template <class ...U>
      explicit storage(const U&... values) : _value{values...} {}

      template <class ...U>
      explicit storage(U&&... values) : _value{std::forward<U>(values)...} {}

      explicit storage(const stored& value) : _value{value} {}
      explicit storage(stored&& value) : _value{std::move(value)} {}

      stored _value;
      std::exception_ptr _exception;
    } _storage;

    void complete(const T&... data);
    void fail(const std::exception_ptr& excp);

  public:
    bool is_moved() const;
    bool is_pending() const;
    bool is_finished() const;
    bool is_completed() const;
    bool is_failed() const;

    const stored& get() const&;
    stored&& get() &&;

    const stored& value() const&;
    stored&& value() &&;

    std::exception_ptr exception() const;
  };

} // namespace hatch

#endif // HATCH_FUTURE_HH