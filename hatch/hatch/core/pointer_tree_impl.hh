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
  void pointer_tree<T>::rotate_prev(pointer_tree* here) {
    auto* next = here->_next;
    auto* head = here->_head;
    auto* hook = next->_prev;

    next->_head = head;
    next->_prev = here;
    here->_head = next;
    here->_next = hook;

    if (hook) {
      hook->_head = here;
    }

    if (head) {
      if (head->_prev == here) {
        head->_prev = next;
      }
      if (head->_next == here) {
        head->_next = next;
      }
    }
  }

  template <class T>
  void pointer_tree<T>::rotate_next(pointer_tree* here) {
    auto* prev = here->_prev;
    auto* head = here->_head;
    auto* hook = prev->_next;

    prev->_head = head;
    prev->_next = here;
    here->_head = prev;
    here->_prev = hook;

    if (hook) {
      hook->_head = here;
    }

    if (head) {
      if (head->_prev == here) {
        head->_prev = prev;
      }
      if (head->_next == here) {
        head->_next = prev;
      }
    }
  }

  template <class T>
  void pointer_tree<T>::insert_binary(pointer_tree* here) {
    if (**here < **this) {
      if (_prev) {
        _prev->insert(here);
      } else {
        this->_prev = here;
        here->_head = this;
      }
    } else {
      if (_next) {
        _next->insert(here);
      } else {
        this->_next = here;
        here->_head = this;
      }
    }
  }

  template <class T>
  void pointer_tree<T>::insert_fix(pointer_tree* here) {
    while (here->_head && here->_head->_color == color::red) {
      auto* head = here->_head;
      auto* gran = head->_head;
      if (head == gran->_prev) {
        auto* aunt = gran->_next;
        if (aunt && aunt->_color == color::red) {
          gran->_color = color::red;
          head->_color = color::black;
          aunt->_color = color::black;
          here = gran;
          continue;
        } else {
          if (here == head->_next) {
            rotate_prev(here);
            std::swap(here, head);
          }
          rotate_next(gran);
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
          here = gran;
          continue;
        } else {
          if (here == head->_prev) {
            rotate_next(here);
            std::swap(here, head);
          }
          rotate_prev(gran);
          gran->_color = color::red;
          head->_color = color::black;
          return;
        }
      }
    }
    if (!here->_head) {
      here->_color = color::black;
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
  void pointer_tree<T>::insert(pointer_tree& tree) {
    tree.detach();

    insert_binary(&tree);
    insert_fix(&tree);
  }

} // namespace hatch

#endif // HATCH_POINTER_TREE_IMPL_HH
