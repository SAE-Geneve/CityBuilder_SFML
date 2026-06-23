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

        // Intrinsic-behavior hooks. Default = no-op; derived classes override.
        virtual void OnHoverEnter()   {}
        virtual void OnHoverExit()    {}
        virtual void OnReleasedLeft() {}
        virtual void OnReleasedRight(){}
        virtual void OnPressedLeft()  {}
        virtual void OnPressedRight() {}

    public:
        virtual ~Clickable() = default;
        bool DoHoverEnterEvents(const std::optional<sf::Event> &evt);
        bool DoHoverExitEvents(const std::optional<sf::Event> &evt);
        bool DoReleasedEvents(const std::optional<sf::Event> &evt);
        bool DoPressedEvents(const std::optional<sf::Event> &evt);
        bool HandleEvent(const std::optional<sf::Event> &event);

        // Force a hover-exit from outside (e.g. to reset sibling widgets).
        void LeaveHover();

        virtual void SetZone(sf::IntRect zone);
    };
} // namespace api::ui

#endif  // CLICKABLE_H
