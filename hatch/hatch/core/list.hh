#ifndef HATCH_LIST_HH
#define HATCH_LIST_HH

namespace hatch {

  template <class T>
  class node;

  template <class T>
  class successor;

  template <class T>
  class precessor;

  template <class T>
  class successor {
  protected:
    explicit successor(precessor<T>* prec = nullptr);
    precessor<T>* _prec{nullptr};

  public:
    friend class node<T>;
    precessor<T>* prec();
  };

  template <class T>
  class precessor {
  protected:
    explicit precessor(successor<T>* succ = nullptr);
    successor<T>* _succ{nullptr};

  public:
    friend class node<T>;
    successor<T>* succ();
  };

  template <class T>
  class node : public successor<T>, public precessor<T>  {
  public:
    explicit node(precessor<T>* prec = nullptr, successor<T>* succ = nullptr);

  public:
    bool detach();
    bool supplant(node<T>* node);
    bool insert_after(precessor<T>* prec);
    bool insert_before(successor<T>* succ);
  };

}

#include <hatch/core/list_impl.hh>

#endif // HATCH_LIST_HH
