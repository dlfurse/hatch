#ifndef HATCH_INDEXED_IMPL_HH
#define HATCH_INDEXED_IMPL_HH

#ifndef HATCH_INDEXED_HH
#error "do not include indexed_impl.hh directly. include indexed.hh instead."
#endif

namespace hatch {

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  template <class U>
  indexed<T, Width, Stride, Offset>::context::context(U* address) {
    indexed::_context = reinterpret_cast<std::byte*>(address);
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::context::~context() {
    indexed::_context = nullptr;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  __thread std::byte* indexed<T, Width, Stride, Offset>::_context = nullptr;

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::indexed() :
      _index{null} {
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::~indexed() {
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::indexed(indexed&& moved) noexcept :
      _index{moved._index} {
    moved._index = null;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>& indexed<T, Width, Stride, Offset>::operator=(indexed&& moved) noexcept {
    _index = moved._index;
    moved._index = null;
    return *this;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::indexed(const indexed& copied) :
      _index{copied._index} {
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>& indexed<T, Width, Stride, Offset>::operator=(const indexed& copied) {
    _index = copied._index;
    return *this;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  template <class U>
  indexed<T, Width, Stride, Offset>::indexed(U* address) :
      _index{address ? (index)((reinterpret_cast<std::byte*>(address) - _context - Offset)/(Stride)) : null} {
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  template <class U>
  indexed<T, Width, Stride, Offset>& indexed<T, Width, Stride, Offset>::operator=(U* address) {
    _index = address ? (index)(reinterpret_cast<std::byte*>(address) - _context - Offset)/(Stride) : null;
    return *this;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  indexed<T, Width, Stride, Offset>::operator bool() const {
    return _index != null;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  bool indexed<T, Width, Stride, Offset>::operator==(const indexed& other) const {
    return _index == other._index;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  bool indexed<T, Width, Stride, Offset>::operator!=(const indexed& other) const {
    return _index != other._index;
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  T* indexed<T, Width, Stride, Offset>::operator()() {
    return reinterpret_cast<T*>(_context + _index * Stride + Offset);
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  const T* indexed<T, Width, Stride, Offset>::operator()() const {
    return const_cast<indexed<T, Width, Stride, Offset>&>(*this).operator->();
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  T* indexed<T, Width, Stride, Offset>::operator->() {
    return reinterpret_cast<T*>(_context + _index * Stride + Offset);
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  const T* indexed<T, Width, Stride, Offset>::operator->() const {
    return const_cast<indexed<T, Width, Stride, Offset>&>(*this).operator->();
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  T& indexed<T, Width, Stride, Offset>::operator*() {
    return *reinterpret_cast<T*>(_context + _index * Stride + Offset);
  }

  template <class T, widths Width, unsigned_t<Width> Stride, unsigned_t<Width> Offset>
  const T& indexed<T, Width, Stride, Offset>::operator*() const {
    return const_cast<indexed<T, Width, Stride, Offset>&>(*this).operator*();
  }

} // namespace hatch

#endif // HATCH_INDEXED_IMPL_HH