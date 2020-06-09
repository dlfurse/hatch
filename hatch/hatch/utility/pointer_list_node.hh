#ifndef HATCH_POINTER_LIST_NODE_HH
#define HATCH_POINTER_LIST_NODE_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_node.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  class pointer_list_node {
  protected:
    pointer_list_node* _prev;
    pointer_list_node* _next;

  protected:
    pointer_list_node();
    ~pointer_list_node();

  public:
    friend class pointer_list_iterator<T>;

    pointer_list_iterator<T> begin();
    pointer_list_iterator<T> end();

    bool detached() const;
    bool detach();

    void insert_replacing(pointer_list_node& item);
    void splice_replacing(pointer_list_node& list);

    void insert_before(pointer_list_node& next);
    void splice_before(pointer_list_node& next);

    void insert_after(pointer_list_node& prev);
    void splice_after(pointer_list_node& prev);
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_NODE_HH
