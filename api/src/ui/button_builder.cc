//
// Created by sebas on 23.06.2026.
//

#include "ui/button_builder.h"
#include "ui/button.h"

namespace api::ui {

    ButtonBuilder &ButtonBuilder::BaseButton(sf::Vector2f pos, std::string text_, sf::Vector2f vertexSize,
                                             sf::FloatRect baseTiling){
        product = Button();

        product.pos_ = pos;
        product.text_ = text_;
        product.vertex_size_ = vertexSize;
        product.baseTiling_ = baseTiling;
        product.tiling_ = baseTiling;
        product.SetZone(sf::IntRect(sf::Vector2i{pos}, sf::Vector2i{96, 96}));

        return *this;
    }

    // ReSharper disable once CppParameterMayBeConst
    ButtonBuilder &ButtonBuilder::SetHover(sf::FloatRect hoverTiling, const UICallback &hCallback){
        product.hoverTiling_ = hoverTiling;
        product.hoverCallback_ = hCallback;
        return *this;
    }

    // ReSharper disable once CppParameterMayBeConst
    ButtonBuilder &ButtonBuilder::SetClick(sf::FloatRect clickTiling, const UICallback &cCallback){
        product.clickTiling_ = clickTiling;
        product.clickCallback_ = cCallback;
        return *this;
    }

    std::unique_ptr<Button> ButtonBuilder::Build(){
        return std::make_unique<Button>(product);
    }

}
