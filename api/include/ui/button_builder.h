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
        ButtonBuilder& BaseButton(sf::Vector2f pos, std::string text_, sf::Vector2f vertexSize, sf::FloatRect baseTiling);
        ButtonBuilder& SetHover(sf::FloatRect hoverTiling, const UICallback &hCallback);
        ButtonBuilder& SetClick(sf::FloatRect clickTiling, const UICallback &cCallback);

        std::unique_ptr<Button> Build();

    };

}

#endif //CITYBUILDER_BUTTON_BUILDER_H
