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

  template<class T>
  tree<T> tree_iterator<T>::remove() {
    if (_node != _before && _node != _after && !_node->alone()) {
      auto is_null_or_black = [](tree_node<T>* node) {
        return !node || node->is_black();
      };

      auto is_real_and_red = [](tree_node<T>* node) {
        return node && node->is_red();
      };

      if (_node->prev() && _node->next()) {
        // this node has both children, so we swap it with either the
        // predecessor or successor, because either of these is guaranteed to
        // have at most one child.  this may leave the BST ordering condition
        // violated as we adjust the tree to maintain RB conditions; ordering
        // conditions will be restored when we finally remove this node at the
        // end.
        //
        _node->exchange(std::max(_node->predecessor(), _node->successor()));
      }

      if (_node->is_black()) {
        // otherwise this node is red, so it can't have any children.
        //
        // -> if red, no action needed, just fall through to disconnecting this
        //    node from its parent.
        //
        if (auto* child = _node->prev() ? _node->prev() : _node->next() ? _node->next() : nullptr) {
          // this node is black and has a child, which must be red, because this
          // node was chosen by construction to have only one child and the black
          // heights on both sides of it must be equal.
          //
          // -> replace this node with the red child and color the red child
          //    black.
          //
          exchange(child);
          child->make_black();
        } else {
          // the target node is black and has no children, which is the difficult
          // case because simply deleting it will mess up the black height of the
          // tree.  so we have to look at the environment of the node and figure
          // out how to compensate.
          //
          auto* target = this;
          while (target->head()) {
            auto target_self_side = *target->side();
            auto target_away_side = ~target_self_side;

            auto* parent = target->head();
            auto* sibling = parent->child(target_away_side);
            auto* inside = sibling->child(target_self_side);
            auto* outside = sibling->child(target_away_side);

            // sibling may be red or black.
            if (is_real_and_red(sibling)) {
              // sibling is red, which implies that the parent is black. this
              // further implies that the sibling has two subtrees that are one
              // greater in black height than the subtree rooted at the current
              // target pointer.
              //
              // -> color the black parent red and the red sibling black, and
              // rotate the parent in the back direction.
              //
              parent->make_red();
              sibling->make_black();

              parent->rotate(target_self_side);

              sibling = parent->child(target_away_side);
              inside = sibling->child(target_self_side);
              outside = sibling->child(target_away_side);
            }

            // sibling must be black now.
            if (is_null_or_black(inside) && is_null_or_black(outside)) {
              // Sibling and its children are also black. We will reduce the black
              // height of the sibling subtree by making the sibling red, leaving
              // both the current and sibling subtrees with the same height, but
              // that height is one less than they were before the deletion. This
              // may cause problems above the parent since the parent subtree's
              // black height is now lessened. We can either restore the black
              // height of the parent subtree by making it black if it's red, or
              // recursing to address it higher up in the tree.
              //
              sibling->make_red();
              if (parent->is_black()) {
                // parent is black.
                //
                // -> recurse on parent.
                //
                target = parent;
                continue;
              } else {
                // parent is red.
                //
                // -> make parent black, restoring the black height of the parent
                //    subtree.
                //
                parent->make_black();
                break;
              }
            } else {
              if (is_real_and_red(inside) && is_null_or_black(outside)) {
                // sibling has a red inside child and a black outside child.
                //
                // -> switch sibling and inside child colors, rotate the sibling
                //    forward.
                //
                sibling->make_red();
                inside->make_black();

                sibling->rotate(target_away_side);

                sibling = parent->child(target_away_side);
                outside = sibling->child(target_away_side);
              }
              // sibling must have a red outside child now, the inside child color
              // is arbitrary.
              //
              // -> switch sibling and parent child colors, rotate the parent
              //    backward
              //
              sibling->make_color(parent->color());
              parent->make_black();
              outside->make_black();

              parent->rotate(target_self_side);

              break;
            }
          }
        }
        _node->detach();
      }
    }

    tree<T> result{_node};
    _tree = &result;
    return result;
  }

} // namespace hatch

#endif // HATCH_TREE_ITERATOR_IMPL_HH