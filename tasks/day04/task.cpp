#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>

constexpr auto testData = "";

TEST_CASE("Check with example data")
{
  SECTION("Part 1")
  {
  }

  SECTION("Part 2")
  {
  }
}

TEST_CASE("Task day 4")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << testData << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << testData << std::endl;
  }
}