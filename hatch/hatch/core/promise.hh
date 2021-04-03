#ifndef HATCH_PROMISE_HH
#define HATCH_PROMISE_HH

#ifndef HATCH_ASYNC_HH
#error "do not include promise.hh directly. include async.hh instead."
#endif

#include <hatch/utility/pointer_list.hh> // pointer_list_node<T>

#include <exception> // std::exception_ptr
#include <list> // std::list
#include <memory> // std::unique_ptr
#include <tuple> // std::tuple, std::tuple_element_t
#include <type_traits> // std::conditional_t, std::enable_if_t
#include <unordered_set> // std::unordered_set

#include <cassert> // assert

namespace hatch {

  /**
   * Promise.
   *
   * Promises are unique, writable interfaces which represent a value that has not yet been
   * computed.  Futures are the corresponding readable interfaces.
   */

  template <class ...T>
  class promise {
    friend class future<T...>;

    static constexpr bool simple = sizeof...(T) == 1;
    static constexpr bool complex = sizeof...(T) > 1;

    using reduced = flatwrapped<std::tuple, T...>;
    using stored = std::conditional_t<simple, std::tuple_element_t<0, reduced>, reduced>;

    /**
     * Construction.
     *
     * Promises may not be copied, only moved.
     */

  public:
    promise();
    ~promise();

    promise(const promise& copied) = delete;
    promise& operator=(const promise& copied) = delete;

    promise(promise&& moved) noexcept;
    promise& operator=(promise&& moved) noexcept;

    /**
     * State.
     *
     * Upon construction, a promise is in the pending state.  If a promise is moved to another
     * promise variable, the original promise will be in the moved state.  When the promise is
     * completed it moves to the completed state, and if it's failed, it moves to the failed state.
     * No other transitions are possible.
     */

  private:
    enum class state {
      moved = 0,
      pending = 1,
      completed = 2,
      failed = 3,
    } _state;

  public:
    bool is_moved() const;
    bool is_pending() const;
    bool is_finished() const;
    bool is_completed() const;
    bool is_failed() const;

    /**
     * Continuations and Recoveries.
     *
     * Promises can be linked to other, dependent values by functions.
     */

  private:
    void repossess_futures();
    void dispossess_futures();
    void discard_futures();

    pointer_list_root<future<T...>> _futures;

    class continuation {
    public:
      virtual ~continuation() = default;

      virtual void complete(const T&... data) = 0;
      virtual void fail(const std::exception_ptr& excp) = 0;
    };

    template <class F, class P = mapped_promise<F, T...>>
    class continued final : public continuation {
    public:
      explicit continued(F&& function, P&& promise);

      void complete(const T&... data) override ;
      void fail(const std::exception_ptr& excp) override;

    private:
      F _function;
      P _promise;
    };

    std::list<std::unique_ptr<continuation>> _continuations;

    class recovery {
    public:
      virtual ~recovery() = default;

      virtual void handle(const std::exception_ptr& excp, promise& p) = 0;
    };

    template <class F>
    class recovered final : public recovery {
    public:
      explicit recovered(F&& function);

      void handle(const std::exception_ptr& excp, promise& p) override;

    private:
      F _function;
    };

    std::unique_ptr<recovery> _recovery;

  public:
    template <class S, class = std::enable_if_t<complex && std::is_same_v<S, stored>>>
    void complete(const S& value);
    void complete(const T&... values);
    void fail(const std::exception_ptr& exception);

    future<T...> awaited();

    template <class F>
    mapped_future<F, T...> then(F&& function);

    template <class F>
    future<T...> recover(F&& function);
  };

} // namespace hatch

#endif // HATCH_PROMISE_HH
