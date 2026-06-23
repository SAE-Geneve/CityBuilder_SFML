//
// Created by sebas on 23.06.2026.
//

#ifndef CITYBUILDER_BUTTON_BUILDER_H
#define CITYBUILDER_BUTTON_BUILDER_H
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "button.h"
#include "clickable.h"


namespace api::ui {

    class ButtonBuilder {
        Button product;

    public:
        ButtonBuilder &New();
        ButtonBuilder &WithPosition(sf::Vector2f pos, sf::Vector2f vertexSize);
        ButtonBuilder &WithText(std::string text);

        ButtonBuilder &WithBaseTile(sf::FloatRect baseTiling);
        ButtonBuilder &WithHoverTile(sf::FloatRect baseTiling);
        ButtonBuilder &WithClickTile(sf::FloatRect clickTiling);

        ButtonBuilder &WithHoverCallback(const UICallback &hCallback);

        ButtonBuilder &WithClickCallback(const UICallback &hCallback);

        std::unique_ptr<Button> Build();

    };

}

#endif //CITYBUILDER_BUTTON_BUILDER_H
