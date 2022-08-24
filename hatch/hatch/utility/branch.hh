#ifndef HATCH_BRANCH_HH
#define HATCH_BRANCH_HH

#include <optional> // optional

#include <cstdint> // uint8_t

namespace hatch {

  class branch_detail {
  private:
    enum class colors : uint8_t {
      black = 0,
      red = 1,
    };

    enum class sides : uint8_t {
      root = 0,
      prev = 1,
      next = 2,
    };

  public:
    template <class T, template <class> class Ref>
    friend class branch;
    friend sides swap(sides side);
    friend colors swap(colors color);
  };

  template <class T, template <class> class Ref>
  class branch {
  ///////////////////////////////////////////
  // Constructors, destructor, assignment. //
  ///////////////////////////////////////////

  protected:
    branch();
    ~branch();

    branch(branch&& moved) noexcept;
    branch& operator=(branch&& moved) noexcept;

  public:
    branch(const branch&) = delete;
    branch& operator=(const branch&) = delete;

  ////////////
  // Color. //
  ////////////

  protected:
    using colors = typename branch_detail::colors;
    colors _color;

  protected:
    colors color() const;
    void make_color(colors color);

    bool is_red() const;
    void make_red();

    bool is_black() const;
    void make_black();

  ////////////////
  // Structure. //
  ////////////////

  protected:
    using sides = typename branch_detail::sides;
    sides _side;

    Ref<T> _head;
    Ref<T> _prev;
    Ref<T> _next;

    ////////////////
    // Accessors. //
    ////////////////

  protected:
    bool alone() const;
    sides side() const;

    Ref<T> head();
    const Ref<T> head() const;

    Ref<T> prev();
    const Ref<T> prev() const;

    Ref<T> next();
    const Ref<T> next() const;

    Ref<T> child(sides side);
    const Ref<T> child(sides side) const;

    Ref<T> root();
    const Ref<T> root() const;

    Ref<T> minimum();
    const Ref<T> minimum() const;

    Ref<T> maximum();
    const Ref<T> maximum() const;

    Ref<T> predecessor();
    const Ref<T> predecessor() const;

    Ref<T> successor();
    const Ref<T> successor() const;

    ///////////////
    // Mutators. //
    ///////////////

  protected:
    void make_head(Ref<T> new_head, sides side);
    void make_child(Ref<T> new_child, sides side);
    void make_next(Ref<T> new_next);
    void make_prev(Ref<T> new_prev);

    void detach();
    void rotate(sides side);
    void exchange(Ref<T> node);

    void insert(branch& node);
    void remove();
  };

} // namespace hatch

#include <hatch/utility/branch_impl.hh>

#endif // HATCH_BRANCH_HH