#ifndef CORE_ASSETS_ASSET_MANAGER_H_
#define CORE_ASSETS_ASSET_MANAGER_H_

#include <array>
#include <span>


#include "profiling/profiling.h"
#include "utils/string_literal.h"

namespace core::assets {



template <typename TAsset, typename TAssetType, utils::StringLiteral folderPath>
  requires std::is_enum_v<TAssetType> && requires {
    { static_cast<size_t>(TAssetType::kLength) };
  }
class AssetManager {
 public:
  void Load(std::span<const std::string_view> assetsPaths) {
    PROFILE_ZONE();
    for (size_t i = 0; i < assets_.size(); ++i) {
      assets_[i] = TAsset(std::format("{}/{}", folderPath.str, assetsPaths[i]));
    }
  }

  const TAsset &Get(TAssetType asset_index) {
    const auto index = static_cast<size_t>(asset_index);
    if (index < 0 || index >= std::size(assets_)) {
      throw std::out_of_range("Invalid index");
    }
    return assets_[index];
  }

 private:
  std::array<TAsset, static_cast<size_t>(TAssetType::kLength)> assets_{};
};

}  // namespace core::assets

#endif  // CORE_ASSETS_ASSET_MANAGER_H_
