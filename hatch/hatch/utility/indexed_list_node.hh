#ifndef HATCH_INDEXED_LIST_NODE_HH
#define HATCH_INDEXED_LIST_NODE_HH

#ifndef HATCH_INDEXED_LIST_HH
#error "do not include indexed_list_node.hh directly. include indexed_list.hh instead."
#endif

#include <cstdint>

namespace hatch {

  template <class T>
  class indexed_list_node {
  protected:
    int64_t _prev;
    int64_t _next;

  protected:
    indexed_list_node();
    ~indexed_list_node();

  public:
    friend class indexed_list_iterator<T>;

    indexed_list_iterator<T> begin(T* data);
    indexed_list_iterator<T> end(T* data);

    bool detached() const;
    bool detach(T* data);

    void insert_replacing(T* data, indexed_list_node& item);
    void splice_replacing(T* data, indexed_list_node& item);

    void insert_before(T* data, indexed_list_node& item);
    void splice_before(T* data, indexed_list_node& item);

    void insert_after(T* data, indexed_list_node& item);
    void splice_after(T* data, indexed_list_node& item);
  };

}

#endif // HATCH_INDEXED_LIST_NODE_HH
