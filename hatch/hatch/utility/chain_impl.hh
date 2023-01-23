#ifndef HATCH_CHAIN_IMPL_HH
#define HATCH_CHAIN_IMPL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_impl.hh directly. include chain.hh instead."
#endif

#include <utility> // move

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T, template <class...> class Ref, class ...Args>
  chain<Ref<T, Args...>>::chain() :
      _prev{static_cast<T*>(this)},
      _next{_prev} {
  }

  template <class T, template <class...> class Ref, class ...Args>
  chain<Ref<T, Args...>>::~chain() {
    if (!alone()) {
      splice(next());
    }
  }

  template <class T, template <class...> class Ref, class ...Args>
  chain<Ref<T, Args...>>::chain(chain&& moved) noexcept :
      _prev{std::move(moved._prev)},
      _next{std::move(moved._next)} {
    auto self = Ref<T, Args...>{static_cast<T*>(this)};
    _prev->_next = self;
    _next->_prev = self;
    auto other = Ref<T, Args...>{static_cast<T*>(&moved)};
    moved._prev = other;
    moved._next = other;
  }

  template <class T, template <class...> class Ref, class ...Args>
  chain<Ref<T, Args...>>& chain<Ref<T, Args...>>::operator=(chain&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    auto self = Ref<T>{static_cast<T*>(this)};
    _prev->_next = self;
    _next->_prev = self;
    auto other = Ref<T>{static_cast<T*>(&moved)};
    moved._prev = other;
    moved._next = other;
    return *this;
  }

  ////////////////
  // Accessors. //
  ////////////////

  template <class T, template <class> class Ref>
  bool chain<T, Ref>::alone() const {
    const auto self = Ref<T>{static_cast<T*>(const_cast<chain<T, Ref>*>(this))};
    return _prev == self && _next == self;
  }

  template <class T, template <class> class Ref>
  Ref<T> chain<T, Ref>::prev() {
    return _prev;
  }

  template <class T, template <class> class Ref>
  const Ref<T> chain<T, Ref>::prev() const {
    return const_cast<chain<T, Ref>*>(this)->prev();
  }

  template <class T, template <class> class Ref>
  Ref<T> chain<T, Ref>::next() {
    return _next;
  }

  template <class T, template <class> class Ref>
  const Ref<T> chain<T, Ref>::next() const {
    return const_cast<chain<T, Ref>*>(this)->next();
  }

  ///////////////
  // Mutators. //
  ///////////////

  template <class T, template <class> class Ref>
  void chain<T, Ref>::splice(Ref<T> node) {
    auto self = Ref<T>{static_cast<T*>(this)};
    auto prev = node._prev;
    auto next = node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = self;
  }

  ///////////////////
  // Accumulators. //
  ///////////////////

  template <class T, template <class> class Ref>
  template <class U>
  void chain<T, Ref>::foreach(U&& callable) {
    const auto self = Ref<T>(this);
    auto node = self;
    do {
      callable(*node);
      node = node->_next;
    } while (node != self);
  }

  template <class T, template <class> class Ref>
  template <class U>
  void chain<T, Ref>::foreach(U&& callable) const {
    const auto self = Ref<T>(this);
    auto node = self;
    do {
      callable(*node);
      node = node->_next;
    } while (node != self);
  }

} // namespace hatch

#endif // HATCH_CHAIN_IMPL_HH
