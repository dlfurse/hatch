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

    pointer_list_root(pointer_list_root&& moved);
    pointer_list_root& operator=(pointer_list_root&& moved);

    pointer_list_root(const pointer_list_root&) = delete;
    pointer_list_root& operator=(const pointer_list_root&) = delete;

    bool empty() const;

  public:
    T* front();
    const T* front() const;
    pointer_list_iterator<T> begin();

    T* pop_front();
    void push_front(pointer_list_node<T>& node);
    void push_front(pointer_list_root<T>& root);

  public:
    T* back();
    const T* back() const;
    pointer_list_iterator<T> end();

    T* pop_back();
    void push_back(pointer_list_node<T>& node);
    void push_back(pointer_list_root<T>& root);
  };

} // namespace hatch

#endif // HATCH_POINTER_LIST_ROOT_HH
