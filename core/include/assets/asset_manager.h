#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <vector>

/* TODO: AssetManager
1 - can handle every type of media : sf::Texture, sf::soundBuffer, sf::Font, etc..
2 - can be used everywhere in the programm at Api or Game level
3 - is independant of the context where it is used
4 - no double usage, one file, one entry in the assets_ collection
*/
template<typename TAsset>
class AssetManager {
    /* TODO: Code smell : dependant of the context,
    move the enum to tilemap and use another system (id, string)
    */
public:
    enum class TextureIndex {
        kDefault = 0,
        KGrass,
        KMaison,
    };

private:
    std::string folder_;
    /* TODO: choose a different data structure
    1 - to have a different id for each file
    2 - to avoid double entries : (each file stored is unique, one id = one file)
    */
    std::vector<TAsset> assets_;
    TAsset default_asset_;

public:
    explicit AssetManager(std::string folder);

    /* TODO: Do we need to load the whole folder, or could it be better to load each file one by one
    Example Load(id, file_path)
    */
    void Load();

    const TAsset &Get(TextureIndex index);
};

template<typename TAsset>
AssetManager<TAsset>::AssetManager(std::string folder) : folder_(folder){
}

template<typename TAsset>
void AssetManager<TAsset>::Load(){
    // TODO: Check if folder exists or not
    if (!std::filesystem::exists(folder_)) {
        return;
    }

    assets_.emplace_back(TAsset(folder_ + "empty.png"));
    assets_.emplace_back(TAsset(folder_ + "grass.png"));
    assets_.emplace_back(TAsset(folder_ + "maison.png"));

}

template<typename TAsset>
const TAsset &AssetManager<TAsset>::Get(TextureIndex textIndex){

    int index = static_cast<int>(textIndex);
    if (index < 0 || index >= assets_.size()) {
        return default_asset_;
    }

    return assets_[index];
}


#endif
