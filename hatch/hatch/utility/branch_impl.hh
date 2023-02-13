#ifndef HATCH_BRANCH_IMPL_HH
#define HATCH_BRANCH_IMPL_HH

#ifndef HATCH_BRANCH_HH
#error "do not include branch.hh directly. include tree.hh instead."
#endif

#include <algorithm>

namespace hatch {

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::sides_t branch<R, A...>::swap(sides_t side) {
    switch (side) {
      case sides_t::root:
        return sides_t::root;
      case sides_t::prev:
        return sides_t::next;
      case sides_t::next:
        return sides_t::prev;
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::colors_t branch<R, A...>::swap(colors_t color) {
    switch (color) {
      case colors_t::black:
        return colors_t::red;
      case colors_t::red:
        return colors_t::black;
    }
  }

  /**
   * Constructors, destructor, assignment.
   */

  template <template <class, auto ...> class R, auto ...A>
  branch<R, A...>::branch() :
      _color{colors_t::black},
      _side{sides_t::root},
      _head{},
      _prev{},
      _next{} {
  }

  template <template <class, auto ...> class R, auto ...A>
  branch<R, A...>::~branch() {
    detach();
  }

  template <template <class, auto ...> class R, auto ...A>
  branch<R, A...>::branch(branch&& moved) noexcept :
      _color{moved._color},
      _side{},
      _head{},
      _prev{},
      _next{} {
    moved._color = colors_t::black;
    make_head(moved._head, moved._side);
    make_prev(moved._prev);
    make_next(moved._next);
  }

  template <template <class, auto ...> class R, auto ...A>
  branch<R, A...>& branch<R, A...>::operator=(branch&& moved) noexcept {
    _color = moved._color;
    moved._color = colors_t::black;
    make_head(moved._head, moved._side);
    make_prev(moved._prev);
    make_next(moved._next);
    return *this;
  }

  /**
   * Color.
   */

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::colors_t branch<R, A...>::color() const {
    return _color;
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_color(colors_t color) {
    _color = color;
  }

  template <template <class, auto ...> class R, auto ...A>
  bool branch<R, A...>::is_red() const {
    return _color == colors_t::red;
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_red() {
    _color = colors_t::red;
  }

  template <template <class, auto ...> class R, auto ...A>
  bool branch<R, A...>::is_black() const {
    return _color == colors_t::black;
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_black() {
    _color = colors_t::black;
  }

  /**
   * Accessors.
   */

  template <template <class, auto ...> class R, auto ...A>
  bool branch<R, A...>::alone() const {
    return !_head && !_prev && !_next;
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::sides_t branch<R, A...>::side() const {
    return _side;
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::head() {
    return _head;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::head() const {
    return const_cast<branch&>(*this).head();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::prev() {
    return _prev;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::prev() const {
    return const_cast<branch&>(*this).prev();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::next() {
    return _next;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::next() const {
    return const_cast<branch&>(*this).next();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::child(sides_t side) {
    switch (side) {
      case sides_t::root:
        return ref_t{};
      case sides_t::prev:
        return _prev;
      case sides_t::next:
        return _next;
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::child(sides_t side) const {
    return const_cast<branch&>(*this).child(side);
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::root() {
    auto* current_ptr = this;
    auto current_ref = ref_t{this};
    while (current_ptr->head()) {
      current_ref = current_ptr->head();
      current_ptr = current_ref();
    }
    return current_ref;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::root() const {
    return const_cast<branch&>(*this).root();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::minimum() {
    auto* current_ptr = this;
    auto current_ref = ref_t{this};
    while (current_ptr->prev()) {
      current_ref = current_ptr->prev();
      current_ptr = current_ref();
    }
    return current_ref;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::minimum() const {
    return const_cast<branch&>(*this).minimum();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::maximum() {
    auto* current_ptr = this;
    auto current_ref = ref_t{this};
    while (current_ptr->next()) {
      current_ref = current_ptr->next();
      current_ptr = current_ref();
    }
    return current_ref;
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::maximum() const {
    return const_cast<branch&>(*this).maximum();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::predecessor() {
    if (prev()) {
      return prev()->maximum();
    } else {
      auto current_ptr = this;
      auto current_ref = ref_t{this};
      while (current_ptr->side() == sides_t::prev) {
        current_ref = current_ptr->head();
        current_ptr = current_ref();
      }
      return current_ptr->head() ? current_ptr->head() : ref_t{};
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::predecessor() const {
    return const_cast<branch&>(*this).predecessor();
  }

  template <template <class, auto ...> class R, auto ...A>
  typename branch<R, A...>::ref_t branch<R, A...>::successor() {
    if (next()) {
      return next()->minimum();
    } else {
      auto* current_ptr = this;
      auto current_ref = ref_t{this};
      while (current_ptr->side() == sides_t::next) {
        current_ref = current_ptr->head();
        current_ptr = current_ref();
      }
      return current_ptr->head() ? current_ptr->head() : ref_t{};
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  const typename branch<R, A...>::ref_t branch<R, A...>::successor() const {
    return const_cast<branch&>(*this).successor();
  }

  /**
   * Mutators
   */

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_head(ref_t new_head, sides_t new_side) {
    auto old_head = _head;
    auto old_side = _side;

    if (old_head) {
      switch (old_side) {
        case sides_t::root:
          // should never occur.
          break;
        case sides_t::prev:
          old_head->_prev = ref_t{};
          break;
        case sides_t::next:
          old_head->_next = ref_t{};
          break;
      }
    }

    if (new_head) {
      auto* new_head_ptr = new_head();
      switch (new_side) {
        case sides_t::root:
          // should never occur.
          break;
        case sides_t::prev:
          if (new_head_ptr->prev()) {
            auto old_prev_ref = new_head_ptr->prev();
            auto* old_prev_ptr = old_prev_ref();
            old_prev_ptr->_side = sides_t::root;
            old_prev_ptr->_head = ref_t{};
          }
          new_head_ptr->_prev = ref_t{this};
          break;
        case sides_t::next:
          if (new_head_ptr->next()) {
            auto old_next_ref = new_head_ptr->next();
            auto* old_next_ptr = old_next_ref();
            old_next_ptr->_side = sides_t::root;
            old_next_ptr->_head = ref_t{};
          }
          new_head_ptr->_next = ref_t{this};
          break;
      }
    }

    _head = new_head;
    _side = new_side;
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_child(ref_t new_child, sides_t new_side) {
    if (auto old_child_ref = child(new_side)) {
      auto* old_child_ptr = old_child_ref();
      old_child_ptr->_side = sides_t::root;
      old_child_ptr->_head = ref_t{};
    }

    switch (new_side) {
      case sides_t::root:
        // should never occur.
        break;
      case sides_t::prev:
        _prev = new_child;
        break;
      case sides_t::next:
        _next = new_child;
        break;
    }

    if (new_child) {
      auto* new_child_ptr = new_child();
      new_child_ptr->_side = new_side;
      new_child_ptr->_head = ref_t{this};
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_prev(ref_t new_prev) {
    if (auto old_prev = prev()) {
      auto* old_prev_ptr = old_prev();
      old_prev_ptr->_side = sides_t::root;
      old_prev_ptr->_head = ref_t{};
    }

    _prev = new_prev;

    if (new_prev) {
      auto* new_prev_ptr = new_prev();
      new_prev_ptr->_side = sides_t::prev;
      new_prev_ptr->_head = ref_t{this};
    }
  }


  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::make_next(ref_t new_next) {
    if (auto old_next = next()) {
      auto* old_next_ptr = old_next();
      old_next_ptr->_side = sides_t::root;
      old_next_ptr->_head = ref_t{};
    }

    _next = new_next;

    if (new_next) {
      auto* new_next_ptr = new_next();
      new_next_ptr->_side = sides_t::next;
      new_next_ptr->_head = ref_t{this};
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::detach() {
    make_head(ref_t{}, sides_t::root);
    make_black();
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::rotate(sides_t direction) {
    if (auto rotated = child(swap(direction))) {
      auto* rotated_ptr = rotated();
      auto pivoted = rotated_ptr->child(direction);

      auto self = ref_t{this};
      auto new_head = head();
      auto new_side = side();

      rotated_ptr->make_head(new_head, new_side);
      rotated_ptr->make_child(self, direction);
      this->make_child(pivoted, swap(direction));
    }
  }

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::exchange(ref_t that) {
    if (that && that != this) {
      auto this_color = this->color();
      auto that_color = that->color();

      this->make_color(that_color);
      that->make_color(this_color);

      static constexpr branch* null = nullptr;

      auto[parent, child, joined] =
      this == that->head() ? std::make_tuple(this, that, true) :
      that == this->head() ? std::make_tuple(that, this, true) :
      std::make_tuple(null, null, false);

      if (joined) {
        auto parent_side = parent->side();
        auto child_side = *child->side();

        auto head = parent->head();
        auto other = parent->child(swap(child_side));
        auto prev = child->prev();
        auto next = child->next();

        parent->make_head(child, child_side);
        parent->make_prev(prev);
        parent->make_next(next);

        child->make_head(head, parent_side);
        child->make_child(other, swap(child_side));
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

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::insert(ref_t node) {
    auto* current = node();
    auto* parent = this;

    current->remove();
    current->make_red();

    // here we simply go through the binary search tree insertion procedure. at
    // the end, parent will point to the head of the inserted node.
    while (true) {
      if (current->get() < parent->get()) {
        if (parent->prev()) {
          parent = parent->prev()();
          continue;
        } else {
          parent->make_child(ref_t{current}, sides_t::prev);
          break;
        }
      } else {
        if (parent->next()) {
          parent = parent->next();
          continue;
        } else {
          parent->make_child(current, sides_t::next);
          break;
        }
      }
    }

    // here we correct the rb properties of the tree.  at the beginning, the
    // node we just inserted is red.
    while (parent && parent->is_red()) {
      // node has a red parent, which means it must have a grandparent as well.
      auto parent_self_side = *parent->side();
      auto parent_away_side = swap(parent_self_side);

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
        //    swap the colors of the grandparent and the parent.
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

  template <template <class, auto ...> class R, auto ...A>
  void branch<R, A...>::remove() {
    if (!alone()) {

      auto is_null_or_black = [](ref_t node) {
        return !node || node->is_black();
      };

      auto is_real_and_red = [](ref_t node) {
        return node && node->is_red();
      };

      if (_prev && _next) {
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
            auto target_self_side = target->side();
            auto target_away_side = swap(target_self_side);

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
      }
      detach();
    }
  }

} // namespace hatch

#endif // HATCH_TREE_NODE_IMPL_HH