#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>

constexpr auto testData = R"(..@@.@@@@.
@@@.@.@.@@
@@@@@.@.@@
@.@@@@..@.
@@.@@@@.@@
.@@@@@@@.@
.@.@.@.@@@
@.@@@.@@@@
.@@@@@@@@.
@.@.@@@.@.)";

class Map
{
public:
  explicit Map(std::istream &is)
  {
    std::string line;
    while (std::getline(is, line))
    {
      grid.push_back(line);
    }

    maxX = grid[0].size() - 1;
    maxY = grid.size() - 1;
  }

  std::optional<char> GetValue(int x, int y)
  {
    if (x < 0 || x > static_cast<int>(maxX))
    {
      return std::nullopt;
    }

    if (y < 0 || y > static_cast<int>(maxY))
    {
      return std::nullopt;
    }

    return grid[y][x];
  }

  bool IsObjectMoveable(size_t x, size_t y)
  {
    const auto object = GetValue(x, y);
    if (!object.has_value() or object.value() != '@')
    {
      return false;
    }

    size_t counter = 0;
    for (int tempX = static_cast<int>(x) - 1; tempX <= static_cast<int>(x) + 1; ++tempX)
    {
      for (int tempY = static_cast<int>(y) - 1; tempY <= static_cast<int>(y) + 1; ++tempY)
      {
        if (tempX == static_cast<int>(x) and tempY == static_cast<int>(y))
        {
          continue;
        }
        if (const auto value = GetValue(tempX, tempY);
            value.has_value() and value.value() == '@')
        {
          ++counter;
        }
        if (counter > 3)
        {
          return false;
        }
      }
    }
    return true;
  }

  size_t CountMoveableObjectsPart1()
  {
    size_t counter = 0;
    for (size_t x = 0; x <= maxX; ++x)
    {
      for (size_t y = 0; y <= maxY; ++y)
      {
        counter += IsObjectMoveable(x, y);
      }
    }
    return counter;
  }

  size_t CountMoveableObjectsPart2()
  {
    size_t counter = 0;
    size_t lastCounterState = 0;
    for (int x = 0; x <= static_cast<int>(maxX); ++x)
    {
      for (int y = 0; y <= static_cast<int>(maxY); ++y)
      {
        if (IsObjectMoveable(x, y))
        {
          ++counter;
          grid[y][x] = '.';
        }
      }
      if (lastCounterState != counter)
      {
        x -= 2;
      }
      lastCounterState = counter;
    }
    return counter;
  }

  size_t maxX;
  size_t maxY;

private:
  std::vector<std::string> grid;
};

TEST_CASE("Check with example data")
{
  std::stringstream input{testData};

  SECTION("Part 1")
  {
    REQUIRE(Map(input).CountMoveableObjectsPart1() == 13);
  }

  SECTION("Part 2")
  {
    REQUIRE(Map(input).CountMoveableObjectsPart2() == 43);
  }
}

TEST_CASE("Task day 4")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << Map(data).CountMoveableObjectsPart1() << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << Map(data).CountMoveableObjectsPart2() << std::endl;
  }
}