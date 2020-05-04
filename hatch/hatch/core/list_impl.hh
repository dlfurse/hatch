#ifndef HATCH_LIST_IMPL_HH
#define HATCH_LIST_IMPL_HH

#ifndef HATCH_LIST_HH
#error "do not include list_impl.hh directly. include list.hh instead."
#endif

namespace hatch {

  template <class T>
  successor<T>::successor(precessor<T>* prec) :
    _prec{prec} {
  }

  template <class T>
  precessor<T>* successor<T>::prec() {
    return _prec;
  }

  template <class T>
  precessor<T>::precessor(successor<T>* succ) :
      _succ{succ} {
  }

  template <class T>
  successor<T>* precessor<T>::succ() {
    return _succ;
  }

  template <class T>
  node<T>::node(hatch::precessor<T> *prec, hatch::successor<T> *succ) :
    successor<T>{prec},
    precessor<T>{succ} {
  }

  template <class T>
  bool node<T>::detach() {
    bool result = false;

    if (this->_prec) {
      result = true;
      this->_prec->_succ = this->_succ;
    }

    if (this->_succ) {
      result = true;
      this->_succ->_prec = this->_prec;
    }

    return result;
  }

  template <class T>
  bool node<T>::supplant(node<T>* node) {
    bool result = false;

    if (auto* prec = node->_prec) {
      result = true;
      prec->_succ = this;
      this->_prec = prec;
      node->_prec = nullptr;
    }

    if (auto* succ = node->_succ) {
      result = true;
      succ->_prec = this;
      this->_succ = succ;
      node->_succ = nullptr;
    }

    return result;
  }

  template <class T>
  bool node<T>::insert_after(precessor<T>* prec) {
    if (!prec) {
      return false;
    }

    if (auto* succ = prec->_succ) {
      succ->_prec = this;
      this->_succ = succ;
    }

    prec->_succ = this;
    this->_prec = prec;

    return true;
  }

  template <class T>
  bool node<T>::insert_before(successor<T>* succ) {
    if (!succ) {
      return false;
    }

    if (auto* prec = succ->_prec) {
      prec->_succ = this;
      this->_prec = prec;
    }

    succ->_prec = this;
    this->_succ = succ;

    return true;
  }
}

#endif // HATCH_LIST_IMPL_HH
