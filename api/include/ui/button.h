//
// Created by sebas on 25/06/2025.
//

#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "clickable.h"

namespace api::ui {
    class Button : public Clickable{
        sf::Sprite sprite_;
        sf::Sprite hoverSprite_;
        sf::Text label_;

    public:
        Button(sf::Vector2f pos, std::string_view label, const sf::Texture &t, const sf::Texture &hover_t, const sf::Font& font);
        void Draw(sf::RenderWindow &window) const;
    };

}

#endif //BUTTON_H
