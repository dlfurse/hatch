#ifndef HATCH_POINTER_TREE_IMPL_HH
#define HATCH_POINTER_TREE_IMPL_HH

#ifndef HATCH_STRUCTURES_HH
#error "do not include pointer_tree_impl.hh directly. include structures.hh instead."
#endif

namespace hatch {

  template <class T>
  pointer_tree<T>::pointer_tree() :
      _head{nullptr},
      _prev{nullptr},
      _next{nullptr} {
  }

  template <class T>
  pointer_tree<T>::~pointer_tree() {
    detach();
  }

  template <class T>
  void pointer_tree<T>::rotate_prev() {
    if (auto* next = this->_next) {
      this->_head = next;
      next->_prev = this;

      auto* hook = next->_prev;
      this->_next = hook;
			if (hook) {
				hook->_head = this;
			}

      auto* head = this->_head;
      next->_head = head;
			if (head) {
				if (head->_prev == this) {
					head->_prev = next;
				}
				if (head->_next == this) {
					head->_next = next;
				}
			}
    }
  }

  template <class T>
  void pointer_tree<T>::rotate_next() {
    if (auto* prev = this->_prev) {
      this->_head = prev;
      prev->_next = this;

      auto* hook = prev->_next;
      this->_prev = hook;
      if (hook) {
        hook->_head = this;
      }

      auto* head = this->_head;
      prev->_head = head;
      if (head) {
        if (head->_prev == this) {
          head->_prev = prev;
        }
        if (head->_next == this) {
          head->_next = prev;
        }
      }
    }
  }

  template <class T>
  int pointer_tree<T>::black_depth() {
    int depth = _color == color::black ? 1 : 0;
    if (_prev && _next) {
      int prev = _prev->black_depth();
      int next = _next->black_depth();
      if (prev > 0 && next > 0 && prev == next) {
        depth += (prev + next) / 2;
      } else {
        depth = -1;
      }
    } else if (_prev) {
      depth += _prev->black_depth();
    } else if (_next) {
      depth += _next->black_depth();
    }
    return depth;
  }

  template <class T>
  T& pointer_tree<T>::operator*() {
    return static_cast<T&>(*this);
  }

  template <class T>
  const T& pointer_tree<T>::operator*() const {
    return static_cast<const T&>(*this);
  }

  template <class T>
  bool pointer_tree<T>::detached() {
    return !_head && !_prev && !_next;
  }

  template <class T>
  void pointer_tree<T>::insert(pointer_tree& node) {
    // this will make node's _head, _prev, and _next all null.
    node.detach();

    // here we simply go through the normal binary search tree insertion procedure.
    auto* parent = this;
    while (true) {
      if (**parent > *node) {
        if (_prev) {
          parent = _prev;
          continue;
        } else {
          parent->_prev = &node;
          node._head = parent;
          break;
        }
      } else {
        if (_next) {
          parent = _next;
          continue;
        } else {
          parent->_next = &node;
          node._head = parent;
          break;
        }
      }
    }

    // here we fix up the tree so that the rb properties remain satisfied.
    auto* current = &node;
    while (current->_head && current->_head->_color == color::red) {
      auto* head = current->_head;
      auto* gran = head->_head;
      if (head == gran->_prev) {
        auto* aunt = gran->_next;
        if (aunt && aunt->_color == color::red) {
          gran->_color = color::red;
          head->_color = color::black;
          aunt->_color = color::black;
          current = gran;
          continue;
        } else {
          if (current == head->_next) {
            head->rotate_prev();
            std::swap(current, head);
          }
          gran->rotate_next();
          gran->_color = color::red;
          head->_color = color::black;
          return;
        }
      } else {
        auto* aunt = gran->_prev;
        if (aunt && aunt->color == color::red) {
          gran->color = color::red;
          head->color = color::black;
          aunt->color = color::black;
          current = gran;
          continue;
        } else {
          if (current == head->_prev) {
            head->rotate_next();
            std::swap(current, head);
          }
          gran->rotate_prev();
          gran->_color = color::red;
          head->_color = color::black;
          return;
        }
      }
    }
    if (!current->_head) {
      current->_color = color::black;
    }
  }

