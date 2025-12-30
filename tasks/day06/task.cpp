#include <catch2/catch_all.hpp>
#include <utils/Timer.h>

#include <fstream>
#include <iostream>
#include <string_view>
#include <ranges>

constexpr std::string_view testData =
    "123 328  51 64 \n"
    " 45 64  387 23 \n"
    "  6 98  215 314\n"
    "*   +   *   +  ";

using Column = std::vector<uint64_t>;
using Columns = std::vector<Column>;
using Operations = std::vector<char>;

std::pair<Columns, Operations> ReadDataPart1(std::istream &is)
{
  std::string s;
  std::vector<std::string> lines;
  while (std::getline(is, s))
  {
    lines.push_back(s);
  }

  Operations operations;
  for (const auto &op : lines.back())
  {
    if (op == '+' or op == '*')
    {
      operations.push_back(op);
    }
  }
  lines.pop_back();

  Columns columns{operations.size()};

  for (const auto &line : lines)
  {
    std::stringstream ss{line};
    for (size_t i = 0; i < operations.size(); ++i)
    {
      uint64_t val = 0;
      ss >> val;
      columns[i].push_back(val);
    }
  }

  return {columns, operations};
}

uint64_t GetSum(const Columns &columns, const Operations &operations)
{
  uint64_t sum = 0;

  for (const auto &[col, op] : std::views::zip(columns, operations))
  {
    uint64_t temp = (op == '*') ? 1 : 0;

    for (uint64_t val : col)
    {
      if (op == '+')
        temp += val;
      else
        temp *= val;
    }
    sum += temp;
  }
  return sum;
}

std::pair<Columns, Operations> ReadDataPart2(std::istream &is)
{
  std::string line;
  std::vector<std::string> lines;
  while (std::getline(is, line))
  {
    lines.push_back(line);
  }

  Operations operations;
  for (const auto &op : lines.back())
  {
    if (op == '+' or op == '*')
    {
      operations.push_back(op);
    }
  }
  std::ranges::reverse(operations);
  lines.pop_back();

  Columns columns{operations.size()};
  uint32_t val = 0;
  size_t column = 0;

  for (size_t count = 0; !lines.back().empty(); ++count)
  {
    size_t row = count % lines.size();
    char c = lines[row].back();
    lines[row].pop_back();
    if (c >= '0' and c <= '9')
    {
      val = val * 10 + c - '0';
    }

    if (row == lines.size() - 1)
    {
      if (val != 0)
      {
        columns[column].push_back(val);
        val = 0;
      }
      else
      {
        ++column;
      }
    }
  }

  return {columns, operations};
}

TEST_CASE("Check with example data")
{
  std::stringstream is{std::string{testData}};

  SECTION("Part 1")
  {
    const auto [columns, operations] = ReadDataPart1(is);
    REQUIRE(GetSum(columns, operations) == 4277556);
  }

  SECTION("Part 2")
  {
    const auto [columns, operations] = ReadDataPart2(is);
    REQUIRE(GetSum(columns, operations) == 3263827);
  }
}

TEST_CASE("Task day 6")
{
  std::ifstream data("data.txt");

  REQUIRE(data.is_open());

  SECTION("part 1")
  {
    Timer t;
    const auto [rowsOfValues, operations] = ReadDataPart1(data);
    std::cout << "Result: " << GetSum(rowsOfValues, operations) << std::endl;
  }

  SECTION("part 2")
  {
    Timer t;
    const auto [rowsOfValues, operations] = ReadDataPart2(data);
    std::cout << "Result: " << GetSum(rowsOfValues, operations) << std::endl;
  }
}