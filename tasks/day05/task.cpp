#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>

constexpr auto testData = R"(3-5
10-14
16-20
12-18

1
5
8
11
17
32)";

using Range = std::pair<uint64_t, uint64_t>;
using Ranges = std::vector<Range>;
using Ids = std::vector<uint64_t>;

std::pair<Ranges, Ids> ParseFile(std::istream &in)
{
  Ranges ranges;
  Ids ids;
  std::string line;
  bool readingRanges = true;

  while (std::getline(in, line))
  {
    if (!line.empty() && (line.back() == '\r' || line.back() == '\n'))
    {
      line.pop_back();
    }

    if (line.empty())
    {
      readingRanges = false;
      continue;
    }

    std::stringstream iss(line);

    if (readingRanges)
    {
      uint64_t a, b;
      char dash;
      if (iss >> a >> dash >> b && dash == '-')
      {
        ranges.emplace_back(a, b);
      }
    }
    else
    {
      uint64_t id;
      if (iss >> id)
      {
        ids.push_back(id);
      }
    }
  }
  return {ranges, ids};
}

int CountFreshPart1(const Ranges &ranges, const Ids &ids)
{
  int count = 0;
  for (const auto &id : ids)
  {
    for (const auto &range : ranges)
    {
      if (id >= range.first && id <= range.second)
      {
        ++count;
        break;
      }
    }
  }
  return count;
}

class RangeManager
{
public:
  RangeManager() {}

  void AddRange(Range newRange)
  {
    ranges.push_back(newRange);
    std::sort(ranges.begin(), ranges.end());
    MergeRanges();
  }

  std::vector<Range> GetRanges() const
  {
    return ranges;
  }

private:
  std::vector<Range> ranges;

  void MergeRanges()
  {
    std::vector<Range> mergedRanges;
    for (auto &range : ranges)
    {
      if (mergedRanges.empty() || mergedRanges.back().second < range.first)
      {
        mergedRanges.push_back(range);
      }
      else
      {
        mergedRanges.back().second = std::max(mergedRanges.back().second, range.second);
      }
    }
    ranges = mergedRanges;
  }
};

uint64_t CountFreshPart2(const Ranges &ranges)
{
  RangeManager rm;
  for (const auto &range : ranges)
  {
    rm.AddRange(range);
  }

  const auto merged = rm.GetRanges();
  uint64_t counter = 0;

  for (const auto &range : merged)
  {
    counter += range.second - range.first + 1;
  }

  return counter;
}

TEST_CASE("Check with example data")
{
  std::stringstream is{testData};
  const auto [ranges, ids] = ParseFile(is);
  SECTION("Part 1")
  {
    REQUIRE(CountFreshPart1(ranges, ids) == 3);
  }

  SECTION("Part 2")
  {
    REQUIRE(CountFreshPart2(ranges) == 14);
  }
}

TEST_CASE("Task day 5")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());
  const auto [ranges, ids] = ParseFile(data);

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << CountFreshPart1(ranges, ids) << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << CountFreshPart2(ranges) << std::endl;
  }
}