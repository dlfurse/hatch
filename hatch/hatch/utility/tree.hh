#ifndef HATCH_TREE_HH
#define HATCH_TREE_HH

#include <hatch/utility/tree_fwd.hh>

#include <hatch/utility/indexed.hh>
#include <hatch/utility/pointed.hh>
#include <hatch/utility/owning.hh>

namespace hatch {

  template <class T, template <class> class Ref>
  class tree final : public owner<tree<T, Ref>, tree_iterator<T, Ref>> {
  public:
    friend class tree_iterator<T, Ref>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    explicit tree(Ref<tree_node<T, Ref>> root, void* data);

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
    tree_iterator<T, Ref> begin();
    const tree_iterator<T, Ref> begin() const;

    tree_iterator<T, Ref> end();
    const tree_iterator<T, Ref> end() const;

    tree_iterator<T, Ref> find(const tree_node<T, Ref>& node);
    const tree_iterator<T, Ref> find(const tree_node<T, Ref>& node) const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    Ref<tree_node<T, Ref>> _root;
    void* _data;

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
    tree_iterator<T, Ref> insert(tree_node<T, Ref>& node);
  };

  template <class T>
  class tree<T, pointed> final : public owner<tree<T, pointed>, tree_iterator<T, pointed>> {
  public:
    friend class tree_iterator<T, pointed>;

    ///////////////////////////////////////////
    // Constructors, destructor, assignment. //
    ///////////////////////////////////////////

  protected:
    explicit tree(pointed<tree_node<T, pointed>> root);

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
    tree_iterator<T, pointed> begin();
    const tree_iterator<T, pointed> begin() const;

    tree_iterator<T, pointed> end();
    const tree_iterator<T, pointed> end() const;

    tree_iterator<T, pointed> find(const tree_node<T, pointed>& node);
    const tree_iterator<T, pointed> find(const tree_node<T, pointed>& node) const;

    ////////////////
    // Structure. //
    ////////////////

  private:
    pointed<tree_node<T, pointed>> _root;

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
    tree_iterator<T, pointed> insert(tree_node<T, pointed>& node);
  };

} // namespace hatch

#include <hatch/utility/tree_node.hh>
#include <hatch/utility/tree_iterator.hh>

#include <hatch/utility/tree_impl.hh>
#include <hatch/utility/tree_node_impl.hh>
#include <hatch/utility/tree_iterator_impl.hh>

#endif
