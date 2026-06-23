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
        sf::Vector2f vertex_size_ = {96, 96};
        std::array<sf::Vertex, 6> vertices_;

        sf::Vector2f pos_;
        static constexpr sf::Color kColor = sf::Color::White;
        std::string_view text_;

        sf::FloatRect tiling_;
        sf::FloatRect baseTiling_;
        sf::FloatRect hoverTiling_;
        sf::FloatRect clickTiling_;

        UICallback hoverCallback_;
        UICallback clickCallback_;

    public:
        Button() = default;

        [[nodiscard]] std::span<sf::Vertex> GetVertices();
        [[nodiscard]] std::string_view GetLabel() const;
        [[nodiscard]] sf::Vector2f GetPosition() const;
        [[nodiscard]] sf::Vector2f GetVertexSize() const;

        void SetPosition(sf::Vector2f pos);
        void SetText(std::string text);
        void SetBaseTiling(sf::FloatRect baseTiling);
        void SetHover(sf::FloatRect hoverTiling, const UICallback &hoverCallback);
        void SetClick(sf::FloatRect clickTiling, const UICallback &clickCallback);


    };



} // namespace api::ui

#endif  // API_UI_BUTTON_H
