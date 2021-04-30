#ifndef HATCH_LIST_NODE_IMPL_HH
#define HATCH_LIST_NODE_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_node_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T>
  template <class ...Args>
  list_node<T>::list_node(Args&&... args) :
      chain<list_node<T>>{},
      container<T>{std::forward<Args>(args)...} {
  }

  template <class T>
  list_node<T>::~list_node() {
  }

  template <class T>
  list_node<T>::list_node(list_node&& moved) noexcept :
      chain<list_node<T>>{std::move(moved)},
      container<T>{std::move(moved)} {
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(list_node&& moved) noexcept {
    chain<list_node<T>>::operator=(std::move(moved));
    container<T>::operator=(std::move(moved));
    return *this;
  }

  template <class T>
  list_node<T>::list_node(const list_node& copied) :
      chain<list_node<T>>{},
      container<T>{copied} {
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(const list_node& copied) {
    container<T>::operator=(copied);
    return *this;
  }

} // namespace hatch

#endif // HATCH_LIST_NODE_IMPL_HH
