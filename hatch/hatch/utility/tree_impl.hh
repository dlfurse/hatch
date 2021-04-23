#ifndef HATCH_TREE_IMPL_HH
#define HATCH_TREE_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_impl.hh directly. include tree.hh instead."
#endif

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
      remove(*_root);
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
    auto* current = &node;

    current->detach();
    current->make_red();

    if (auto* parent = _root) {
      // this tree has a root already.
      //
      // -> go through the normal binary search tree insertion procedure. later
      //    we will adjust the tree to correct red-black properties, if needed.
      //
      while (true) {
        if (current->get() < parent->get()) {
          if (parent->prev()) {
            parent = parent->prev();
            continue;
          } else {
            parent->make_child(current, tree_node<T>::sides::prev);
            break;
          }
        } else {
          if (parent->next()) {
            parent = parent->next();
            continue;
          } else {
            parent->make_child(current, tree_node<T>::sides::next);
            break;
          }
        }
      }

      while (parent && parent->is_red()) {
        // node has a red parent, which means it must have a grandparent as well.
        auto parent_self_side = *parent->side();
        auto parent_away_side = ~parent_self_side;

        auto* grandma = parent->head();
        auto* aunt = grandma->child(parent_away_side);

        if (aunt && aunt->is_red()) {
          // this node's parent has a red sibling.
          //
          // -> swap the colors of the parent + parent's sibling with the color of
          //    the grandparent, then recurse on the grandparent.
          //
          grandma->make_red();
          parent->make_black();
          aunt->make_black();

          current = grandma;
          parent = current->head();

          continue;
        } else {
          // this node's parent has either a black sibling or no sibling.
          //
          // -> rotate the grandparent away putting the parent in its place, then
          //    swap the the colors of the grandparent and the parent.
          //
          if (current->side() == parent_away_side) {
            // this node is on a different side of its parent than its parent is
            // with respect to the parent's parent.
            //
            // -> rotate the parent away from this node so it descends from this
            //    node but is on the same side of it as the parent is to the
            //    grandparent, then swap the current and parent pointers so the
            //    relationships are correctly labeled.
            //
            parent->rotate(parent_self_side);
            std::swap(current, parent);
          }

          grandma->rotate(parent_away_side);
          grandma->make_red();
          parent->make_black();

          break;
        }
      }
    }

    if (current->is_root()) {
      // this node is the root.
      //
      // -> color it black.
      //
      current->make_black();
    } else {
      // this node is not the root.
      //
      // -> move to the current root.
      //
      while (current->head()) {
        current = current->head();
      }
    }

    _root = current;
  }
}

#endif