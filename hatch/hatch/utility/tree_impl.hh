#ifndef HATCH_TREE_IMPL_HH
#define HATCH_TREE_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_impl.hh directly. include tree.hh instead."
#endif

#include <algorithm>

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template<class T>
  tree<T>::tree(tree_node<T>* root) :
      _root{root} {
  }

  template<class T>
  tree<T>::tree() :
    _root{nullptr} {
  }

  template<class T>
  tree<T>::~tree() {
    while(_root) {
      auto* min = _root->minimum();
      auto* max = _root->maximum();
      if (min != _root) {
        min->remove();
      } else if (max != _root) {
        max->remove();
      } else {
        _root = nullptr;
      }
    }
  }

  template<class T>
  tree<T>::tree(tree<T>&& moved) noexcept :
      _root{moved.root} {
    moved._root = nullptr;
  }

  template<class T>
  tree<T>& tree<T>::operator=(tree<T>&& moved) noexcept {
    _root = moved.root;
    moved._root = nullptr;
  }

  ////////////////
  // Iterators. //
  ////////////////

  template <class T>
  tree_iterator<T> tree<T>::begin() {
    return {this, _root ? minimum() : tree_iterator<T>::_after};
  }

  template <class T>
  const tree_iterator<T> tree<T>::begin() const {
    return {this, _root ? minimum() : tree_iterator<T>::_after};
  }

  template <class T>
  tree_iterator<T> tree<T>::end() {
    return {this, tree_iterator<T>::_after};
  }

  template <class T>
  const tree_iterator<T> tree<T>::end() const {
    return {this, tree_iterator<T>::_after};
  }

  //////////////////////////
  // Structure: accessors //
  //////////////////////////

  template<class T>
  bool tree<T>::empty() const {
    return _root == nullptr;
  }

  template<class T>
  T* tree<T>::root() const {
    return _root ? &_root->get() : nullptr;
  }

  template<class T>
  T* tree<T>::minimum() const {
    auto* current = _root;
    if (current) {
      while (current->prev()) {
        current = current->prev();
      }
    }
    return current;
  }

  template<class T>
  T* tree<T>::maximum() const {
    auto* current = _root;
    if (current) {
      while (current->next()) {
        current = current->next();
      }
    }
    return current;
  }

  /////////////////////////
  // Structure: mutators //
  /////////////////////////

  template<class T>
  tree_iterator<T> tree<T>::insert(tree_node<T>& node) {
    if (_root) {
      _root->insert(node);
    } else {
      _root = &node;
    }
    _root = _root->root();
    return {this, &node};
  }
}

#endif