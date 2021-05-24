#ifndef HATCH_LIBERATED_HH
#define HATCH_LIBERATED_HH

#ifndef HATCH_MEMORY_HH
#error "do not include liberated.hh directly. include memory.hh instead."
#endif

#include <hatch/utility/tree.hh>

namespace hatch {

  template <uint64_t S>
  class liberated : public tree_node<liberated<S>> {
  public:
    friend class allocator;

  protected:
    liberated();
    ~liberated();

    liberated(liberated&& moved) noexcept;
    liberated& operator=(liberated&& moved) noexcept;

  public:
    liberated(const liberated& copied) = delete;
    liberated& operator=(const liberated& copied) = delete;

  public:
    bool operator<(const liberated& other);
    bool operator>(const liberated& other);
  };

} // namespace hatch

#endif // HATCH_LIBERATED_HH