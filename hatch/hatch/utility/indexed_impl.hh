#ifndef HATCH_INDEXED_IMPL_HH
#define HATCH_INDEXED_IMPL_HH

#ifndef HATCH_INDEXED_HH
#error "do not include indexed_impl.hh directly. include indexed.hh instead."
#endif

namespace hatch {

  template <class T, widths W, size_t S, size_t O>
  template <class U>
  indexed<T, W, S, O>::context::context(U* address) {
    _context = reinterpret_cast<std::byte*>(address);
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::context::~context() {
    _context = nullptr;
  }

  template <class T, widths W, size_t S, size_t O>
  __thread std::byte* indexed<T, W, S, O>::_context = nullptr;

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::indexed() :
      _index{null} {
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::~indexed() {
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::indexed(indexed&& moved) noexcept :
      _index{moved._index} {
    moved._index = null;
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>& indexed<T, W, S, O>::operator=(indexed&& moved) noexcept {
    _index = moved._index;
    moved._index = null;
    return *this;
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::indexed(const indexed& copied) :
      _index{copied._index} {
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>& indexed<T, W, S, O>::operator=(const indexed& copied) {
    _index = copied._index;
    return *this;
  }

  template <class T, widths W, size_t S, size_t O>
  template <class U>
  indexed<T, W, S, O>::indexed(U* address) :
      _index{address ? (index)((reinterpret_cast<std::byte*>(address) - _context - O)/(S)) : null} {
  }

  template <class T, widths W, size_t S, size_t O>
  template <class U>
  indexed<T, W, S, O>& indexed<T, W, S, O>::operator=(U* address) {
    _index = address ? (index)(reinterpret_cast<std::byte*>(address) - _context - O)/(S) : null;
    return *this;
  }

  template <class T, widths W, size_t S, size_t O>
  indexed<T, W, S, O>::operator bool() const {
    return _index != null;
  }

  template <class T, widths W, size_t S, size_t O>
  bool indexed<T, W, S, O>::operator==(const indexed& other) const {
    return _index == other._index;
  }

  template <class T, widths W, size_t S, size_t O>
  bool indexed<T, W, S, O>::operator!=(const indexed& other) const {
    return _index != other._index;
  }

  template <class T, widths W, size_t S, size_t O>
  T* indexed<T, W, S, O>::operator()() {
    return _index == null ? nullptr : reinterpret_cast<T*>(_context + _index * S + O);
  }

  template <class T, widths W, size_t S, size_t O>
  const T* indexed<T, W, S, O>::operator()() const {
    return const_cast<indexed&>(*this).operator()();
  }

  template <class T, widths W, size_t S, size_t O>
  T* indexed<T, W, S, O>::operator->() {
    return _index == null ? nullptr : reinterpret_cast<T*>(_context + _index * S + O);
  }

  template <class T, widths W, size_t S, size_t O>
  const T* indexed<T, W, S, O>::operator->() const {
    return const_cast<indexed&>(*this).operator->();
  }

  template <class T, widths W, size_t S, size_t O>
  T& indexed<T, W, S, O>::operator*() {
    return _index == null ? *reinterpret_cast<T*>(nullptr) : *reinterpret_cast<T*>(_context + _index * S + O);
  }

  template <class T, widths W, size_t S, size_t O>
  const T& indexed<T, W, S, O>::operator*() const {
    return const_cast<indexed&>(*this).operator*();
  }

} // namespace hatch

#endif // HATCH_INDEXED_IMPL_HH