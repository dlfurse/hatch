#ifndef HATCH_CHAIN_IMPL_HH
#define HATCH_CHAIN_IMPL_HH

#ifndef HATCH_CHAIN_HH
#error "do not include chain_impl.hh directly. include chain.hh instead."
#endif

namespace hatch {

  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  template <class T>
  chain<T>::chain() :
      _prev{static_cast<T*>(this)},
      _next{static_cast<T*>(this)} {
  }

  template <class T>
  chain<T>::~chain() {
    if (!alone()) {
      splice(next());
    }
  }

  template <class T>
  chain<T>::chain(T&& moved) noexcept :
      _prev{moved._prev},
      _next{moved._next} {
    _prev->_next = static_cast<T*>(this);
    _next->_prev = static_cast<T*>(this);
    moved._prev = &moved;
    moved._next = &moved;
  }

  template <class T>
  chain<T>& chain<T>::operator=(T&& moved) noexcept {
    _prev = moved._prev;
    _next = moved._next;
    _prev->_next = static_cast<T*>(this);
    _next->_prev = static_cast<T*>(this);
    moved._prev = &moved;
    moved._next = &moved;
    return *this;
  }

  ////////////
  // Chain. //
  ////////////

  template <class T>
  bool chain<T>::alone() const {
    return _prev == this && _next == this;
  }

  template <class T>
  T& chain<T>::prev() {
    return *_prev;
  }

  template <class T>
  const T& chain<T>::prev() const {
    return *_prev;
  }

  template <class T>
  T& chain<T>::next() {
    return *_next;
  }

  template <class T>
  const T& chain<T>::next() const {
    return *_next;
  }

  template <class T>
  void chain<T>::splice(T& node) {
    auto prev = node._prev;
    auto next = &node;

    _prev->_next = next;
    next->_prev = _prev;

    this->_prev = prev;
    prev->_next = static_cast<T*>(this);
  }

  template <class T>
  template <class U>
  void chain<T>::foreach(U&& callable) {
    auto* node = static_cast<T*>(this);
    do {
      auto* next = &node->next();
      callable(*node);
      node = next;
    } while (node != static_cast<T*>(this));
  }

  template <class T>
  template <class U>
  void chain<T>::foreach(U&& callable) const {
    auto* node = static_cast<const T*>(this);
    do {
      auto* next = &node->next();
      callable(*node);
      node = next;
    } while (node != static_cast<const T*>(this));
  }

} // namespace hatch

#endif // HATCH_CHAIN_IMPL_HH
