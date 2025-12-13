#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <algorithm>
#include <fstream>
#include <iostream>

constexpr auto testData = R"(987654321111111
811111111111119
234234234234278
818181911112111)";

int GetVoltagePart1(const std::string &bank)
{
  const auto highestBatteryIt = std::max_element(bank.begin(), bank.end() - 1);
  const auto secondHighestElemIt = std::max_element(highestBatteryIt + 1, bank.end());
  return (*highestBatteryIt - '0') * 10 + (*secondHighestElemIt - '0');
}

int SumBanksVoltagePart1(const std::vector<std::string> &banks)
{
  int sum = 0;
  for (const auto &bank : banks)
  {
    sum += GetVoltagePart1(bank);
  }
  return sum;
}

uint64_t GetVoltagePart2(const std::string &bank)
{
  size_t maxBattery = 12;
  uint64_t voltage = 0;
  auto bankIt = bank.begin();
  for (size_t batteryCounter = 0; batteryCounter < maxBattery; ++batteryCounter)
  {
    bankIt = std::max_element(bankIt, bank.end() - (maxBattery - batteryCounter - 1));
    voltage += (*bankIt - '0') * std::pow(10, maxBattery - batteryCounter - 1);
    bankIt += 1;
  }
  return voltage;
}

uint64_t SumBanksVoltagePart2(const std::vector<std::string> &banks)
{
  uint64_t sum = 0;
  for (const auto &bank : banks)
  {
    sum += GetVoltagePart2(bank);
  }
  return sum;
}

std::vector<std::string> ReadBatteryBanks(std::istream &is)
{
  std::string line;
  std::vector<std::string> banks;
  while (std::getline(is, line))
  {
    banks.push_back(line);
  }
  return banks;
}

TEST_CASE("Check with example data")
{
  std::stringstream input{testData};
  const auto banks = ReadBatteryBanks(input);

  SECTION("Part 1")
  {
    REQUIRE(SumBanksVoltagePart1(banks) == 357);
  }

  SECTION("Part 2")
  {
    REQUIRE(SumBanksVoltagePart2(banks) == 3121910778619);
  }
}

TEST_CASE("Task day 3")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());
  const auto banks = ReadBatteryBanks(data);

  SECTION("part 1")
  {
    Timer t;
    std::cout << "Result: " << SumBanksVoltagePart1(banks) << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    std::cout << "Result: " << SumBanksVoltagePart2(banks) << std::endl;
  }
}