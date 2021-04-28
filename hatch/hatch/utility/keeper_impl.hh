#ifndef HATCH_KEEPER_IMPL_HH
#define HATCH_KEEPER_IMPL_HH

#ifndef HATCH_KEEP_HH
#error "do not include keeper_impl.hh directly. include keep.hh instead."
#endif

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
  }

  ///////////
  // Keep. //
  ///////////

  template <class T, class U>
  template <class F>
  void keeper<T, U>::foreach(F f) {
    if (auto* node = _kept) {
      do {
        auto* next = static_cast<U*>(node->_next);

        f(*node);

        node = next;
      } while (node != _kept);
    }
  }

  template <class T, class U>
  void keeper<T, U>::acquire(keeper<T, U>& keeper) {
    _kept = keeper._kept;
    foreach([&](U& node) {
      node._keeper = this;
    });
    keeper._kept = nullptr;
  }

  template <class T, class U>
  void keeper<T, U>::release() {
    foreach([&](U& node) {
      node._keeper = nullptr;
      node._prev = &node;
      node._next = &node;
    });
    _kept = nullptr;
  }

} // namespace hatch

#endif // HATCH_KEEPER_IMPL_HH