  template <class T>
  void pointer_tree<T>::detach() {
    if (!detached()) {
      auto const* pred = predecessor();
      auto const* succ = successor();
      auto* target = !pred ? this : !succ ? this : pred;

      if (target->_color == color::red) {
        // the target node is red, so it can't have any children.
        //
        // -> disconnect the target from its parent.

        auto* parent = target->_head;
        if (parent) {
          if (parent->_prev == target) {
            parent->_prev = nullptr;
          }
          if (parent->_next == target) {
            parent->_next = nullptr;
          }
        }

        target->_head = nullptr;
        target->_color = color::black;

      } else {
        // in the two cases below, the target node is black and has a child, which must be red,
        // because the target node, being a successor, can have only one child and the black heights
        // on both sides of the target node must be equal.
        //
        // -> replace the target node with the red child and color the red child black.

        if (auto const* prev = target->_prev) {

          auto* parent = target->_head;
          prev->_head = parent;
          if (parent) {
            if (parent->_prev == target) {
              parent->_prev = prev;
            }
            if (parent->_next == target) {
              parent->_next = prev;
            }
          }

          target->_head = nullptr;
          target->_prev = nullptr;

          prev->_color = color::black;
          target->_color = color::black;

        } else if (auto const* next = target->_next) {

          auto* parent = target->_head;
          next->_head = parent;
          if (parent) {
            if (parent->_prev == target) {
              parent->_prev = next;
            }
            if (parent->_next == target) {
              parent->_next = next;
            }
          }

          target->_head = nullptr;
          target->_next = nullptr;

          next->_color = color::black;
          target->_color = color::black;

        } else {
          // the target node is black and has no children, which is the difficult case because
          // simply deleting it will mess up the black height of the tree.  so we have to look at
          // the environment of the node and figure out how to compensate.

          auto color = [](pointer_tree* node) {
            return node ? node->_color : color::black;
          };

          pointer_tree* parent = target->_head;
          pointer_tree* pointer_tree::*child_away = nullptr;
          pointer_tree* pointer_tree::*child_self = nullptr;
          void (pointer_tree::* rotate_away)(void) = nullptr;
          void (pointer_tree::* rotate_self)(void) = nullptr;

          if (parent) {
            if (parent->_prev == target) {
              parent->_prev = nullptr;
              child_away = &pointer_tree::_next;
              child_self = &pointer_tree::_prev;
              rotate_away = &pointer_tree::rotate_next;
              rotate_self = &pointer_tree::rotate_prev;
            }
            if (parent->_next == target) {
              parent->_next = nullptr;
              child_away = &pointer_tree::_prev;
              child_self = &pointer_tree::_next;
              rotate_away = &pointer_tree::rotate_prev;
              rotate_self = &pointer_tree::rotate_next;
            }
          }

          target->_head = nullptr;
          target->_color = color::black;

          while (parent) {
            auto* sibling = parent->*child_away;

            if (color(sibling) == color::red) {
              // sibling is red, which implies that the parent is black. this further implies that
              // the sibling has two subtrees that are one greater in black height than the subtree
              // rooted at the current target pointer.
              //
              // -> color the black parent red and the red sibling black, and rotate the parent in
              // the back direction.

              parent->_color = color::red;
              sibling->_color = color::black;
              (parent->*rotate_self)();
              sibling = parent->*child_away;
            }

            // sibling must be black now.
            if (color(sibling->*child_away) == color::black &&
                color(sibling->*child_self) == color::black) {
              // sibling's children are also black.  we will reduce the black-height of the sibling
              // subtree by making the sibling red.  we can either restore the black height of the
              // parent subtree by making it black if it's red, or recursing to address it higher
              // up in the tree.
              sibling->_color = color::red;

              if (color(parent) == color::black) {
                // parent is black.
                //
                // -> recurse on parent.
                parent = parent->_head;
                if (parent) {
                  if (parent->_prev == target) {
                    child_away = &pointer_tree::_next;
                    child_self = &pointer_tree::_prev;
                    rotate_away = &pointer_tree::rotate_next;
                    rotate_self = &pointer_tree::rotate_prev;
                  }
                  if (parent->_next == target) {
                    child_away = &pointer_tree::_prev;
                    child_self = &pointer_tree::_next;
                    rotate_away = &pointer_tree::rotate_prev;
                    rotate_self = &pointer_tree::rotate_next;
                  }
                }
                continue;
              } else {
                // parent is red.
                //
                // -> make parent black, restoring the black height of the parent subtree.
                parent->_color = color::black;
                break;
              }
            } else {
              if (color(sibling->*child_self) == color::red &&
                  color(sibling->*child_away) == color::black) {
                // sibling has a red inside child and a black outside child.
                //
                // -> switch sibling and inside child colors, rotate the sibling forward

                sibling->_color = color::red;
                sibling->*child_self->_color = color::black;
                (sibling->*rotate_away)();
                sibling = parent->*child_away;
              }
              // sibling must have a red outside child now, the inside child color is arbitrary.
              //
              // -> switch sibling and inside child colors, rotate the sibling forward

              sibling->_color = parent->_color;
              parent->_color = color::black;
              sibling->*child_away->_color = color::black;
              (parent->*rotate_self)();
              break;
            }
          }
        }
      }

      if (target != this) {
        // the target node is now detached and we need to swap it with this, which becomes black.

        auto* head = _head;
        target->_head = head;
        if (head) {
          if (head->_prev == this) {
            head->_prev = target;
          }
          if (head->_next == this) {
            head->_next = target;
          }
        }

        auto* prev = _prev;
        target->_prev = prev;
        if (prev) {
          prev->_head = target;
        }

        auto* next = _next;
        target->_next = next;
        if (next) {
          target->_next = next;
        }

        this->_nead = nullptr;
        this->_prev = nullptr;
        this->_next = nullptr;

        target->_color = this->_color;
        this->_color = color::black;
      }
    }
  }

} // namespace hatch

#endif // HATCH_POINTER_TREE_IMPL_HH
