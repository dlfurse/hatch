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
      container<T>{std::forward<Args>(args)...},
      chain<list_node<T>>{} {
  }

  template <class T>
  list_node<T>::~list_node() {
  }

  template <class T>
  list_node<T>::list_node(list_node&& moved) noexcept :
      container<T>{std::move(moved)},
      chain<list_node<T>>{std::move(moved)} {
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(list_node&& moved) noexcept {
    container<T>::operator=(std::move(moved));
    chain<list_node<T>>::operator=(std::move(moved));
    return *this;
  }

  template <class T>
  list_node<T>::list_node(const list_node& copied) :
      container<T>{copied},
      chain<list_node<T>>{} {
  }

  template <class T>
  list_node<T>& list_node<T>::operator=(const list_node& copied) {
    container<T>::operator=(copied);
    return *this;
  }

} // namespace hatch

#endif // HATCH_LIST_NODE_IMPL_HH
