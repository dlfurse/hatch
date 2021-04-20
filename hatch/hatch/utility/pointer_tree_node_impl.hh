#ifndef HATCH_POINTER_TREE_NODE_IMPL_HH
#define HATCH_POINTER_TREE_NODE_IMPL_HH

#ifndef HATCH_POINTER_TREE_HH
#error "do not include pointer_tree_node.hh directly. include pointer_tree.hh instead."
#endif

#include <algorithm>

namespace hatch {

  template<class T>
  pointer_tree_node<T>::pointer_tree_node() :
      _head{nullptr},
      _prev{nullptr},
      _next{nullptr},
      _color{pointer_tree::colors::black} {
  }

  template<class T>
  pointer_tree_node<T>::~pointer_tree_node() {
    detach();
  }

  template<class T>
  std::optional<pointer_tree::sides> pointer_tree_node<T>::side() const {
    if (_head) {
      if (this == _head->_prev) {
        return pointer_tree::sides::prev;
      }
      if (this == _head->_next) {
        return pointer_tree::sides::next;
      }
    }
    return {};
  }

  template<class T>
  bool pointer_tree_node<T>::is_root() const {
    return !_head;
  }

  template<class T>
  bool pointer_tree_node<T>::is_prev() const {
    return _head && this == _head->_prev;
  }

