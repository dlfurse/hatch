#ifndef HATCH_PROMISE_HH
#define HATCH_PROMISE_HH

#ifndef HATCH_ASYNCHRONOUS_HH
#error "do not include promise.hh directly. include asynchronous.hh instead."
#endif

namespace hatch {

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
     * Promises are unique, writable interfaces which represent a value which has not yet been
     * computed. Promises may not be copied, only moved.
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
     * promise variable, the orignal promise will be in the moved state.  When the promise is
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

    std::enable_if_t<complex, void> complete(const stored& data);
    void complete(const T&... data);
    void fail(const std::exception_ptr& excp);

  private:
    std::unordered_set<future<T...>*> _futures;

  public:
    future<T...> awaited();

    /**
     * Continuations and Recoveries.
     *
     * Promises can be linked to other, dependent values.
     */

  private:
    class continuation {
    public:
      virtual std::enable_if_t<complex, void> complete(const stored& data) = 0;
      virtual void complete(const T&... data) = 0;
      virtual void fail(const std::exception_ptr& excp) = 0;
      virtual ~continuation() = default;
    };

    template <class F, class P = mapped_promise<F, T...>>
    class continued final : public continuation {
      public:
      std::enable_if_t<complex, void> complete(const stored& data) override;
      void complete(const T&... data) override ;
      void fail(const std::exception_ptr& excp) override;
      continued(F&& function, P&& promise);

      private:
      F _function;
      P _promise;
    };

    std::list<std::unique_ptr<continuation>> _continuations;

    class recovery {
    public:
      virtual void handle(const std::exception_ptr& excp, promise& p) = 0;
      virtual ~recovery() = default;
    };

    template <class F>
    class recovered final : public recovery {
    public:
      void handle(const std::exception_ptr& excp, promise& p) override;
      explicit recovered(F&& function);

    private:
      F _function;
    };

    std::unique_ptr<recovery> _recovery;

  public:
    template <class F>
    mapped_future<F, T...> then(F&& function);

    template <class F>
    future<T...> recover(F&& function);
  };

} // namespace hatch

#endif // HATCH_PROMISE_HH
