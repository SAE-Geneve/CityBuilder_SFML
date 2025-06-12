
#ifndef PATH_H
#define PATH_H
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace api::motion {

    class Path {
        int idxPoint_ = 0;
        std::vector<sf::Vector2f> pathPoints_;

    public:
        [[nodiscard]] bool IsDone() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] sf::Vector2f GetNextPoint();
        [[nodiscard]] sf::Vector2f StartPoint() const;

        void Fill(std::vector<sf::Vector2f>& pathPoints);

    };
}

#endif //PATH_H
