#ifndef HATCH_KEEPER_IMPL_HH
#define HATCH_KEEPER_IMPL_HH

#ifndef HATCH_KEEP_HH
#error "do not include keeper_impl.hh directly. include keep.hh instead."
#endif

#include <utility> // std::forward

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, class U>
  keeper<T, U>::keeper() :
      _kept{nullptr} {
  }

  template <class T, class U>
  keeper<T, U>::~keeper() {
    release();
  }

  template <class T, class U>
  keeper<T, U>::keeper(keeper&& moved) noexcept {
    acquire(moved);
  }

  template <class T, class U>
  keeper<T, U>& keeper<T, U>::operator=(keeper&& moved) noexcept {
    release();
    acquire(moved);
    return *this;
  }

  ///////////
  // Keep. //
  ///////////

  template <class T, class U>
  template <class V>
  void keeper<T, U>::foreach(V&& callable) {
    if (_kept) {
      _kept->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U>
  template <class V>
  void keeper<T, U>::foreach(V&& callable) const {
    if (_kept) {
      _kept->foreach(std::forward<V>(callable));
    }
  }

  template <class T, class U>
  void keeper<T, U>::acquire(keeper<T, U>& keeper) {
    _kept = keeper._kept;

    foreach([&](U& node) {
      node._keeper = static_cast<T*>(this);
    });

    keeper._kept = nullptr;
  }

  template <class T, class U>
  void keeper<T, U>::release() {
    foreach([&](U& node) {
      node._keeper = nullptr;
    });

    if (auto* node = _kept) {
      while (!node->alone()) {
        auto* next = &node->next();
        node->splice(node->next());
        node = next;
      }
    }

    _kept = nullptr;
  }

} // namespace hatch

#endif // HATCH_KEEPER_IMPL_HH