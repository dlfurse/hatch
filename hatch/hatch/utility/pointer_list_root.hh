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

  private:
    pointer_list_node<T>* _head;

  public:
    pointer_list_root();
    ~pointer_list_root();

    bool empty() const;

    pointer_list_iterator<T> begin();
    pointer_list_iterator<T> end();

  public:
    T* get_front() const;
    T* pop_front();

    void push_front(pointer_list_node<T>& node);
    void push_front(pointer_list_root<T>& root);
    void push_front(pointer_list_iterator<T>& iterator);
    void push_front(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after);

    T* replace_front(pointer_list_node<T>& node);
    T* replace_front(pointer_list_root<T>& root);
    T* replace_front(pointer_list_iterator<T>& iterator);
    T* replace_front(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after);

  public:
    T* get_back() const;
    T* pop_back();

    T* replace_back(pointer_list_node<T>& node);
    T* replace_back(pointer_list_iterator<T>& iterator);
    T* replace_back(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after);

    void push_back(pointer_list_node<T>& node);
    void push_back(pointer_list_iterator<T>& iterator);
    void push_back(pointer_list_iterator<T>& start, pointer_list_iterator<T>& after);
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_HH
