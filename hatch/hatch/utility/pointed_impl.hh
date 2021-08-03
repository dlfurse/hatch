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
  pointed<T>& pointed<T>::operator=(pointed&& moved) noexcept {
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
  template <class U>
  pointed<T>::pointed(U* address) :
      _pointer{reinterpret_cast<T*>(address)} {
  }

  template <class T>
  template <class U>
  pointed<T>& pointed<T>::operator=(U* address) {
    _pointer = reinterpret_cast<T*>(address);
    return *this;
  }

  template <class T>
  pointed<T>::operator bool() const {
    return _pointer != nullptr;
  }

  template <class T>
  bool pointed<T>::operator==(const pointed& other) const {
    return _pointer == other._pointer;
  }

  template <class T>
  bool pointed<T>::operator!=(const pointed& other) const {
    return _pointer != other._pointer;
  }

  template <class T>
  T* pointed<T>::operator()() {
    return _pointer;
  }

  template <class T>
  const T* pointed<T>::operator()() const {
    return const_cast<pointed<T>&>(*this).operator->();
  }

  template <class T>
  T* pointed<T>::operator->() {
    return _pointer;
  }

  template <class T>
  const T* pointed<T>::operator->() const {
    return const_cast<pointed<T>&>(*this).operator->();
  }

  template <class T>
  T& pointed<T>::operator*() {
    return *_pointer;
  }

  template <class T>
  const T& pointed<T>::operator*() const {
    return const_cast<pointed<T>&>(*this).operator*();
  }

} // namespace hatch

#endif // HATCH_POINTED_IMPL_HH