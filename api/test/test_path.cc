
#include <gtest/gtest.h>
#include "motion/path.h"

void Fill_5_Points(api::motion::Path& path) {
  path.Fill(
    {
      sf::Vector2f(1,1),
      sf::Vector2f(2,2),
      sf::Vector2f(3,3),
      sf::Vector2f(4,4),
      sf::Vector2f(5,5)
    }
  );
}

TEST(Path, IsValid) {
  api::motion::Path path;
  EXPECT_FALSE(path.IsValid());

  Fill_5_Points(path);
  EXPECT_TRUE(path.IsValid());

}

TEST(Path, IsDone) {
  api::motion::Path path;
  Fill_5_Points(path);

  EXPECT_FALSE(path.IsDone());

  for (int i = 0; i < 4; i++) {
    EXPECT_FALSE(path.IsDone());
    auto p = path.GetNextPoint();
  }
  EXPECT_TRUE(path.IsDone());

  path.GetNextPoint();
  EXPECT_TRUE(path.IsDone());

}

TEST(Path, StartPoint) {
  api::motion::Path path;

  EXPECT_EQ(path.StartPoint(), sf::Vector2f(0,0));

  Fill_5_Points(path);
  EXPECT_EQ(path.StartPoint(), sf::Vector2f(1,1));

  path.GetNextPoint();
  EXPECT_EQ(path.StartPoint(), sf::Vector2f(1,1));

}

TEST(Path, FillAndPoints) {
  api::motion::Path path;
  path.Fill({});
  EXPECT_FALSE(path.IsValid());

  path.Fill({{1,1}});
  EXPECT_TRUE(path.IsValid());

  path.Fill({{11,11}, {12,12}, {13,13}, {14,14}, {15,15}});
  EXPECT_TRUE(path.IsValid());

  auto pts = path.Points();
  EXPECT_EQ(pts.size(), 5);
  EXPECT_EQ(pts[0], sf::Vector2f(11,11));
  EXPECT_EQ(pts[4], sf::Vector2f(15,15));

}