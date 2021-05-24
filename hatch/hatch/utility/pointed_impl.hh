#ifndef HATCH_POINTED_IMPL_HH
#define HATCH_POINTED_IMPL_HH

#ifndef HATCH_POINTED_HH
#error "do not include pointed_impl.hh directly.  include pointed.hh instead."
#endif

namespace hatch {

  template <class T>
  pointed<T>::pointed() :
      _pointer{nullptr} {
  }

  template <class T>
  pointed<T>::~pointed() {
  }

  template <class T>
  pointed<T>::pointed(pointed&& moved) noexcept :
      _pointer{moved._pointer} {
    moved._pointer = nullptr;
  }

  template <class T>
  pointed<T>& pointed<T>::operator=(pointed&& moved) {
    _pointer = moved._pointer;
    moved._pointer = nullptr;
    return *this;
  }

  template <class T>
  pointed<T>::pointed(const pointed& copied) :
      _pointer{copied._pointer} {
  }

  template <class T>
  pointed<T>& pointed<T>::operator=(const pointed& copied) {
    _pointer = copied._pointer;
    return *this;
  }

  template <class T>
  pointed<T>::pointed(T* address) :
      _pointer{address} {
  }

  template <class T>
  pointed<T>& pointed<T>::operator=(T* address) {
    _pointer = address;
    return *this;
  }

  template <class T>
  pointed<T>::operator bool() const {
    return _pointer != nullptr;
  }

  template <class T>
  T& pointed<T>::operator*(datatype) {
    return *_pointer;
  }

  template <class T>
  const T& pointed<T>::operator*(datatype) const {
    return const_cast<pointed<T>&>(*this).operator*();
  }

  template <class T>
  T* pointed<T>::operator->(datatype) {
    return _pointer;
  }

  template <class T>
  const T* pointed<T>::operator->(datatype) const {
    return const_cast<pointed<T>&>(*this).operator->();
  }

} // namespace hatch

#endif // HATCH_POINTED_IMPL_HH