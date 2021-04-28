#ifndef HATCH_LIST_HH
#define HATCH_LIST_HH

#include <hatch/utility/list_fwd.hh>

namespace hatch {

  template <class T>
  class list final {
  public:
    friend class list_iterator<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  private:
    list(list_node<T>* head);

  public:
    list();
    ~list();

    list(list&& moved) noexcept;
    list& operator=(list&& moved) noexcept;

    list(const list&) = delete;
    list& operator=(const list&) = delete;

    ////////////////
    // Iterators. //
    ////////////////

  public:
    list_iterator<T> begin();
    const list_iterator<T> begin() const;

    list_iterator<T> end();
    const list_iterator<T> end() const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    list_node<T>* _head;

    //////////////////////////
    // Structure: accessors //
    //////////////////////////

  public:
    bool empty() const;
    T* front() const;
    T* back() const;

    /////////////////////////
    // Structure: mutators //
    /////////////////////////

  public:
    T* pop_front();
    void push_front(list_node<T>& node);
    void push_front(list<T>& root);

    T* pop_back();
    void push_back(list_node<T>& node);
    void push_back(list<T>& list);
  };

} // namespace hatch

#include <hatch/utility/list_node.hh>
#include <hatch/utility/list_iterator.hh>

#include <hatch/utility/list_impl.hh>
#include <hatch/utility/list_node_impl.hh>
#include <hatch/utility/list_iterator_impl.hh>

#endif // HATCH_LIST_HH
