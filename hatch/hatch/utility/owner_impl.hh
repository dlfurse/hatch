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

  template <class T, class U>
  owner<T, U>::owner() :
      _owned{nullptr} {
  }

  template <class T, class U>
  owner<T, U>::~owner() {
    release();
  }

  template <class T, class U>
  owner<T, U>::owner(owner&& moved) noexcept {
    acquire(static_cast<T&&>(moved));
  }

  template <class T, class U>
  owner<T, U>& owner<T, U>::operator=(owner&& moved) noexcept {
    release();
    acquire(static_cast<T&&>(moved));
    return *this;
  }

  ///////////
  // Keep. //
  ///////////

  template <class T, class U>
  template <class V>
  void owner<T, U>::foreach(V&& callable) {
    if (_owned) {
      _owned->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U>
  template <class V>
  void owner<T, U>::foreach(V&& callable) const {
    if (_owned) {
      _owned->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U>
  void owner<T, U>::acquire(T&& owner) {
    _owned = owner._owned;

    foreach([&](U& node) {
      node._owner = static_cast<T*>(this);
    });

    owner._owned = nullptr;
  }

  template <class T, class U>
  void owner<T, U>::release() {
    foreach([&](U& node) {
      node._owner = nullptr;
    });

    if (auto* node = _owned) {
      while (!node->alone()) {
        auto* next = &node->next();
        node->splice(node->next());
        node = next;
      }
    }

    _owned = nullptr;
  }

} // namespace hatch

#endif // HATCH_OWNER_IMPL_HH