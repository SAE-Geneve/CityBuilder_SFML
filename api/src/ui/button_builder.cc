//
// Created by sebas on 23.06.2026.
//

#include "ui/button_builder.h"
#include "ui/button.h"

namespace api::ui {
    ButtonBuilder &ButtonBuilder::New(){
        product = Button();
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithPosition(sf::Vector2f pos, sf::Vector2f vertexSize){
        product.pos_ = pos;
        product.vertex_size_ = vertexSize;
        product.SetZone(sf::IntRect(sf::Vector2i{pos}, sf::Vector2i{vertexSize}));

        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithText(std::string text){
        product.text_ = text;
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithBaseTile(sf::FloatRect baseTiling){
        product.baseTiling_ = baseTiling;
        product.tiling_ = baseTiling;
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithHoverTile(sf::FloatRect hoverTiling){
        product.hoverTiling_ = hoverTiling;
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithClickTile(sf::FloatRect clickTiling){
        product.leftClickTiling_ = clickTiling;
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithHoverCallback(const UICallback &hCallback){
        product.hoverCallback_ = hCallback;
        return *this;
    }

    ButtonBuilder &ButtonBuilder::WithClickCallback(const UICallback &cCallback){
        product.leftClickCallback_ = cCallback;
        return *this;
    }

    std::unique_ptr<Button> ButtonBuilder::Build(){
        return std::make_unique<Button>(product);
    }

}
