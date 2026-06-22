//
// Created by sebas on 25/06/2025.
//

#include "ui/button.h"

#include <SFML/Graphics/Texture.hpp>


namespace api::ui {
    Button::Button(sf::Vector2f pos, std::string_view text) : Clickable(), pos_(pos), text_(text){


        Button::SetZone(sf::IntRect(sf::Vector2i{pos_}, sf::Vector2i{vertex_size_}));

        OnHoverEnter = [this]() {
            color_ = sf::Color::White;
            texture_bounds_.position += sf::Vector2f({48,0});
        };
        OnHoverExit = [this]() {
            color_ = sf::Color::White;
            texture_bounds_.position -= sf::Vector2f({48,0});
        };
    }

    void Button::Draw(sf::RenderWindow &window) const{

        sf::RectangleShape rectangle;
        rectangle.setFillColor(color_);
        rectangle.setPosition(pos_);
        rectangle.setSize(vertex_size_);
    }

    std::span<sf::Vertex> Button::GetVertices(){

        vertices_[0] = sf::Vertex(pos_, color_, texture_bounds_.position);
        vertices_[1] = sf::Vertex(pos_ + sf::Vector2f({vertex_size_.x, 0.f}), color_,
                                  texture_bounds_.position + sf::Vector2f(texture_bounds_.size.x, 0.f)); // + X
        vertices_[2] = sf::Vertex(pos_ + vertex_size_, color_, texture_bounds_.position + texture_bounds_.size);
        vertices_[3] = sf::Vertex(pos_, color_, texture_bounds_.position);
        vertices_[4] = sf::Vertex(pos_ + vertex_size_, color_, texture_bounds_.position + texture_bounds_.size);
        vertices_[5] = sf::Vertex(pos_ + sf::Vector2f(0.f, vertex_size_.y), color_,
                                  texture_bounds_.position + sf::Vector2f(0.f, texture_bounds_.size.y)); // + Y

        return std::span<sf::Vertex>(vertices_);

    }

    // ReSharper disable once CppConstValueFunctionReturnType
    const std::string_view Button::GetLabel() const{
        return text_;
    }

    // ReSharper disable once CppConstValueFunctionReturnType
    const sf::Vector2f Button::GetPosition() const{
        return pos_;
    }

    // ReSharper disable once CppMemberFunctionMayBeStatic
    const sf::Vector2f Button::GetVertexSize() const{
        return vertex_size_;
    }
} // namespace api::ui
