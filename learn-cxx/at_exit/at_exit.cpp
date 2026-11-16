#include "at_exit.hpp"

namespace base::detail {

template class BaseExitManager<true>;
template class BaseExitManager<false>;

} // namespace base::detail
