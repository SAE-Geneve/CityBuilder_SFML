#include "resources/resource.h"

#include <algorithm>

namespace api::resource {
void Resource::Exploit(const float rate) {
  quantity_ -= rate;
  quantity_ = std::max<float>(quantity_, 0);

  if (on_chop_resource_) {
    on_chop_resource_(tile_index_, quantity_);
  }
}
}