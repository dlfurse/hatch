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

  template<class T, template <class> class Ref>
  tree<T, Ref>::tree(Ref<tree_node<T, Ref>> root, void* data) :
      _root{root},
      _data{data} {
  }

  template<class T, template <class> class Ref>
  tree<T, Ref>::tree() :
    _root{},
    _data{nullptr} {
  }

  template<class T, template <class> class Ref>
  tree<T, Ref>::~tree() {
    while(_root) {
      auto* min = _root->minimum(_data);
      auto* max = _root->maximum(_data);
      if (min != _root) {
        min->remove(_data);
      } else if (max != _root) {
        max->remove(_data);
      } else {
        _root = nullptr;
      }
    }
  }

  template<class T, template <class> class Ref>
  tree<T, Ref>::tree(tree&& moved) noexcept :
      owner<tree<T>, tree_iterator<T>>::owner{std::move(moved)},
      _root{moved._root},
      _data{moved._data} {
    moved._root = Ref<tree_node<T, Ref>>{};
    moved._data = nullptr;
  }

  template<class T, template <class> class Ref>
  tree<T, Ref>& tree<T, Ref>::operator=(tree&& moved) noexcept {
    owner<tree<T>, tree_iterator<T>>::operator=(std::move(moved));
    _root = moved._root;
    _data = moved._data;
    moved._root = Ref<tree_node<T, Ref>>{};
    moved._data = nullptr;
  }

  ////////////////
  // Iterators. //
  ////////////////

  template<class T, template <class> class Ref>
  tree_iterator<T, Ref> tree<T, Ref>::begin() {
    return tree_iterator<T, Ref>{this, _root ? minimum() : tree_iterator<T, Ref>::_after};
  }

  template<class T, template <class> class Ref>
  const tree_iterator<T, Ref> tree<T, Ref>::begin() const {
    return const_cast<tree<T, Ref>&>(*this).begin();
  }

  template<class T, template <class> class Ref>
  tree_iterator<T, Ref> tree<T, Ref>::end() {
    return tree_iterator<T, Ref>{this, tree_iterator<T, Ref>::_after};
  }

  template<class T, template <class> class Ref>
  const tree_iterator<T, Ref> tree<T, Ref>::end() const {
    return const_cast<tree<T, Ref>&>(*this).end();
  }

  //////////////////////////
  // Structure: accessors //
  //////////////////////////

  template<class T, template <class> class Ref>
  bool tree<T, Ref>::empty() const {
    return _root;
  }

  template<class T, template <class> class Ref>
  T* tree<T, Ref>::root() const {
    return _root ? &_root.get(_data)->get() : nullptr;
  }

  template<class T, template <class> class Ref>
  T* tree<T, Ref>::minimum() const {
    return _root ? &_root.get(_data)->minimum(_data)->get() : nullptr;
  }

  template<class T, template <class> class Ref>
  T* tree<T, Ref>::maximum() const {
    return _root ? &_root.get(_data)->maximum(_data)->get() : nullptr;
  }

  /////////////////////////
  // Structure: mutators //
  /////////////////////////

  template<class T, template <class> class Ref>
  tree_iterator<T, Ref> tree<T, Ref>::insert(tree_node<T, Ref>& node) {
    this->disown_all();
    if (_root) {
      _root->insert(node, _data);
    } else {
      _root = &node;
    }
    _root = Ref<tree_node<T, Ref>>{_root->root(_data), _data};
    return tree_iterator<T, Ref>{this, &node};
  }
}

#endif