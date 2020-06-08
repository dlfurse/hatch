#ifndef HATCH_POINTER_LIST_HH
#define HATCH_POINTER_LIST_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_list.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  class pointer_list {
  protected:
    pointer_list* _prev;
    pointer_list* _next;

  protected:
    pointer_list();
    ~pointer_list();

  public:
    friend class pointer_list_iterator<T>;

    pointer_list_iterator<T> begin();
    pointer_list_iterator<T> end();

    bool detached() const;

    bool detach();

    void insert_replacing(pointer_list& item);
    void splice_replacing(pointer_list& list);

    void insert_before(pointer_list& next);
    void splice_before(pointer_list& next);

    void insert_after(pointer_list& prev);
    void splice_after(pointer_list& prev);
  };

}

#endif // HATCH_POINTER_LIST_HH
