//
// Created by sebas on 22.06.2026.
//

#ifndef CITYBUILDER_UI_MANAGER_H
#define CITYBUILDER_UI_MANAGER_H

#include <string_view>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include "button.h"

namespace api::ui {
    class ui_manager {
        sf::Texture texture_;
        sf::VertexArray vertices_ = sf::VertexArray(sf::PrimitiveType::Triangles);
        sf::Font font_;
        std::optional<sf::Text> label_;
        std::vector<std::unique_ptr<Button> > buttons_;

    public:
        bool InitTexture(std::string_view texture_path);
        bool InitLabelStyle(std::string_view label);
        void AddButton(sf::Vector2f pos, const std::string& text, const UICallback &hCallback, const UICallback &cCallback);
        void Draw(sf::RenderWindow &window);
        void HandleEvent(const sf::Event & event, const sf::RenderWindow & window) const;

    };
}

#endif //CITYBUILDER_UI_MANAGER_H
