//
// Created by sebas on 25/06/2025.
//

#include "ui/clickable.h"

namespace api::ui {
    void Clickable::HandleEvent(std::optional<sf::Event> evt, bool &wasClicked){

        const auto released = evt->getIf<sf::Event::MouseButtonReleased>();
        if (released && !wasClicked) {
            if (zone_.contains(released->position)) {
                if (released->button == sf::Mouse::Button::Left) {
                    if (OnReleasedLeft) {
                      OnReleasedLeft();
                      wasClicked |= true;  // Indicate that a click was handled
                    }
                }

                if (released->button == sf::Mouse::Button::Right) {
                    if (OnReleasedRight) {
                      OnReleasedRight();
                      wasClicked |= true;  // Indicate that a click was handled
                    }
                }
            }
        }

        const auto pressed = evt->getIf<sf::Event::MouseButtonPressed>();
        if (pressed && !wasClicked) {
            if (zone_.contains(pressed->position)) {
                if (pressed->button == sf::Mouse::Button::Left) {
                    if (OnPressedLeft) {
                      OnPressedLeft();
                      wasClicked |= true;  // Indicate that a click was handled
                    }
                }

                if (pressed->button == sf::Mouse::Button::Right) {
                    if (OnPressedRight) {
                      OnPressedRight();
                      wasClicked |= true;  // Indicate that a click was handled
                    }
                }
            }
        }

        const auto mouseMove = evt->getIf<sf::Event::MouseMoved>();
        if (mouseMove) {
            if (zone_.contains(mouseMove->position) && !isHover_) {
                if (OnHoverEnter) OnHoverEnter();
                isHover_ = true;
            }
            if (!zone_.contains(mouseMove->position) && isHover_) {
                if (OnHoverExit) OnHoverExit();
                isHover_ = false;
            }
        }
    }

    void Clickable::SetZone(sf::IntRect zone){
        zone_ = zone;
    }

}
