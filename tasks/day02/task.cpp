#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <regex>

constexpr auto testData = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,1698522-1698528,446443-446449,38593856-38593862,565653-565659,824824821-824824827,2121212118-2121212124";

std::vector<std::pair<uint64_t, uint64_t>> ReadRanges(std::istream &is)
{
  char c;
  uint64_t first = 0;
  uint64_t second = 0;
  bool isFirst = true;
  std::vector<std::pair<uint64_t, uint64_t>> ranges;

  while (is.get(c))
  {
    if (c == '-')
    {
      isFirst = false;
    }
    else if (c == ',')
    {
      ranges.push_back({first, second});
      isFirst = true;
      first = 0;
      second = 0;
    }
    else if (isFirst)
    {
      first = first * 10 + c - '0';
    }
    else
    {
      second = second * 10 + c - '0';
    }
  }
  ranges.push_back({first, second});

  return ranges;
}

uint64_t GetNumberOfDifits(uint64_t value)
{
  uint64_t digits = 0;
  while (value > 0)
  {
    value /= 10;
    ++digits;
  }
  return digits;
}

std::set<uint64_t> GetInvalidIds(const std::pair<uint64_t, uint64_t> &range)
{
  std::set<uint64_t> ids;
  const auto startNumberOfDigits = GetNumberOfDifits(range.first);
  const auto endNumberOfDigits = GetNumberOfDifits(range.second);

  if (startNumberOfDigits % 2 != 0 and startNumberOfDigits == endNumberOfDigits)
  {
    return {};
  }

  uint64_t startIdFactor = std::pow(10, startNumberOfDigits % 2 == 0 ? startNumberOfDigits / 2 : startNumberOfDigits / 2 + 1);
  uint64_t endIdFactor = std::pow(10, endNumberOfDigits / 2);

  for (uint64_t i = range.first / startIdFactor; i <= range.second / endIdFactor; ++i)
  {
    if ((i * std::pow(10, GetNumberOfDifits(i)) + i) >= range.first and
        (i * std::pow(10, GetNumberOfDifits(i)) + i) <= range.second)
    {
      ids.emplace(i * std::pow(10, GetNumberOfDifits(i)) + i);
    }
  }
  return ids;
}

uint64_t GetSumOfInvalidIds(const std::vector<std::pair<uint64_t, uint64_t>> &ranges)
{
  uint64_t sum = 0;
  for (const auto &range : ranges)
  {
    const auto values = GetInvalidIds(range);
    sum = std::accumulate(values.begin(), values.end(), sum);
    std::cout << sum << std::endl;
  }
  return sum;
}

std::set<uint64_t> GetInvalidIdsPart2(const std::pair<uint64_t, uint64_t> &range)
{
  std::regex repeated(R"(^(\d+?)\1+$)");
  std::set<uint64_t> values;

  for (uint64_t i = range.first; i <= range.second; i++)
  {
    const auto str = std::to_string(i);

    std::smatch matches;

    if (!std::regex_search(str, matches, repeated))
    {
      continue;
    }
    values.emplace(i);
  }
  return values;
}

uint64_t GetSumOfInvalidIdsPart2(const std::vector<std::pair<uint64_t, uint64_t>> &ranges)
{
  uint64_t sum = 0;

  for (const auto &range : ranges)
  {
    const auto values = GetInvalidIdsPart2(range);
    sum = std::accumulate(values.begin(), values.end(), sum);
  }
  return sum;
}

TEST_CASE("Check with example data")
{
  std::stringstream input{testData};

  SECTION("Part 1")
  {
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{11, 22}) == std::set<uint64_t>{11, 22});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{95, 115}) == std::set<uint64_t>{99});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{998, 1012}) == std::set<uint64_t>{1010});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{1188511880, 1188511890}) == std::set<uint64_t>{1188511885});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{222220, 222224}) == std::set<uint64_t>{222222});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{1698522, 1698528}) == std::set<uint64_t>{});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{446443, 446449}) == std::set<uint64_t>{446446});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{38593856, 38593862}) == std::set<uint64_t>{38593859});
    REQUIRE(GetInvalidIds(std::pair<uint64_t, uint64_t>{1, 1313}) == std::set<uint64_t>{11, 22, 33, 44, 55, 66, 77, 88, 99, 1010, 1111, 1212, 1313});
    REQUIRE(GetSumOfInvalidIds(ReadRanges(input)) == 1227775554);
  }

  SECTION("Part 2")
  {
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{11, 22}) == std::set<uint64_t>{11, 22});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{95, 115}) == std::set<uint64_t>{99, 111});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{998, 1012}) == std::set<uint64_t>{999, 1010});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{1188511880, 1188511890}) == std::set<uint64_t>{1188511885});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{222220, 222224}) == std::set<uint64_t>{222222});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{1698522, 1698528}) == std::set<uint64_t>{});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{446443, 446449}) == std::set<uint64_t>{446446});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{38593856, 38593862}) == std::set<uint64_t>{38593859});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{824824821, 824824827}) == std::set<uint64_t>{824824824});
    REQUIRE(GetInvalidIdsPart2(std::pair<uint64_t, uint64_t>{2121212118, 2121212124}) == std::set<uint64_t>{2121212121});

    REQUIRE(GetSumOfInvalidIdsPart2(ReadRanges(input)) == 4174379265);
  }
}

TEST_CASE("Task day 2")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << GetSumOfInvalidIds(ReadRanges(data)) << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << GetSumOfInvalidIdsPart2(ReadRanges(data)) << std::endl;
  }
}