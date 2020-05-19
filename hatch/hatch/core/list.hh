#ifndef HATCH_LIST_HH
#define HATCH_LIST_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include list.hh directly. include structures.hh instead."
#endif

#include <type_traits> // std::is_base_of_v

namespace hatch {

  template <class T>
  class list;

  template <class T>
  class list_node;

  template <class T>
  class list_base {
  public:
    friend class list<T>;
    friend class list_node<T>;

  public:
    list_base();
    ~list_base() = default;

    bool operator!=(const list_base& compared) const;

    list_base& operator++();
    const list_base& operator++() const;

    list_base& operator--();
    const list_base& operator--() const;

    T& operator*();
    const T& operator*() const;

  private:
    list_base* _prev;
    list_base* _next;
  };

  template <class T>
  class list_node : public list_base<T> {
  protected:
    list_node() = default;

  public:
    friend class list<T>;
    ~list_node();

    void detach();
    void detach_and_exchange_with(list_node& node);
    void detach_and_insert_after(list_node& prev);
    void detach_and_insert_before(list_node& next);
  };

  template <class T>
  class list : public list_base<T> {
  public:
    static_assert(std::is_base_of_v<list_node<T>, T>);
    list();
    ~list();

    list(list&& moved) noexcept;
    list& operator=(list&& moved) noexcept;

    bool empty() const;
    void clear();

    list_base<T>& begin();
    const list_base<T>& begin() const;

    list_base<T>& end();
    const list_base<T>& end() const;

    T* get_front() const;
    T* pop_front();
    void push_front(T& node);

    T* get_back() const;
    T* pop_back();
    void push_back(T& node);
  };

} // namespace hatch

#endif // HATCH_LIST_HH
