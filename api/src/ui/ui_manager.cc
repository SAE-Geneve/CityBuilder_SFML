//
// Created by sebas on 22.06.2026.
//

#include "ui/ui_manager.h"
#include "ui/button_builder.h"

namespace api::ui {
    bool ui_manager::InitTexture(std::string_view texture_path){
        //buttons_.reserve(3);
        return texture_.loadFromFile(texture_path);
    }

    bool ui_manager::InitLabelStyle(std::string_view font_path){
        if (font_.openFromFile(font_path)) {
            label_ = sf::Text(font_);
            if (label_.has_value()) {
                label_->setCharacterSize(18);
                return true;
            }
        }
        return false;
    }

    // ReSharper disable once CppParameterMayBeConst
    void ui_manager::AddButton(sf::Vector2f pos, const std::string &text, const UICallback &hCallback,
                               const UICallback &cCallback){

        ButtonBuilder btnBuilder;

        buttons_.emplace_back(
            std::move(btnBuilder
            .BaseButton(pos, text, {96, 96}, sf::FloatRect({0, 3 * 48}, {48, 48}))
            .SetHover(sf::FloatRect({48, 3 * 48}, {48, 48}), hCallback)
            .SetClick(sf::FloatRect({48, 0 * 48}, {48, 48}), cCallback)
            .Build())
        );

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
            // Do we enter in a button
            if (button->DoHoverEnterEvents(event)) {
                // Exit other buttons
                for (auto &exitButton: buttons_) {
                    if (button != exitButton) {
                        exitButton->LeaveHover();
                    }
                }
            }
            // Do we exit a button
            button->DoHoverExitEvents(event);
            // Do we press
            if (button->DoPressedEvents(event)) {
                return;
            }
            // Do we release
            if (button->DoReleasedEvents(event)) {
                return;
            }

        }
    }

}
