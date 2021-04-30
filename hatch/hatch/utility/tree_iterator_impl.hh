#ifndef HATCH_TREE_ITERATOR_IMPL_HH
#define HATCH_TREE_ITERATOR_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_iterator_impl.hh directly. include tree.hh instead."
#endif

namespace hatch {

  template<class T>
  tree_node<T>* tree_iterator<T>::_before =
      const_cast<tree_node<T>*>(reinterpret_cast<const tree_node<T>*>("tree_iterator::before"));

  template<class T>
  tree_node<T>* tree_iterator<T>::_after =
      const_cast<tree_node<T>*>(reinterpret_cast<const tree_node<T>*>("tree_iterator::after"));

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template<class T>
  tree_iterator<T>::tree_iterator(tree<T>* owner, tree_node<T>* node) :
      kept<tree<T>, tree_iterator<T>>::kept{owner},
      _node{node} {
  }

  template<class T>
  tree_iterator<T>::tree_iterator() :
      kept<tree<T>, tree_iterator<T>>::kept{},
    _node{nullptr} {
  }

  template<class T>
  tree_iterator<T>::~tree_iterator() {
  }

  template<class T>
  tree_iterator<T>::tree_iterator(tree_iterator&& moved) noexcept :
      kept<tree<T>, tree_iterator<T>>::kept{std::move(moved)},
      _node{moved._node} {
    moved._node = nullptr;
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator=(tree_iterator&& moved) noexcept {
    kept<tree<T>, tree_iterator<T>>::operator=(std::move(moved));
    _node = moved._node;
    moved._node = nullptr;
    return *this;
  }

  template<class T>
  tree_iterator<T>::tree_iterator(const tree_iterator& copied) :
      kept<tree<T>, tree_iterator<T>>::kept{copied},
      _node{copied._node} {
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator=(const tree_iterator& copied) {
    kept<tree<T>, tree_iterator<T>>::operator=(copied);
    _node = copied._node;
    return *this;
  }

  //////////////////
  // Comparisons. //
  //////////////////

  template <class T>
  tree_iterator<T>::operator bool() const {
    return this->_keeper;
  }

  template<class T>
  bool tree_iterator<T>::operator==(const tree_iterator& compared) const {
    return this->_keeper == compared._keeper && _node == compared._node;
  }

  template<class T>
  bool tree_iterator<T>::operator!=(const tree_iterator& compared) const {
    return this->_keeper != compared._keeper || _node != compared._node;
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
  tree_iterator<T>& tree_iterator<T>::operator++() {
    if (auto* tree = this->_keeper) {
      if (_node == _before) {
        if (tree->_root == nullptr) {
          _node = _after;
        } else {
          _node = tree->minimum();
        }
      } else if (_node != _after) {
        _node = _node->successor();
        if (_node == nullptr) {
          _node = _after;
        }
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator<T>& tree_iterator<T>::operator++() const {
    return const_cast<tree_iterator<T>*>(this)->operator++();
  }

  template<class T>
  const tree_iterator<T> tree_iterator<T>::operator++(int) const {
    auto* const node = _node;
    this->operator++();
    return {this->_keeper, node};
  }

  template<class T>
  tree_iterator<T>& tree_iterator<T>::operator--() {
    if (auto* tree = this->_keeper) {
      if (_node == _after) {
        if (tree->_root == nullptr) {
          _node = _before;
        } else {
          _node = tree->maximum();
        }
      } else if (_node != _before) {
        _node = _node->predecessor();
        if (_node == nullptr) {
          _node = _before;
        }
      }
    }
    return *this;
  }

  template<class T>
  const tree_iterator<T>& tree_iterator<T>::operator--() const {
    return const_cast<tree_iterator<T>*>(this)->operator--();
  }

  template<class T>
  const tree_iterator<T> tree_iterator<T>::operator--(int) const {
    auto* const node = _node;
    this->operator--();
    return {this->_keeper, node};
  }

  ////////////////////////////////////////
  // Structure: mutate underlying tree. //
  ////////////////////////////////////////

  template <class T>
  tree<T> tree_iterator<T>::remove() {
    if (auto* tree = this->_keeper) {
      if (_node) {
        auto* other = std::max(_node->predecessor(), _node->successor());

        _node->remove();

        if (other) {
          tree->_root = other->root();
        } else {
          tree->_root = nullptr;
        }
        tree->release();

        return ::hatch::tree<T>{_node};
      }
    }
    return ::hatch::tree<T>{nullptr};
  }

} // namespace hatch

#endif // HATCH_TREE_ITERATOR_IMPL_HH