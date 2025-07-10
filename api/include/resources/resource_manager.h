
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <vector>

#include "graphics/tilemap.h"
#include "resource.h"

class ResourceManager {
    std::vector<Resource> resources_;

public:
    void LoadResources(
        Resource::Type type,
        const std::vector<int>& indexes,
        std::function<void(int, float)> OnChopEvent);

    [[nodiscard]] std::vector<Resource> GetResources(Resource::Type type) const;
};

inline void ResourceManager::LoadResources(
    const Resource::Type type,
    const std::vector<int>& indexes,
    std::function<void(int, float)> OnChopEvent) {

    for (auto& index : indexes) {
        resources_.emplace_back();
        resources_.back().SetType(type);
        resources_.back().SetIndex(index);
        resources_.back().SetQuantity(10);
        resources_.back().OnChopResource_ = OnChopEvent;
    }

}

inline std::vector<Resource> ResourceManager::GetResources(const Resource::Type type) const{
    std::vector<Resource> resources_of_type = {};

    for (const auto& resource : resources_) {
        if (resource.GetType() == type) {
            resources_of_type.emplace_back(resource);
        }
    }
    return resources_of_type;
}

#endif
