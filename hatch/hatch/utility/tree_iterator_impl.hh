#ifndef HATCH_TREE_ITERATOR_IMPL_HH
#define HATCH_TREE_ITERATOR_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_iterator_impl.hh directly. include tree.hh instead."
#endif

namespace hatch {

  template<class T>
  tree_node <T>* tree_iterator<T>::_before =
      const_cast<tree_node <T>*>(reinterpret_cast<const tree_node <T>*>("tree_iterator::before"));

  template<class T>
  tree_node <T>* tree_iterator<T>::_after =
      const_cast<tree_node <T>*>(reinterpret_cast<const tree_node <T>*>("tree_iterator::after"));

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template<class T>
  tree_iterator<T>::tree_iterator(tree <T>* tree, tree_node <T>* node) :
      _tree{tree},
      _node{node} {
  }

  template<class T>
  tree_iterator<T>::tree_iterator(const tree_iterator& copied) :
      _tree{copied._tree},
      _node{copied._node} {
  }

  template<class T>
  tree_iterator <T>& tree_iterator<T>::operator=(const tree_iterator& copied) {
    _tree = copied._tree;
    _node = copied._node;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template<class T>
  bool tree_iterator<T>::operator==(const tree_iterator& compared) const {
    return _node == compared._node;
  }

  template<class T>
  bool tree_iterator<T>::operator!=(const tree_iterator& compared) const {
    return _node != compared._node;
  }

  /////////////////////////////////////
  // Structure: get underlying data. //
  /////////////////////////////////////

  template<class T>
  T& tree_iterator<T>::operator*() const {
    return _node->get();
  }

  template<class T>
  T* tree_iterator<T>::operator->() const {
    return &_node->get();
  }

  ///////////////////////////////
  // Structure: move iterator. //
  ///////////////////////////////

  template<class T>
  tree_iterator <T>& tree_iterator<T>::operator++() {
    if (_node == _before) {
      if (_tree->_root == nullptr) {
        _node = _after;
      }
      _node = _tree->minimum();
    } else if (_node != _after) {
      _node = _node->successor();
      if (_node == nullptr) {
        _node = _after;
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator <T>& tree_iterator<T>::operator++() const {
    return const_cast<tree_iterator <T>*>(this)->operator++();
  }

  template<class T>
  const tree_iterator <T> tree_iterator<T>::operator++(int) const {
    auto* const node = _node;
    this->operator++();
    return {_tree, node};
  }

  template<class T>
  tree_iterator <T>& tree_iterator<T>::operator--() {
    if (_node == _after) {
      if (_tree->_root == nullptr) {
        _node = _before;
      }
      _node = _tree->maximum();
    } else if (_node != _before) {
      _node = _node->predecessor();
      if (_node == nullptr) {
        _node = _before;
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator <T>& tree_iterator<T>::operator--() const {
    return const_cast<tree_iterator <T>*>(this)->operator--();
  }

  template<class T>
  const tree_iterator <T> tree_iterator<T>::operator--(int) const {
    auto* const node = _node;
    this->operator--();
    return {_tree, node};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying tree. //
  ////////////////////////////////////////



} // namespace hatch

#endif // HATCH_TREE_ITERATOR_IMPL_HH