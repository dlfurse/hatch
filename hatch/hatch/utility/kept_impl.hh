#ifndef HATCH_KEPT_IMPL_HH
#define HATCH_KEPT_IMPL_HH

#ifndef HATCH_KEEP_HH
#error "do not include kept_impl.hh directly. include keep.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, class U>
  kept<T, U>::kept(T* keeper) :
      _keeper{nullptr},
      _prev{this},
      _next{this} {
    attach(keeper);
  }

  template <class T, class U>
  kept<T, U>::~kept() {
    detach();
  }

  template <class T, class U>
  kept<T, U>::kept(kept<T, U>&& moved) noexcept :
      _keeper{nullptr},
      _prev{this},
      _next{this} {
    attach(moved._keeper);
    moved.detach();
  }

  template <class T, class U>
  kept<T, U>& kept<T, U>::operator=(kept<T, U>&& moved) noexcept {
    detach();
    attach(moved._keeper);
    moved.detach();
  }

  template <class T, class U>
  kept<T, U>::kept(const kept<T, U>& copied) :
      _keeper{nullptr},
      _prev{this},
      _next{this}  {
    attach(copied._keeper);
  }

  template <class T, class U>
  kept<T, U>& kept<T, U>::operator=(const kept<T, U>& copied) {
    detach();
    attach(copied._keeper);
  }

  ///////////
  // Keep. //
  ///////////

  template <class T, class U>
  void kept<T, U>::attach(T* keeper) {
    detach();

    _keeper = keeper;
    if (_keeper) {
      auto* self = static_cast<U*>(this);

      if (_keeper->_kept) {
        auto* prev = _keeper->_kept;
        auto* next = _keeper->_kept->_next;

        _prev = prev;
        _next = next;

        prev->_next = self;
        next->_prev = self;
      }
      _keeper->_kept = self;
    }
  }

  template <class T, class U>
  void kept<T, U>::detach() {
    if (_keeper) {
      auto* self = static_cast<U*>(this);

      if (_keeper->_kept == self) {
        _keeper->_kept = _next;
        if (_keeper->_kept == self) {
          _keeper->_kept = nullptr;
        }
      }

      auto* prev = _prev;
      auto* next = _next;

      _prev = self;
      _next = self;

      prev->_next = next;
      next->_prev = prev;
    }
    _keeper = nullptr;
  }

} // namespace hatch

#endif // HATCH_KEPT_IMPL_HH