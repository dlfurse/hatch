#ifndef HATCH_BRANCH_DECL_HH
#define HATCH_BRANCH_DECL_HH

#ifndef HATCH_BRANCH_HH
#error "do not include branch_decl.hh directly. include branch.hh instead."
#endif

#include <cstdint> // uint8_t

namespace hatch {

  template <template <class, auto ...> class R, auto ...A>
  class branch {
  private:
    enum class colors_t : uint8_t {
      black = 0,
      red = 1,
    };

    enum class sides_t : uint8_t {
      root = 0,
      prev = 1,
      next = 2,
    };

    static sides_t swap(sides_t side);
    static colors_t swap(colors_t color);

  public:
    using ref_t = R<branch, A...>;

    /**
     * Constructors, destructor, assignment.
     */

  protected:
    branch();
    ~branch();

    branch(branch&& moved) noexcept;
    branch& operator=(branch&& moved) noexcept;

  public:
    branch(const branch&) = delete;
    branch& operator=(const branch&) = delete;

    /**
     * Color.
     */

  protected:
    colors_t _color;

  protected:
    colors_t color() const;
    void make_color(colors_t color);

    bool is_red() const;
    void make_red();

    bool is_black() const;
    void make_black();

    /**
     * Structure.
     */

  protected:
    sides_t _side;

    ref_t _head;
    ref_t _prev;
    ref_t _next;

    /**
     * Accessors.
     */

  protected:
    bool alone() const;
    sides_t side() const;

    ref_t head();
    const ref_t head() const;

    ref_t prev();
    const ref_t prev() const;

    ref_t next();
    const ref_t next() const;

    ref_t child(sides_t side);
    const ref_t child(sides_t side) const;

    ref_t root();
    const ref_t root() const;

    ref_t minimum();
    const ref_t minimum() const;

    ref_t maximum();
    const ref_t maximum() const;

    ref_t predecessor();
    const ref_t predecessor() const;

    ref_t successor();
    const ref_t successor() const;

    /**
     * Mutators.
     */

  protected:
    void make_head(ref_t new_head, sides_t side);
    void make_child(ref_t new_child, sides_t side);
    void make_next(ref_t new_next);
    void make_prev(ref_t new_prev);

    void detach();
    void rotate(sides_t side);
    void exchange(ref_t node);

    void insert(ref_t node);
    void remove();
  };

} // namespace hatch

#endif // HATCH_BRANCH_DECL_HH