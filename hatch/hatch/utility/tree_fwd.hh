#ifndef HATCH_TREE_FWD_HH
#define HATCH_TREE_FWD_HH

#include <hatch/utility/pointed.hh>

namespace hatch {

  template <class T, template <class> class Ref = pointed>
  class tree;

  template <class T, template <class> class Ref = pointed>
  class tree_node;

  template <class T, template <class> class Ref = pointed>
  class tree_iterator;

} // namespace hatch

#endif // HATCH_TREE_FWD_HH

