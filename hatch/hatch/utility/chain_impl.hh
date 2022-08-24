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

  template <class T, template <class> class Ref>
  chain<T, Ref>::chain() :
      _self{static_cast<T*>(this)},
      _prev{_self},
      _next{_self} {
  }

  template <class T, template <class> class Ref>
  chain<T, Ref>::~chain() {
    static_assert(std::is_base_of_v<chain<T, Ref>, T>);
    if (!alone()) {
      splice(next());
    }
  }

  template <class T, template <class> class Ref>
  chain<T, Ref>::chain(chain&& moved) noexcept :
      _self{std::move(moved._self)},
      _prev{std::move(moved._prev)},
      _next{std::move(moved._next)} {
    _prev->_next = _self;
    _next->_prev = _self;
    moved._prev = static_cast<T*>(&moved);
    moved._next = static_cast<T*>(&moved);
  }

  template <class T, template <class> class Ref>
  chain<T, Ref>& chain<T, Ref>::operator=(chain&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    _prev->_next = static_cast<T*>(this);
    _next->_prev = static_cast<T*>(this);
    moved._prev = static_cast<T*>(&moved);
    moved._next = static_cast<T*>(&moved);
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
    auto prev = node._prev;
    auto next = node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = Ref<T>{this};
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
