#ifndef HATCH_TREE_HH
#define HATCH_TREE_HH

#include <hatch/utility/tree_fwd.hh>

namespace hatch {

  template <class T>
  class tree {
  public:
    friend class tree_iterator<T>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    explicit tree(tree_node<T>* root);

  public:
    tree();
    ~tree();

    tree(tree&& moved) noexcept;
    tree& operator=(tree&& moved) noexcept;

    tree(const tree&) = delete;
    tree& operator=(const tree&) = delete;

    ////////////////
    // Iterators. //
    ////////////////

  public:
    tree_iterator<T> begin();
    const tree_iterator<T> begin() const;

    tree_iterator<T> end();
    const tree_iterator<T> end() const;

    tree_iterator<T> find(const tree_node<T>& node);
    const tree_iterator<T> find(const tree_node<T>& node) const;


    ////////////////
    // Structure. //
    ////////////////

  private:
    tree_node<T>* _root;

    //////////////////////////
    // Structure: accessors //
    //////////////////////////

  public:
    bool empty() const;
    T* root() const;
    T* minimum() const;
    T* maximum() const;

    /////////////////////////
    // Structure: mutators //
    /////////////////////////

  public:
    tree_iterator<T> insert(tree_node<T>& node);
  };

} // namespace hatch

#include <hatch/utility/tree_node.hh>
#include <hatch/utility/tree_iterator.hh>

#include <hatch/utility/tree_impl.hh>
#include <hatch/utility/tree_node_impl.hh>
#include <hatch/utility/tree_iterator_impl.hh>

#endif
