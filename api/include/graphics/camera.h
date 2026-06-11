#ifndef CITYBUILDER_CAMERA_H
#define CITYBUILDER_CAMERA_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

namespace graphics {
    class Camera {
        sf::View view_;
        sf::FloatRect mapBounds_;

        bool dragging_ = false;
        sf::Vector2i lastMousePixel_{};
        sf::Vector2f lastWindowSize_{};

        float wheelZoomStep_ = 1.1f;
        float currentZoom_ = 1.f;
        float minZoom_ = 0.25f;
        float maxZoom_ = 4.f;

        float panSpeed_ = 600.f;
        float keyZoomRate_ = 1.5f;


    public:
        void Setup(sf::Vector2f windowSize, sf::FloatRect mapBounds);

        void HandleEvent(const sf::Event &event, const sf::RenderWindow &window);

        void Update(float dt);

        void Apply(sf::RenderWindow &window);

        void OnWindowResized(sf::Vector2u newSize);

    private:
        void Zoom(float factor, sf::Vector2f anchorWorld);
        bool DoesTheViewLookTheMap();

    };
}

#endif  // CITYBUILDER_CAMERA_H