  template<class T>
  bool pointer_tree_node<T>::is_next() const {
    return _head && this == _head->_next;
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::head() {
    return _head;
  }

  template <class T>
  void pointer_tree_node<T>::make_head(pointer_tree_node* new_head, std::optional<pointer_tree::sides> new_side) {
    if (auto* old_head = head()) {
      switch (*side()) {
        case pointer_tree::sides::prev:
          old_head->_prev = nullptr;
          break;
        case pointer_tree::sides::next:
          old_head->_next = nullptr;
          break;
      }
    }

    _head = new_head;

    if (new_head) {
      switch (*new_side) {
        case pointer_tree::sides::prev:
          new_head->_prev = this;
          break;
        case pointer_tree::sides::next:
          new_head->_next = this;
          break;
      }
    }
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::child(pointer_tree::sides type) {
    switch (type) {
      case pointer_tree::sides::prev:
        return _prev;
      case pointer_tree::sides::next:
        return _next;
    }
  }

  template <class T>
  void pointer_tree_node<T>::make_child(pointer_tree_node* new_child, pointer_tree::sides side) {
    if (auto* old_child = child(side)) {
      old_child->_head = nullptr;
    }

    switch (side) {
      case pointer_tree::sides::prev:
        _prev = new_child;
        break;
      case pointer_tree::sides::next:
        _next = new_child;
        break;
    }

    if (new_child) {
      new_child->_head = this;
    }
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::prev() {
    return _prev;
  }

  template <class T>
  void pointer_tree_node<T>::make_prev(pointer_tree_node* new_prev) {
    if (auto* old_prev = prev()) {
      old_prev->_head = nullptr;
    }

    _prev = new_prev;

    if (new_prev) {
      new_prev->_head = this;
    }
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::next() {
    return _next;
  }

  template <class T>
  void pointer_tree_node<T>::make_next(pointer_tree_node* new_next) {
    if (auto* old_next = next()) {
      old_next->_head = nullptr;
    }

    _next = new_next;

    if (new_next) {
      new_next->_head = this;
    }
  }

  template<class T>
  void pointer_tree_node<T>::detach() {
    make_head(nullptr);
    make_black();
  }

  template<class T>
  void pointer_tree_node<T>::rotate(pointer_tree::sides direction) {
    if (auto* rotated = child(~direction)) {
      auto* pivoted = rotated->child(direction);

      auto* new_head = head();
      auto new_side = side();
      rotated->make_head(new_head, new_side);
      
      rotated->make_child(this, direction);
      this->make_child(pivoted, ~direction);
    }
  }

  template<class T>
  void pointer_tree_node<T>::exchange(pointer_tree_node* that) {

    if (that) {

      auto this_color = this->color();
      auto that_color = that->color();

      this->make_color(that_color);
      that->make_color(this_color);

      static constexpr pointer_tree_node* null = nullptr;
      
      auto [parent, child] = this == that->head() ? std::make_tuple(this, that) :
                             that == this->head() ? std::make_tuple(that, this) :
                             std::make_tuple(null, null);
      
      if (parent && child) {
        auto parent_side = parent->side();
        auto child_side = *child->side();
        
        auto head = parent->head();
        auto other = parent->child(~child_side);
        auto prev = child->prev();
        auto next = child->next();
        
        parent->make_head(child, child_side);
        parent->make_prev(prev);
        parent->make_next(next);
        
        child->make_head(head, parent_side);
        child->make_child(other, ~child_side);
      } else {
        auto this_side = this->side();
        auto this_head = this->head();
        auto this_prev = this->prev();
        auto this_next = this->next();

        auto that_side = that->side();
        auto that_head = that->head();
        auto that_prev = that->prev();
        auto that_next = that->next();

        this->make_head(that_head, that_side);
        this->make_prev(that_prev);
        this->make_next(that_next);

        that->make_head(this_head, this_side);
        that->make_prev(this_prev);
        that->make_next(this_next);
      }
    }
  }

  template<class T>
  pointer_tree::colors pointer_tree_node<T>::color() const {
    return _color;
  }

  template<class T>
  void pointer_tree_node<T>::make_color(pointer_tree::colors color) {
    _color = color;
  }

  template<class T>
  bool pointer_tree_node<T>::is_red() const {
    return _color == pointer_tree::colors::red;
  }

  template<class T>
  void pointer_tree_node<T>::make_red() {
    _color = pointer_tree::colors::red;
  }

  template<class T>
  bool pointer_tree_node<T>::is_black() const {
    return _color == pointer_tree::colors::black;
  }

  template<class T>
  void pointer_tree_node<T>::make_black() {
    _color = pointer_tree::colors::black;
  }

  template<class T>
  T& pointer_tree_node<T>::get() const {
    return const_cast<T&>(static_cast<const T&>(*this));
  }

  template<class T>
  bool pointer_tree_node<T>::alone() {
    return !_head && !_prev && !_next;
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::root() {
    auto* current = this;
    while (current->head()) {
      current = current->head();
    }
    return current;
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::predecessor() {
    if (_prev) {
      return _prev->maximum();
    } else {
      auto* current = this;
      while (current->is_prev()) {
        current = current->head();
      }
      return current->head() ? current->head() : nullptr;
    }
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::minimum() {
    auto* current = this;
    while (current->prev()) {
      current = current->prev();
    }
    return current;
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::successor() {
    if (_next) {
      return _next->minimum();
    } else {
      auto* current = this;
      while (current->is_next()) {
        current = current->head();
      }
      return current->head() ? current->head() : nullptr;
    }
  }

  template<class T>
  pointer_tree_node<T>* pointer_tree_node<T>::maximum() {
    auto* current = this;
    while (current->next()) {
      current = current->next();
    }
    return current;
  }

  template<class T>
  void pointer_tree_node<T>::insert(pointer_tree_node& node) {
    auto* current = &node;
    auto* parent = this;

    node.remove();
    node.make_red();

    // here we simply go through the normal binary search tree insertion
    // procedure.
    while (true) {
      if (current->get() < parent->get()) {
        if (parent->prev()) {
          parent = parent->prev();
          continue;
        } else {
          parent->make_child(current, pointer_tree::sides::prev);
          break;
        }
      } else {
        if (parent->next()) {
          parent = parent->next();
          continue;
        } else {
          parent->make_child(current, pointer_tree::sides::next);
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
        // -> swap the colors of the parent + parent's sibling with the color of the grandparent,
        //    then recurse on the grandparent.

        grandma->make_red();
        parent->make_black();
        aunt->make_black();

        current = grandma;
        parent = current->head();

        continue;
      } else {
        // this node's parent has either a black sibling or no sibling.
        //
        // -> rotate the grandparent away putting the parent in its place, then swap the the colors
        //    of the grandparent and the parent.

        if (current->side() == parent_away_side) {
          // this node is on a different side of its parent than its parent is with respect to the
          // parent's parent.
          //
          // -> rotate the parent away from this node so it descends from this node but is on the
          //    same side of it as the parent is to the grandparent, then swap the current and
          //    parent pointers so the relationships are correctly labeled.

          parent->rotate(parent_self_side);
          std::swap(current, parent);
        }

        grandma->rotate(parent_away_side);
        grandma->make_red();
        parent->make_black();

        return;
      }
    }

    if (current->is_root()) {
      // this node is the root.
      //
      // -> color it black.

      current->make_black();
    }
  }

  template<class T>
  void pointer_tree_node<T>::remove() {
    if (!alone()) {

      auto is_null_or_black = [](pointer_tree_node* node) {
        return !node || node->is_black();
      };

      auto is_real_and_red = [](pointer_tree_node* node) {
        return node && node->is_red();
      };

      if (prev() && next()) {
        // this node has both children, so we swap it with either the
        // predecessor or successor, because either of these is guaranteed to
        // have at most one child.  this may leave the BST ordering condition
        // violated as we adjust the tree to maintain RB conditions; ordering
        // conditions will be restored when we finally remove this node at the
        // end.
        //
        exchange(std::max(predecessor(), successor()));
      }

      if (is_black()) {
        // otherwise this node is red, so it can't have any children.
        //
        // -> if red, no action needed, just fall through to disconnecting this
        //    node from its parent.
        //
        if (auto* child = prev() ? prev() : next() ? next() : nullptr) {
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
        detach();
      }
    }
  }

}

#endif