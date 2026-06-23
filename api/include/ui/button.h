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

        friend class ButtonBuilder;

        sf::Vector2f vertex_size_;
        std::array<sf::Vertex, 6> vertices_;

        sf::Vector2f pos_;
        static constexpr sf::Color kColor = sf::Color::White;
        std::string text_;

        sf::FloatRect tiling_;
        sf::FloatRect baseTiling_;
        sf::FloatRect hoverTiling_;
        sf::FloatRect clickTiling_;

        UICallback hoverCallback_;
        UICallback clickCallback_;

    protected:
        // Intrinsic button behavior: swap the tiling, then fire the user callback.
        void OnHoverEnter()   override;
        void OnHoverExit()    override;
        void OnPressedLeft()  override;
        void OnReleasedLeft() override;

    public:
        Button() = default;

        [[nodiscard]] std::span<sf::Vertex> GetVertices();
        [[nodiscard]] std::string GetLabel() const;
        [[nodiscard]] sf::Vector2f GetPosition() const;
        [[nodiscard]] sf::Vector2f GetVertexSize() const;
    };



} // namespace api::ui

#endif  // API_UI_BUTTON_H
