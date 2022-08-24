#ifndef HATCH_OWNER_IMPL_HH
#define HATCH_OWNER_IMPL_HH

#ifndef HATCH_OWNING_HH
#error "do not include owner_impl.hh directly. include owning.hh instead."
#endif

#include <utility> // std::forward

#include <iostream>

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, class U, template <class> class Ref>
  owner<T, U, Ref>::owner() :
      _owned{nullptr} {
  }

  template <class T, class U, template <class> class Ref>
  owner<T, U, Ref>::~owner() {
    disown_all();
  }

  template <class T, class U, template <class> class Ref>
  owner<T, U, Ref>::owner(owner&& moved) noexcept {
    transfer_from(static_cast<T&&>(moved));
  }

  template <class T, class U, template <class> class Ref>
  owner<T, U, Ref>& owner<T, U, Ref>::operator=(owner&& moved) noexcept {
    disown_all();
    transfer_from(static_cast<T&&>(moved));
    return *this;
  }

  /////////////
  // Owning. //
  /////////////

  template <class T, class U, template <class> class Ref>
  template <class V>
  void owner<T, U, Ref>::foreach(V&& callable) {
    if (_owned) {
      _owned->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U, template <class> class Ref>
  template <class V>
  void owner<T, U, Ref>::foreach(V&& callable) const {
    if (_owned) {
      _owned->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U, template <class> class Ref>
  void owner<T, U, Ref>::transfer_from(T&& owner) {
    _owned = owner._owned;

    if (auto node = _owned) {
      auto* self = static_cast<T*>(this);
      do {
        node->_owner = self;
        node = node->_next;
      } while (node != _owned);
    }

    owner._owned = nullptr;
  }

  template <class T, class U, template <class> class Ref>
  void owner<T, U, Ref>::disown_all() {
    if (auto node = _owned) {
      do {
        auto next = node->_next;
        node->_owner = nullptr;
        node->_prev = node;
        node->_next = node;
        node = next;
      } while (node != _owned);
    }
    _owned = nullptr;
  }

} // namespace hatch

#endif // HATCH_OWNER_IMPL_HH