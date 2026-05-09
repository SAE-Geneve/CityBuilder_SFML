
#ifndef PATH_H
#define PATH_H
#include <SFML/System/Vector2.hpp>
#include <vector>

namespace api::motion {

class Path {
  std::vector<sf::Vector2f> path_points_;
  size_t idxPoint_ = 0;

 public:
  [[nodiscard]] bool IsDone() const;
  [[nodiscard]] bool valid() const;
  [[nodiscard]] sf::Vector2f next_point();
  [[nodiscard]] sf::Vector2f StartPoint() const;

  void Fill(const std::vector<sf::Vector2f> &path_points);
  std::vector<sf::Vector2f> Points() { return path_points_; };
};
}  // namespace api::motion

#endif  // PATH_H
