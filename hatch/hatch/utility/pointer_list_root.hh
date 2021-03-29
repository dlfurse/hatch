#ifndef HATCH_POINTER_LIST_ROOT_HH
#define HATCH_POINTER_LIST_ROOT_HH

#ifndef HATCH_POINTER_LIST_HH
#error "do not include pointer_list_root.hh directly. include pointer_list.hh instead."
#endif

namespace hatch {

  template <class T>
  class pointer_list_root final {
  public:
    friend class pointer_list_iterator<T>;
    static_assert(std::is_base_of_v<pointer_list_node<T>, T>);

  private:
    pointer_list_node<T>* _head;

  public:
    pointer_list_root();
    ~pointer_list_root();

    bool empty() const;

    T* front() const;
    T* pop_front();

    T* back() const;
    T* pop_back();

    pointer_list_iterator<T> begin();
    pointer_list_iterator<T> end();

  public:
    T* replace_front(pointer_list_node<T>& node);
    T* replace_front(pointer_list_iterator<T>& iterator);

    void push_front(pointer_list_node<T>& node);
    void push_front(pointer_list_iterator<T>& iterator);

    void splice_front(pointer_list_node<T>& node);
    void splice_front(pointer_list_root<T>& root);

  public:

    T* replace_back(pointer_list_node<T>& node);
    T* replace_back(pointer_list_iterator<T>& iterator);

    void push_back(pointer_list_node<T>& node);
    void push_back(pointer_list_iterator<T>& iterator);

    void splice_back(pointer_list_node<T>& node);
    void splice_back(pointer_list_root<T>& root);
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_HH
