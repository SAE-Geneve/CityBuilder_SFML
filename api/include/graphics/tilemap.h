#ifndef API_GRAPHICS_TILEMAP_H
#define API_GRAPHICS_TILEMAP_H

#include <SFML/Graphics.hpp>

#include "assets/asset_manager.h"
#include "ui/clickable.h"

constexpr int kWidth = 1600;
constexpr int kHeight = 1280;
constexpr int kPixelStep = 64;


class TileMap : public api::ui::Clickable {

public:
    enum class Tile {
        kEmpty, kBg, kWater,
        kRock, kTree, kFood,
        kCutTree,
        kLength
    };

private:
	std::array<std::string_view, static_cast<size_t>(Tile::kLength)> files_ = {
		"empty.png", "bg_tile_a.png", "water.png",
		"bg_plus_rock.png","bg_plus_tree.png", "bg_plus_food.png",
		"scifiEnvironment_15.png"
    };

    std::array<Tile, kWidth / kPixelStep * kHeight / kPixelStep> tiles_ = {};
    core::experimental::AssetManager<sf::Texture, Tile, "_assets/sprites"> textures_;

    std::vector<sf::Vector2f> walkables_;

    static int Index(sf::Vector2f screenPosition);

public:
	static sf::Vector2f ScreenPosition(int index);
	static sf::Vector2f TilePos(sf::Vector2i);
	static int GetStep(){return kPixelStep;};

    void Setup();
    void Draw(sf::RenderWindow &window);
	void SetTile(int, Tile);

    std::vector<sf::Vector2f> &GetWalkables();
    std::vector<int> GetCollectibles(Tile);

};
#endif
