#ifndef HATCH_LIST_ITERATOR_HH
#define HATCH_LIST_ITERATOR_HH

#ifndef HATCH_LIST_HH
#error "do not include list_iterator.hh directly. include list.hh instead."
#endif

#include <type_traits>

namespace hatch {

  template <class T>
  class list_iterator {
  public:
    friend class list<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  private:
    list_iterator(list<T>* list, list_node<T>* node);

  public:
    list_iterator() = delete;
    ~list_iterator() = default;

    list_iterator(list_iterator&& moved) = delete;
    list_iterator& operator=(list_iterator&& moved) = delete;

    list_iterator(const list_iterator& copied);
    list_iterator& operator=(const list_iterator& copied);

    //////////////////
    // Comparisons. //
    //////////////////

  public:
    bool operator==(const list_iterator& compared) const;
    bool operator!=(const list_iterator& compared) const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    list<T>* _list;
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
    list_iterator insert(list<T>& list);
    list<T> remove(list_iterator& end);
  };

} // namespace hatch

#endif // HATCH_LIST_ITERATOR_HH
