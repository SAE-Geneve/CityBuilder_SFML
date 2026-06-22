//
// Created by sebas on 25/06/2025.
//

#ifndef API_UI_BUTTON_H
#define API_UI_BUTTON_H

#include <span>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "clickable.h"

namespace api::ui {
    class Button : public Clickable {
        static constexpr sf::Vector2f vertex_size_ = {96, 96};
        // ReSharper disable once CppUseAuto
        sf::FloatRect texture_bounds_ = sf::FloatRect({0, 3 * 48}, {48, 48});

        sf::Vector2f pos_;
        sf::Color color_ = sf::Color::White;
        std::array<sf::Vertex, 6> vertices_;
        std::string_view text_;

    public:
        Button(sf::Vector2f pos, std::string_view text);
        void Draw(sf::RenderWindow &window) const;
        [[nodiscard]] std::span<sf::Vertex> GetVertices();
        [[nodiscard]] const std::string_view GetLabel() const;
        [[nodiscard]] const sf::Vector2f GetPosition() const;
        [[nodiscard]] const sf::Vector2f GetVertexSize() const;


    };

} // namespace api::ui

#endif  // API_UI_BUTTON_H
