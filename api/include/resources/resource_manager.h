
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <ranges>
#include <vector>

#include "resource.h"

namespace api::resource {
class ResourceManager {
  std::vector<Resource> resources_;

 public:
  void LoadResources(Resource::Type type, const std::vector<int>& indices,
                     void (*on_chop_event)(int, float));

  auto resources(const Resource::Type type) {
    return resources_ | std::ranges::views::filter([type](const Resource& t) {
             return t.type() == type;
           });
  }
  auto resources(const Resource::Type type) const {
    return resources_ | std::ranges::views::filter([type](const Resource& t) {
             return t.type() == type;
           });
  }
};

inline void ResourceManager::LoadResources(const Resource::Type type,
                                           const std::vector<int>& indices,
                                           void (*on_chop_event)(int, float)) {
  for (auto& index : indices) {
    resources_.emplace_back();
    resources_.back().set_type(type);
    resources_.back().set_index(index);
    resources_.back().set_quantity(10);
    resources_.back().on_chop_resource_ = on_chop_event;
  }
}

}  // namespace api::resource
#endif
