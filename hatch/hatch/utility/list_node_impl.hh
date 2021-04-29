#ifndef HATCH_LIST_NODE_IMPL_HH
#define HATCH_LIST_NODE_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_node_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  template <class T>
  list_node<T>::list_node() {
  }

  template <class T>
  list_node<T>::~list_node() {
  }

  template <class T>
  list_node<T>::list_node(list_node&& moved) noexcept :
      container<T>{moved},
      keeper<list_node<T>, list_iterator<T>>{moved} {
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(list_node&& moved) noexcept {
    container<T>::operator=(moved);
    keeper<list_node<T>, list_iterator<T>>::operator=(moved);
    return *this;
  }

} // namespace hatch

#endif // HATCH_LIST_NODE_IMPL_HH
