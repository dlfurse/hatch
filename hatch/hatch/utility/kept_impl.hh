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
      _keeper{nullptr} {
    attach(keeper);
  }

  template <class T, class U>
  kept<T, U>::~kept() {
    detach();
  }

  template <class T, class U>
  kept<T, U>::kept(kept<T, U>&& moved) noexcept :
      chain<U>{},
      _keeper{nullptr} {
    attach(moved._keeper);
    moved.detach();
  }

  template <class T, class U>
  kept<T, U>& kept<T, U>::operator=(kept<T, U>&& moved) noexcept {
    detach();
    attach(moved._keeper);
    moved.detach();
    return *this;
  }

  template <class T, class U>
  kept<T, U>::kept(const kept<T, U>& copied) :
      chain<U>{},
      _keeper{nullptr} {
    attach(copied._keeper);
  }

  template <class T, class U>
  kept<T, U>& kept<T, U>::operator=(const kept<T, U>& copied) {
    detach();
    attach(copied._keeper);
    return *this;
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
        _keeper->_kept->splice(*self);
      }

      _keeper->_kept = self;
    }
  }

  template <class T, class U>
  void kept<T, U>::detach() {
    if (_keeper) {
      auto* self = static_cast<U*>(this);
      auto* next = &this->next();

      if (self == next) {
        _keeper->_kept = nullptr;
      } else {
        _keeper->_kept = next;
      }

      self->splice(*next);
    }
    _keeper = nullptr;
  }

} // namespace hatch

#endif // HATCH_KEPT_IMPL_HH