#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>

constexpr auto testData = R"(L68
L30
R48
L5
R60
L55
L1
L99
R14
L82)";

std::vector<int> ReadRotations(std::istream &is)
{
  std::string line;
  std::vector<int> rotations;
  while (std::getline(is, line))
  {
    int rotation = 0;
    for (size_t i = 1; i < line.size(); i++)
    {
      rotation = rotation * 10 + line[i] - '0';
    }
    rotation = line[0] == 'L' ? rotation * (-1) : rotation;
    rotations.push_back(rotation);
  }
  return rotations;
}

int GetExactZeroPositions(const std::vector<int> &rotations)
{
  int position = 50;
  int zeros = 0;
  for (const auto &rotation : rotations)
  {
    position = (position + rotation) % 100;
    position = position < 0 ? position + 100 : position;
    if (position == 0)
      ++zeros;
  }
  return zeros;
}

int GetZeroPositions(const std::vector<int> &rotations)
{
  int position = 50;
  int zeros = 0;
  for (const auto &rotation : rotations)
  {
    position = position + rotation;
    if (position >= 100)
      zeros += position / 100;
    else if (position < 0)
    {
      bool startedFromOtherThanZero = position != rotation;
      zeros += std::abs(position / 100) + startedFromOtherThanZero;
    }
    else
      zeros += position == 0;
    position %= 100;
    position = position < 0 ? position + 100 : position;
  }
  return zeros;
}

TEST_CASE("Check with example data")
{
  std::stringstream input{testData};

  SECTION("Part 1")
  {
    REQUIRE(GetExactZeroPositions(ReadRotations(input)) == 3);
  }

  SECTION("Part 2")
  {
    REQUIRE(GetZeroPositions(ReadRotations(input)) == 6);
    constexpr auto testData2 = R"(R50
L50)";
    std::stringstream input2{testData2};

    REQUIRE(GetZeroPositions(ReadRotations(input2)) == 1);
  }
}

TEST_CASE("Task day 1")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << GetExactZeroPositions(ReadRotations(data)) << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << GetZeroPositions(ReadRotations(data)) << std::endl;
  }
}