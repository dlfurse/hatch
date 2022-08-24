#ifndef HATCH_OWNED_IMPL_HH
#define HATCH_OWNED_IMPL_HH

#ifndef HATCH_OWNING_HH
#error "do not include owned_impl.hh directly. include owning.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>::owned(Ref<T> owner) :
      _owner{nullptr} {
    attach(owner);
  }

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>::~owned() {
    detach();
  }

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>::owned(owned<T, U, Ref>&& moved) noexcept :
      chain<U, Ref>{},
      _owner{nullptr} {
    attach(moved._owner);
    moved.detach();
  }

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>& owned<T, U, Ref>::operator=(owned<T, U, Ref>&& moved) noexcept {
    detach();
    attach(moved._owner);
    moved.detach();
    return *this;
  }

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>::owned(const owned<T, U, Ref>& copied) :
      chain<U, Ref>{},
      _owner{nullptr} {
    attach(copied._owner);
  }

  template <class T, class U, template <class> class Ref>
  owned<T, U, Ref>& owned<T, U, Ref>::operator=(const owned<T, U, Ref>& copied) {
    detach();
    attach(copied._owner);
    return *this;
  }

  /////////////
  // Owning. //
  /////////////

  template <class T, class U, template <class> class Ref>
  void owned<T, U, Ref>::attach(Ref<T> owner) {
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

  template <class T, class U, template <class> class Ref>
  void owned<T, U, Ref>::detach() {
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