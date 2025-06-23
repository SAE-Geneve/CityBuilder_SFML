
#ifndef PATH_H
#define PATH_H
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace api::motion {

    class Path {
        int idxPoint_ = 0;
        std::vector<sf::Vector2f> path_points_;

    public:
        [[nodiscard]] bool IsDone() const;
        [[nodiscard]] bool IsValid() const;
        [[nodiscard]] sf::Vector2f GetNextPoint();
        [[nodiscard]] sf::Vector2f StartPoint() const;

        void Fill(const std::vector<sf::Vector2f> &path_points);
        std::vector<sf::Vector2f> Points(){return  path_points_;};

    };
}

#endif //PATH_H
