#ifndef HATCH_POINTER_LIST_NODE_HH
#define HATCH_POINTER_LIST_NODE_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_node.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  class pointer_list_node {
  public:
    friend class pointer_list_iterator<T>;
    friend class pointer_list_root<T>;

  private:
    pointer_list_node* _prev;
    pointer_list_node* _next;

  protected:
    pointer_list_node();
    ~pointer_list_node();

  public:
    T& data();
    const T& data() const;

    bool detached() const;

    pointer_list_node* prev();
    const pointer_list_node* prev() const;

    pointer_list_node* next();
    const pointer_list_node* next() const;

  protected:
    pointer_list_node<T>& detach();

    void splice_before(pointer_list_node& node);
    void splice_replacing(pointer_list_node& node);
    void splice_after(pointer_list_node& node);
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_NODE_HH
