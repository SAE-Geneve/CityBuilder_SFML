//
// Created by sebas on 25/06/2025.
//

#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <functional>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

namespace api::ui {
    class Clickable {
    private:
        sf::IntRect zone_;
        bool isHover_ = false;

    protected:
        // Constructeur protected, la classe demande un heritage
        Clickable() = default;

        std::function<void()> OnHoverEnter;
        std::function<void()> OnHoverExit;

    public:
        virtual ~Clickable() = default;
        void HandleEvent(std::optional<sf::Event> event, bool &wasClicked);

        virtual void SetZone(sf::IntRect zone);

        std::function<void()> OnReleasedLeft;
        std::function<void()> OnReleasedRight;
        std::function<void()> OnPressedLeft;
        std::function<void()> OnPressedRight;
    };
}

#endif //CLICKABLE_H