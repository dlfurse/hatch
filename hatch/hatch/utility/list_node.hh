#ifndef HATCH_LIST_NODE_HH
#define HATCH_LIST_NODE_HH

#ifndef HATCH_LIST_HH
#error "do not include list_node.hh directly. include list.hh instead."
#endif

#include <hatch/utility/container.hh>
#include <hatch/utility/keep.hh>

namespace hatch {

  template <class T>
  class list_node : public container<T>, public keeper<list_node<T>, list_iterator<T>> {
  public:
    friend class list<T>;
    friend class list_iterator<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    list_node();
    ~list_node();

  public:
    list_node(list_node&& moved) noexcept;
    list_node& operator=(list_node&& moved) noexcept;

    list_node(const list_node&) = delete;
    list_node& operator=(const list_node&) = delete;

    ////////////////
    // Structure. //
    ////////////////

  private:
    list_node* _prev;
    list_node* _next;

    //////////////////////////
    // Structure: accessors //
    //////////////////////////

  public:
    bool alone() const;

    list_node& prev();
    const list_node& prev() const;

    list_node& next();
    const list_node& next() const;

    /////////////////////////
    // Structure: mutators //
    /////////////////////////

  protected:
    void splice(list_node& node);
  };

} // namespace hatch

#endif // HATCH_LIST_NODE_HH
