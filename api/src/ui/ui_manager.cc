//
// Created by sebas on 22.06.2026.
//

#include "ui/ui_manager.h"

namespace api::ui {
    bool ui_manager::InitTexture(std::string_view texture_path){
        return texture_.loadFromFile(texture_path);
    }

    bool ui_manager::InitLabelStyle(std::string_view font_path){
        if(font_.openFromFile(font_path)) {
            label_ = sf::Text(font_);
            if (label_.has_value()) {
                label_->setCharacterSize(24);
                return true;
            }
        }
        return false;
    }

    void ui_manager::AddButton(const sf::Vector2f pos, std::string_view text){
        buttons_.emplace_back(std::make_unique<Button>(pos, text));
    }

    void ui_manager::Draw(sf::RenderWindow &window){

        vertices_.clear();
        for (auto &button: buttons_) {
            for (auto &vertex: button->GetVertices()) {
                vertices_.append(vertex);
            }
        }

        sf::RenderStates states;
        states.texture = &texture_;
        if (vertices_.getVertexCount() > 0) {
            window.draw(vertices_, states);
        }

        if (label_.has_value()) {
            for (const auto &button: buttons_) {
                label_->setString(button->GetLabel().data());
                label_->setOrigin(label_->getLocalBounds().getCenter());
                label_->setPosition(button->GetPosition() + button->GetVertexSize() * 0.5f);
                window.draw(label_.value());
            }
        }

    }

    void ui_manager::HandleEvent(const sf::Event &event, const sf::RenderWindow &window) const{
        for (auto &button: buttons_) {
            bool wasClicked = false;
            button->HandleEvent(event, wasClicked);
            if (wasClicked) {
                return;
            }
        }
    }

}
