#ifndef HATCH_LIST_ITERATOR_HH
#define HATCH_LIST_ITERATOR_HH

#ifndef HATCH_LIST_HH
#error "do not include list_iterator.hh directly. include list.hh instead."
#endif

#include <hatch/utility/keep.hh>

namespace hatch {

  template <class T>
  class list_iterator final : public kept<list<T>, list_iterator<T>> {
  public:
    friend class list<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  private:
    list_iterator(list<T>* list, list_node<T>* owner);

  public:
    list_iterator();
    ~list_iterator();

    list_iterator(list_iterator&& moved) noexcept;
    list_iterator& operator=(list_iterator&& moved) noexcept;

    list_iterator(const list_iterator& copied);
    list_iterator& operator=(const list_iterator& copied);

    //////////////////
    // Comparisons. //
    //////////////////

  public:
    operator bool() const;
    bool operator==(const list_iterator& compared) const;
    bool operator!=(const list_iterator& compared) const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    mutable list_node<T>* _node;
    static list_node<T>* _before;
    static list_node<T>* _after;

    /////////////////////////////////////
    // Structure: get underlying data. //
    /////////////////////////////////////

  public:
    T& operator*() const;
    T* operator->() const;

    ///////////////////////////////
    // Structure: move iterator. //
    ///////////////////////////////

  public:
    list_iterator& operator++();
    const list_iterator& operator++() const;
    const list_iterator operator++(int) const;

    list_iterator& operator--();
    const list_iterator& operator--() const;
    const list_iterator operator--(int) const;

    ////////////////////////////////////////
    // Structure: mutate underlying list. //
    ////////////////////////////////////////

  public:
    list_iterator insert(list<T>& other);
    list<T> remove(list_iterator& other);
  };

} // namespace hatch

#endif // HATCH_LIST_ITERATOR_HH
