//
// Created by unite on 07.05.2026.
//

#include "resources/resource.h"

namespace api::resource {
void Resource::Exploit(const float rate) {
  quantity_ -= rate;
  quantity_ = std::max<float>(quantity_, 0);

  if (on_chop_resource_) {
    on_chop_resource_(tile_index_, quantity_);
  }
}
}