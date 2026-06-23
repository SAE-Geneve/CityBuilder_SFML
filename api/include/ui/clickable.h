//
// Created by sebas on 25/06/2025.
//

#ifndef CLICKABLE_H
#define CLICKABLE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>

namespace api::ui {

    using UICallback = std::function<void()>;

    class Clickable {
    private:
        sf::IntRect zone_;
        bool isHover_ = false;

    protected:
        // Constructeur protected, la classe demande un heritage
        Clickable() = default;

        UICallback OnHoverEnter;
        UICallback OnReleasedLeft;
        UICallback OnReleasedRight;
        UICallback OnPressedLeft;
        UICallback OnPressedRight;

    public:
        virtual ~Clickable() = default;
        bool DoHoverEnterEvents(const std::optional<sf::Event> &evt);
        bool DoHoverExitEvents(const std::optional<sf::Event> &evt);
        bool DoReleasedEvents(const std::optional<sf::Event> &evt) const;
        bool DoPressedEvents(const std::optional<sf::Event> &evt) const;
        bool HandleEvent(const std::optional<sf::Event> &event);

        virtual void SetZone(sf::IntRect zone);

        UICallback OnHoverExit;

    };
} // namespace api::ui

#endif  // CLICKABLE_H
