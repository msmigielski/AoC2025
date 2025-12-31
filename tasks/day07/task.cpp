#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <numeric>
#include <unordered_set>
#include <algorithm>

constexpr auto testData = R"(.......S.......
...............
.......^.......
...............
......^.^......
...............
.....^.^.^.....
...............
....^.^...^....
...............
...^.^...^.^...
...............
..^...^.....^..
...............
.^.^.^.^.^...^.
...............)";

class TachyonManifold
{
public:
  using SplitterPositionsToHits = std::unordered_set<size_t>;
  using Layers = std::vector<SplitterPositionsToHits>;

  TachyonManifold(std::istream &is)
  {
    std::string line;
    std::getline(is, line);
    start = line.find('S');

    while (std::getline(is, line))
    {
      SplitterPositionsToHits positions;
      for (size_t i = 0; i < line.size(); ++i)
      {
        if (line[i] == '^')
          positions.emplace(i);
      }
      layers.push_back(positions);
    }
  }

  size_t CountHitSplitters() const
  {
    size_t counter = 0;
    std::unordered_set<size_t> currentBeamPositions{start};
    for (const auto &layer : layers)
    {
      std::unordered_set<size_t> newBeamPositions;
      std::unordered_set<size_t> splittersHits;

      for (const auto &beamPosition : currentBeamPositions)
      {
        if (layer.contains(beamPosition))
        {
          splittersHits.emplace(beamPosition);
          newBeamPositions.emplace(beamPosition - 1);
          newBeamPositions.emplace(beamPosition + 1);
        }
        else
        {
          newBeamPositions.emplace(beamPosition);
        }
      }
      currentBeamPositions = newBeamPositions;
      counter += splittersHits.size();
    }
    return counter;
  }

  size_t CountBeams() const
  {
    std::unordered_map<size_t, size_t> currentBeamPositionsToCount{{start, 1}};
    for (const auto &layer : layers)
    {
      std::unordered_map<size_t, size_t> newBeamPositionsToCount;

      for (const auto &[beamPosition, count] : currentBeamPositionsToCount)
      {
        if (layer.contains(beamPosition))
        {
          newBeamPositionsToCount[beamPosition - 1] += count;
          newBeamPositionsToCount[beamPosition + 1] += count;
        }
        else
        {
          newBeamPositionsToCount[beamPosition] += count;
        }
      }
      currentBeamPositionsToCount.swap(newBeamPositionsToCount);
    }

    size_t sum = std::accumulate(
        currentBeamPositionsToCount.begin(), currentBeamPositionsToCount.end(),
        size_t{0},
        [](uint64_t acc, const auto &kv)
        { return acc + kv.second; });

    return sum;
  }

private:
  size_t start;
  Layers layers;
};

TEST_CASE("Check with example data")
{
  std::stringstream is{std::string{testData}};

  SECTION("Part 1")
  {
    REQUIRE(TachyonManifold(is).CountHitSplitters() == 21);
  }

  SECTION("Part 2")
  {
    REQUIRE(TachyonManifold(is).CountBeams() == 40);
  }
}

TEST_CASE("Task day 7")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << TachyonManifold(data).CountHitSplitters() << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << TachyonManifold(data).CountBeams() << std::endl;
  }
}