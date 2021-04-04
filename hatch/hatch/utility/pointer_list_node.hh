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

  protected:
    pointer_list_node();
    ~pointer_list_node();

  public:
    pointer_list_node(pointer_list_node&&) = delete;
    pointer_list_node& operator=(pointer_list_node&&) = delete;

    pointer_list_node(const pointer_list_node&) = delete;
    pointer_list_node& operator=(const pointer_list_node&) = delete;

  private:
    pointer_list_node* _prev;
    pointer_list_node* _next;

  protected:
    void splice(pointer_list_node& node);

  public:
    T& get() const;
    [[nodiscard]] bool alone() const;

    pointer_list_node& prev();
    const pointer_list_node& prev() const;

    pointer_list_node& next();
    const pointer_list_node& next() const;
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_NODE_HH
