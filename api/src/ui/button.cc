//
// Created by sebas on 25/06/2025.
//

#include "ui/button.h"

#include <SFML/Graphics/Texture.hpp>


namespace api::ui {
    std::span<sf::Vertex> Button::GetVertices(){

        vertices_[0] = sf::Vertex(pos_, kColor, tiling_.position);
        vertices_[1] = sf::Vertex(pos_ + sf::Vector2f({vertex_size_.x, 0.f}), kColor,
                                  tiling_.position + sf::Vector2f(tiling_.size.x, 0.f)); // + X
        vertices_[2] = sf::Vertex(pos_ + vertex_size_, kColor, tiling_.position + tiling_.size);
        vertices_[3] = sf::Vertex(pos_, kColor, tiling_.position);
        vertices_[4] = sf::Vertex(pos_ + vertex_size_, kColor, tiling_.position + tiling_.size);
        vertices_[5] = sf::Vertex(pos_ + sf::Vector2f(0.f, vertex_size_.y), kColor,
                                  tiling_.position + sf::Vector2f(0.f, tiling_.size.y)); // + Y

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

    void Button::SetPosition(sf::Vector2f pos){
        pos_ = pos;
    }

    void Button::SetText(const std::string_view text){
        text_ = text;
    }

    void Button::SetBaseTiling(const sf::FloatRect baseTiling){
        baseTiling_ = baseTiling;
        tiling_ = baseTiling_;
    }

    void Button::SetHover(sf::FloatRect hoverTiling, const UICallback &hoverCallback){
        hoverTiling_ = hoverTiling;
        hoverCallback_ = hoverCallback;

        OnHoverEnter = [this]() {
            tiling_ = hoverTiling_;
            if (hoverCallback_) {
                hoverCallback_();
            }
        };
        OnHoverExit = [this]() {
            tiling_ = baseTiling_;
        };

    }

    void Button::SetClick(sf::FloatRect clickTiling, const UICallback &clickCallback){
        clickTiling_ = clickTiling;
        clickCallback_ = clickCallback;

        OnReleasedLeft = [this]() {
            tiling_ = baseTiling_;
        };

        OnPressedLeft = [this]() {
            tiling_ = clickTiling_;
            if (clickCallback_) {
                clickCallback_();
            }
        };
    }


} // namespace api::ui
