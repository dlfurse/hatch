#ifndef HATCH_TREE_NODE_IMPL_HH
#define HATCH_TREE_NODE_IMPL_HH

#ifndef HATCH_TREE_HH
#error "do not include tree_node.hh directly. include tree.hh instead."
#endif

#include <algorithm>

using std::nullopt;

namespace hatch {

  template <class T, template <class> class Ref>
  typename tree_node<T, Ref>::sides tree_node<T, Ref>::swap(sides side) {
    switch (side) {
      case sides::prev:
        return sides::next;
      case sides::next:
      default:
        return sides::prev;
    }
  }

  ///////////////////////////////
  // Constructors, destructor. //
  ///////////////////////////////

  template <class T, template <class> class Ref>
  template <class ...Args>
  tree_node<T, Ref>::tree_node(Args&&... args) :
      container<T>::container{std::forward<Args>(args)...},
      _color{colors::black},
      _head{},
      _prev{},
      _next{} {
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>::~tree_node() {
    detach();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>::tree_node(tree_node&& moved) noexcept :
      container<T>::container{std::move(moved)},
      _color{colors::black},
      _head{},
      _prev{},
      _next{} {
    auto side = moved.side();
    make_head(moved._head, side);
    make_prev(moved._prev);
    make_next(moved._next);
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>& tree_node<T, Ref>::operator=(tree_node&& moved) noexcept {
    container<T>::operator=(std::move(moved));
    _prev = moved._prev;
    _next = moved._next;
    _prev->_next = this;
    _next->_prev = this;
    moved._prev = &moved;
    moved._next = &moved;
    return *this;
  }

  ////////////
  // Color. //
  ////////////

  template <class T, template <class> class Ref>
  typename tree_node<T, Ref>::colors tree_node<T, Ref>::color() const {
    return _color;
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_color(colors color) {
    _color = color;
  }

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::is_red() const {
    return _color == colors::red;
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_red() {
    _color = colors::red;
  }

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::is_black() const {
    return _color == colors::black;
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_black() {
    _color = colors::black;
  }

  ///////////////////////////
  // Structure: accessors. //
  ///////////////////////////

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::alone() const {
    return !_head && !_prev && !_next;
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::minimum() {
    auto* current = this;
    while (current->_prev) {
      current = _prev;
    }
    return current;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::minimum() const {
    return const_cast<tree_node<T, Ref>&>(*this).minimum();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::predecessor() {
    if (_prev) {
      return _prev->maximum();
    } else {
      auto* current = this;
      while (current->is_prev()) {
        current = current->head();
      }
      return current->head();
    }
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::predecessor() const {
    return const_cast<tree_node<T, Ref>&>(*this).predecessor();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::root() {
    auto* current = this;
    while (current->_head) {
      current = _head;
    }
    return current;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::root() const {
    return const_cast<tree_node<T, Ref>&>(*this).root();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::successor() {
    if (_next) {
      return _next->minimum();
    } else {
      auto current_ref = Ref<tree_node<T, Ref>>{this};
      auto* current_ptr = this;
      while (true) {
        if (auto head_ref = current_ptr->_head) {
          auto* head_ptr = head_ref.address();
          if (head_ptr->_next == current_ref) {
            current_ref = head_ref;
            current_ptr = head_ptr;
          } else {
            break;
          }
        }
      }
      return current_ptr->_head;
    }
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::successor() const {
    return const_cast<tree_node<T, Ref>&>(*this).successor();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::maximum() {
    auto* current = this;
    while (current->_next) {
      current = _next;
    }
    return current;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::maximum() const {
    return const_cast<tree_node<T, Ref>&>(*this).maximum();
  }

  template <class T, template <class> class Ref>
  std::optional<typename tree_node<T, Ref>::sides> tree_node<T, Ref>::side() const {
    if (_head) {
      auto* head = this->head(data);
      if (this == head->prev(data)) {
        return sides::prev;
      }
      if (this == head->next(data)) {
        return sides::next;
      }
    }
    return {};
  }

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::is_root() const {
    return !_head;
  }

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::is_prev(void* data) const {
    if (_head) {
      auto* head = this->head(data);
      return this == head->prev(data);
    }
    return false;
  }

  template <class T, template <class> class Ref>
  bool tree_node<T, Ref>::is_next() const {
    if (_head) {
      auto* head = this->head(data);
      return this == head->next(data);
    }
    return false;
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::head(void* data) {
    if (_head) {
      return &_head.get(data);
    }
    return nullptr;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::head(void* data) const {
    return const_cast<tree_node<T, Ref>&>(*this)->head(data);
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::child(sides side, void* data) {
    switch (side) {
      case sides::prev:
        return prev(data);
      case sides::next:
      default:
        return next(data);
    }
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::child(sides side, void* data) const {
    return const_cast<tree_node<T, Ref>&>(*this)->child(side);
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::prev(void* data) {
    if (_prev) {
      return &_prev.get(data);
    }
    return nullptr;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::prev(void* data) const {
    return const_cast<tree_node<T, Ref>&>(*this)->prev();
  }

  template <class T, template <class> class Ref>
  tree_node<T, Ref>* tree_node<T, Ref>::next(void* data) {
    if (_next) {
      return &_next.get(data);
    }
    return nullptr;
  }

  template <class T, template <class> class Ref>
  const tree_node<T, Ref>* tree_node<T, Ref>::next(void* data) const {
    return const_cast<tree_node<T, Ref>&>(*this)->next(data);
  }

  //////////////////////////
  // Structure: mutators. //
  //////////////////////////

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_head(tree_node* new_head, std::optional<sides> new_side, void* data) {
    if (auto* old_head = head(data)) {
      if (this == old_head->prev(data)) {
        old_head->_prev = Ref<T>{nullptr, data};
      } else if (this == old_head->next(data)) {
        old_head->_next = Ref<T>{nullptr, data};
      }
    }

    _head = Ref<T>{new_head, data};

    if (new_head) {
      auto side = *new_side;
      if (side == sides::prev) {
        new_head->_prev = Ref<T>{this, data};
      } else if (side == sides::next) {
        new_head->_next = Ref<T>{this, data};
      }
    }
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_child(tree_node* new_child, sides side, void* data) {
    if (auto* old_child = child(side, data)) {
      old_child->_head = Ref<T>{nullptr, data};
    }

    switch (side) {
      case sides::prev:
        _prev = Ref<T>{new_child, data};
        break;
      case sides::next:
        _next = Ref<T>{new_child, data};
        break;
    }

    if (new_child) {
      new_child->_head = Ref<T>{this, data};
    }
  }



  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_prev(tree_node* new_prev, void* data) {
    if (auto* old_prev = prev(data)) {
      old_prev->_head = Ref<T>{nullptr, data};
    }

    _prev = Ref<T>{new_prev, data};

    if (new_prev) {
      new_prev->_head = Ref<T>{this, data};
    }
  }



  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::make_next(tree_node* new_next, void* data) {
    if (auto* old_next = next(data)) {
      old_next->_head = Ref<T>{nullptr, data};
    }

    _next = Ref<T>{new_next, data};

    if (new_next) {
      new_next->_head = Ref<T>{this, data};
    }
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::detach(void* data) {
    make_head(nullptr, nullopt, data);
    make_black(data);
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::rotate(sides direction, void* data) {
    if (auto* rotated = child(swap(direction), data)) {
      auto* pivoted = rotated->child(direction, data);

      auto* new_head = head(data);
      auto new_side = side(data);
      rotated->make_head(new_head, new_side, data);
      
      rotated->make_child(this, direction, data);
      this->make_child(pivoted, swap(direction), data);
    }
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::exchange(tree_node<T, Ref>* that, void* data) {
    if (that && that != this) {
      auto this_color = this->color();
      auto that_color = that->color();

      this->make_color(that_color);
      that->make_color(this_color);

      static constexpr tree_node* null = nullptr;

      auto [parent, child, joined] =
          this == that->head(data) ? std::make_tuple(this, that, true) :
          that == this->head(data) ? std::make_tuple(that, this, true) :
          std::make_tuple(null, null, false);

      if (joined) {
        auto parent_side = parent->side(data);
        auto child_side = *child->side(data);

        auto head = parent->head(data);
        auto other = parent->child(swap(child_side), data);
        auto prev = child->prev(data);
        auto next = child->next(data);

        parent->make_head(child, child_side, data);
        parent->make_prev(prev, data);
        parent->make_next(next, data);

        child->make_head(head, parent_side, data);
        child->make_child(other, swap(child_side), data);
      } else {
        auto this_side = this->side(data);
        auto this_head = this->head(data);
        auto this_prev = this->prev(data);
        auto this_next = this->next(data);

        auto that_side = that->side(data);
        auto that_head = that->head(data);
        auto that_prev = that->prev(data);
        auto that_next = that->next(data);

        this->make_head(that_head, that_side, data);
        this->make_prev(that_prev, data);
        this->make_next(that_next, data);

        that->make_head(this_head, this_side, data);
        that->make_prev(this_prev, data);
        that->make_next(this_next, data);
      }
    }
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::insert(tree_node<T, Ref>& node, void* data) {
    node.remove(data);
    node.make_red();

    auto* current = &node;
    auto* parent = this;

    // here we simply go through the binary search tree insertion procedure. at
    // the end, parent will point to the head of the inserted node.
    while (true) {
      if (current->get() < parent->get()) {
        if (auto* prev = parent->prev(data)) {
          parent = prev;
          continue;
        } else {
          parent->make_child(current, sides::prev, data);
          break;
        }
      } else {
        if (auto* next = parent->next(data)) {
          parent = next;
          continue;
        } else {
          parent->make_child(current, sides::next, data);
          break;
        }
      }
    }

    // here we correct the rb properties of the tree.  at the beginning, the
    // node we just inserted is red.
    while (parent && parent->is_red()) {
      // node has a red parent, which means it must have a grandparent as well.
      auto parent_self_side = *parent->side(data);
      auto parent_away_side = swap(parent_self_side);

      auto* grandma = parent->head(data);
      auto* aunt = grandma->child(parent_away_side, data);

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
        parent = current->head(data);

        continue;
      } else {
        // this node's parent has either a black sibling or no sibling.
        //
        // -> rotate the grandparent away putting the parent in its place, then
        //    swap the the colors of the grandparent and the parent.
        //
        if (current->side(data) == parent_away_side) {
          // this node is on a different side of its parent than its parent is
          // with respect to the parent's parent.
          //
          // -> rotate the parent away from this node so it descends from this
          //    node but is on the same side of it as the parent is to the
          //    grandparent, then swap the current and parent pointers so the
          //    relationships are correctly labeled.
          //
          parent->rotate(parent_self_side, data);
          std::swap(current, parent);
        }

        grandma->rotate(parent_away_side, data);
        grandma->make_red();
        parent->make_black();

        return;
      }
    }

    // we only get here if we break out of the loop, which might happen by
    // recursing up to the root, in which case the root will be red now.
    // if this is the case, make the root black to restore red-black properties.
    if (current->is_root()) {
      // this node is the root.
      //
      // -> color it black.
      //
      current->make_black();
    }
  }

  template <class T, template <class> class Ref>
  void tree_node<T, Ref>::remove(void* data) {
    if (!alone()) {

      auto is_null_or_black = [](tree_node<T, Ref>* node) {
        return !node || node->is_black();
      };

      auto is_real_and_red = [](tree_node<T, Ref>* node) {
        return node && node->is_red();
      };

      auto* prev = this->prev(data);
      auto* next = this->next(data);

      if (prev && next) {
        // this node has both children, so we swap it with either the
        // predecessor or successor, because either of these is guaranteed to
        // have at most one child.  this may leave the BST ordering condition
        // violated as we adjust the tree to maintain RB conditions; ordering
        // conditions will be restored when we finally remove this node at the
        // end.
        //
        if (auto* pred = predecessor(data)) {
          exchange(pred, data);
        } else if (auto* succ = successor(data)) {
          exchange(succ, data);
        }
      }

      if (is_black()) {
        // otherwise this node is red, so it can't have any children.
        //
        // -> if red, no action needed, just fall through to disconnecting this
        //    node from its parent.
        //
        if (auto* child = prev ? prev : next ? next : nullptr) {
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
          while (target->_head) {
            auto target_self_side = *target->side(data);
            auto target_away_side = swap(target_self_side);

            auto* parent = target->head(data);
            auto* sibling = parent->child(target_away_side, data);
            auto* inside = sibling->child(target_self_side, data);
            auto* outside = sibling->child(target_away_side, data);

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

              parent->rotate(target_self_side, data);

              sibling = parent->child(target_away_side, data);
              inside = sibling->child(target_self_side, data);
              outside = sibling->child(target_away_side, data);
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

                sibling->rotate(target_away_side, data);

                sibling = parent->child(target_away_side, data);
                outside = sibling->child(target_away_side, data);
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

              parent->rotate(target_self_side, data);

              break;
            }
          }
        }
      }
      detach(data);
    }
  }

} // namespace hatch

#endif // HATCH_TREE_NODE_IMPL_HH