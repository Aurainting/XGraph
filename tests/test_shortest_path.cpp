#include <catch2/catch_test_macros.hpp>
#include <format>
#include <ranges>

#include "xgraph"

using xgraph::XEdge;
using xgraph::XNode;

static const std::vector<std::vector<int>> grid_1 = {
    {0, 0, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 1, 0, 0}};

static const std::vector<std::vector<int>> grid_2 = {
    {0, 1, 0}, {1, 1, 0}, {0, 0, 0}};

template <xgraph::NodeType Node, xgraph::EdgeType Edge>
void build_graph(xgraph::DiGraph<Node, Edge>& graph,
                 const std::vector<std::vector<int>>& grid_map) {
  for (const auto [i, row] : std::views::enumerate(grid_map)) {
    for (const auto [j, _] : std::views::enumerate(row)) {
      graph.AddNode(std::format("({}, {})", i, j));
    }
  }
  for (const auto [i, row] : std::views::enumerate(grid_map)) {
    for (const auto [j, elem] : std::views::enumerate(row)) {
      if (elem == 1) {
        continue;
      }

      // Up
      if (i > 0) {
        if (grid_map[i - 1][j] == 0) {
          graph.AddEdge(std::format("({}, {})", i, j),
                        std::format("({}, {})", i - 1, j));
        }
      }
      // Down
      if (i < grid_map.size() - 1) {
        if (grid_map[i + 1][j] == 0) {
          graph.AddEdge(std::format("({}, {})", i, j),
                        std::format("({}, {})", i + 1, j));
        }
      }
      // Left
      if (j > 0) {
        if (row[j - 1] == 0) {
          graph.AddEdge(std::format("({}, {})", i, j),
                        std::format("({}, {})", i, j - 1));
        }
      }
      // Right
      if (j < row.size() - 1) {
        if (row[j + 1] == 0) {
          graph.AddEdge(std::format("({}, {})", i, j),
                        std::format("({}, {})", i, j + 1));
        }
      }
    }
  }
}

TEST_CASE("DiGraph AStar", "DiGraph") {
  auto graph = std::make_shared<xgraph::DiGraph<>>();
  build_graph(*graph, grid_1);

  constexpr auto source = std::make_pair(0, 0);
  auto target = std::make_pair(3, 3);
  const auto res = xgraph::algorithm::AStarPath(
      *graph,
      graph->GetNode(std::format("({}, {})", source.first, source.second)),
      graph->GetNode(std::format("({}, {})", target.first, target.second)));

  REQUIRE(!res.empty());
  REQUIRE(res.size() == 7);
  REQUIRE(res.front()->Name() ==
          std::format("({}, {})", source.first, source.second));
  REQUIRE(res.back()->Name() ==
          std::format("({}, {})", target.first, target.second));

  graph = std::make_shared<xgraph::DiGraph<>>();
  build_graph(*graph, grid_2);

  target = std::make_pair(2, 2);
  REQUIRE_THROWS_AS(
      xgraph::algorithm::AStarPath(
          *graph,
          graph->GetNode(std::format("({}, {})", source.first, source.second)),
          graph->GetNode(std::format("({}, {})", target.first, target.second))),
      std::runtime_error);
}

TEST_CASE("Graph AStar", "Graph") {
  auto graph = std::make_shared<xgraph::Graph<>>();
  build_graph(*graph, grid_1);

  constexpr auto source = std::make_pair(0, 0);
  auto target = std::make_pair(3, 3);
  const auto res = xgraph::algorithm::AStarPath(
      *graph,
      graph->GetNode(std::format("({}, {})", source.first, source.second)),
      graph->GetNode(std::format("({}, {})", target.first, target.second)));

  REQUIRE(!res.empty());
  REQUIRE(res.size() == 7);
  REQUIRE(res.front()->Name() ==
          std::format("({}, {})", source.first, source.second));
  REQUIRE(res.back()->Name() ==
          std::format("({}, {})", target.first, target.second));

  graph = std::make_shared<xgraph::Graph<>>();
  build_graph(*graph, grid_2);

  target = std::make_pair(2, 2);
  REQUIRE_THROWS_AS(
      xgraph::algorithm::AStarPath(
          *graph,
          graph->GetNode(std::format("({}, {})", source.first, source.second)),
          graph->GetNode(std::format("({}, {})", target.first, target.second))),
      std::runtime_error);
}
