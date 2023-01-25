#ifndef HATCH_CHAIN_IMPL_HH
#define HATCH_CHAIN_IMPL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_impl.hh directly. include chain.hh instead."
#endif

#include <utility> // move

namespace hatch {

  /**
   * Construction and assignment.
   */

  template <template <class> class Ref>
  chain<Ref>::chain() :
      _prev{this},
      _next{_prev} {
  }

  template <template <class> class Ref>
  chain<Ref>::~chain() {
    if (!alone()) {
      splice(next());
    }
  }

  template <template <class> class Ref>
  chain<Ref>::chain(chain&& moved) noexcept :
      _prev{std::move(moved._prev)},
      _next{std::move(moved._next)} {
    auto self = Ref<chain>{this};
    _prev->_next = self;
    _next->_prev = self;
    auto other = Ref<chain>{&moved};
    moved._prev = other;
    moved._next = other;
  }

  template <template <class> class Ref>
  chain<Ref>& chain<Ref>::operator=(chain&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    auto self = Ref<chain>{this};
    _prev->_next = self;
    _next->_prev = self;
    auto other = Ref<chain>{&moved};
    moved._prev = other;
    moved._next = other;
    return *this;
  }

  /**
   * Accessors.
   */

  template <template <class> class Ref>
  bool chain<Ref>::alone() const {
    const auto self = Ref<chain>{const_cast<chain<Ref>*>(this)};
    return _prev == self && _next == self;
  }

  template <template <class> class Ref>
  Ref<chain<Ref>> chain<Ref>::prev() {
    return _prev;
  }

  template <template <class> class Ref>
  const Ref<chain<Ref>> chain<Ref>::prev() const {
    return const_cast<chain<Ref>*>(this)->prev();
  }

  template <template <class> class Ref>
  Ref<chain<Ref>> chain<Ref>::next() {
    return _next;
  }

  template <template <class> class Ref>
  const Ref<chain<Ref>> chain<Ref>::next() const {
    return const_cast<chain<Ref>*>(this)->next();
  }

  /**
   * Mutators.
   */

  template <template <class> class Ref>
  void chain<Ref>::splice(Ref<chain> node) {
    auto self = Ref<chain>{this};

    auto prev = node->_prev;
    auto next = node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = self;
  }

} // namespace hatch

#endif // HATCH_CHAIN_IMPL_HH
