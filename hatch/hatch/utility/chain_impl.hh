#ifndef HATCH_CHAIN_IMPL_HH
#define HATCH_CHAIN_IMPL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_impl.hh directly. include chain.hh instead."
#endif

#include <hatch/utility/pointed.hh> // pointed

#include <utility> // move

namespace hatch {

  /**
   * Construction and assignment.
   */

  template <template <class, auto...> class R, auto ...A>
  chain<R, A...>::chain() :
      _prev{this},
      _next{_prev} {
  }

  template <template <class, auto...> class R, auto ...A>
  chain<R, A...>::~chain() {
    if (!alone()) {
      splice(next());
    }
  }

  template <template <class, auto...> class R, auto ...A>
  chain<R, A...>::chain(chain&& moved) noexcept :
      _prev{std::move(moved._prev)},
      _next{std::move(moved._next)} {
    auto self = R<chain, A...>{this};
    _prev->_next = self;
    _next->_prev = self;
    auto other = R<chain, A...>{&moved};
    moved._prev = other;
    moved._next = other;
  }

  template <template <class, auto...> class R, auto ...A>
  chain<R, A...>& chain<R, A...>::operator=(chain&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    auto self = R<chain, A...>{this};
    _prev->_next = self;
    _next->_prev = self;
    auto other = R<chain, A...>{&moved};
    moved._prev = other;
    moved._next = other;
    return *this;
  }

  /**
   * Accessors.
   */

  template <template <class, auto...> class R, auto ...A>
  bool chain<R, A...>::alone() const {
    const auto self = R<chain, A...>{const_cast<chain*>(this)};
    return _prev == self && _next == self;
  }

  template <template <class, auto...> class R, auto ...A>
  R<chain<R, A...>, A...> chain<R, A...>::prev() {
    return _prev;
  }

  template <template <class, auto...> class R, auto ...A>
  const R<chain<R, A...>, A...> chain<R, A...>::prev() const {
    return const_cast<chain*>(this)->prev();
  }

  template <template <class, auto...> class R, auto ...A>
  R<chain<R, A...>, A...> chain<R, A...>::next() {
    return _next;
  }

  template <template <class, auto...> class R, auto ...A>
  const R<chain<R, A...>, A...> chain<R, A...>::next() const {
    return const_cast<chain*>(this)->next();
  }

  /**
   * Mutators.
   */

  template <template <class, auto...> class R, auto ...A>
  void chain<R, A...>::splice(R<chain, A...> node) {
    chain* node_p{node()};
    R<chain, A...> node_prev_r{node_p->_prev};
    chain* node_prev_p{node_prev_r()};
    R<chain, A...> this_r{this};
    chain* this_prev_p{_prev()};


    this_prev_p->_next = node;
    node_p->_prev = _prev;

    node_prev_p->_next = this_r;
    this->_prev = node_prev_r;
  }

} // namespace hatch

#endif // HATCH_CHAIN_IMPL_HH
