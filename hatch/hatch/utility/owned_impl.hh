#ifndef HATCH_OWNED_IMPL_HH
#define HATCH_OWNED_IMPL_HH

#ifndef HATCH_OWNING_HH
#error "do not include owned_impl.hh directly. include owning.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, class U>
  owned<T, U>::owned(T* owner) :
      _owner{nullptr} {
    attach(owner);
  }

  template <class T, class U>
  owned<T, U>::~owned() {
    detach();
  }

  template <class T, class U>
  owned<T, U>::owned(owned<T, U>&& moved) noexcept :
      chain<U>{},
      _owner{nullptr} {
    attach(moved._owner);
    moved.detach();
  }

  template <class T, class U>
  owned<T, U>& owned<T, U>::operator=(owned<T, U>&& moved) noexcept {
    detach();
    attach(moved._owner);
    moved.detach();
    return *this;
  }

  template <class T, class U>
  owned<T, U>::owned(const owned<T, U>& copied) :
      chain<U>{},
      _owner{nullptr} {
    attach(copied._owner);
  }

  template <class T, class U>
  owned<T, U>& owned<T, U>::operator=(const owned<T, U>& copied) {
    detach();
    attach(copied._owner);
    return *this;
  }

  ///////////
  // Keep. //
  ///////////

  template <class T, class U>
  void owned<T, U>::attach(T* owner) {
    detach();

    _owner = owner;
    if (_owner) {
      auto* self = static_cast<U*>(this);

      if (_owner->_owned) {
        _owner->_owned->splice(*self);
      }

      _owner->_owned = self;
    }
  }

  template <class T, class U>
  void owned<T, U>::detach() {
    if (_owner) {
      auto* self = static_cast<U*>(this);
      auto* next = &this->next();

      if (self == next) {
        _owner->_owned = nullptr;
      } else {
        _owner->_owned = next;
      }

      self->splice(*next);
    }
    _owner = nullptr;
  }

} // namespace hatch

#endif // HATCH_OWNED_IMPL_HH