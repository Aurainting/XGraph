#include <catch2/catch_test_macros.hpp>

#include "xgraph"

using xgraph::XEdge;
using xgraph::XNode;

static constexpr int N = 10;

TEST_CASE("DiGraph DFS", "DiGraph") {
  const auto graph = std::make_shared<xgraph::DiGraph<>>();

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);
  for (int i = 0; i < N; ++i) {
    graph->AddNode(i);
  }

  // Add edges
  graph->AddEdge("source", "target", 2);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j)
        continue;
      graph->AddEdge(i, j);
    }
  }

  std::vector<std::shared_ptr<XNode<>>> res{};
  const std::optional<xgraph::NodePtrVisitor_t<XNode<>>> add_visitor =
      [&res](const std::shared_ptr<XNode<>>& node_ptr) {
        res.push_back(node_ptr);
      };
  xgraph::algorithm::DFS(*graph, s_node, add_visitor);
  REQUIRE(res.size() == 2);

  res.clear();
  xgraph::algorithm::DFS(*graph, graph->GetNode(0), add_visitor);
  REQUIRE(res.size() == 10);
}

TEST_CASE("DiGraph BFS", "DiGraph") {
  const auto graph = std::make_shared<xgraph::DiGraph<>>();

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);
  for (int i = 0; i < N; ++i) {
    graph->AddNode(i);
  }

  // Add edges
  graph->AddEdge("source", "target", 2);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j)
        continue;
      graph->AddEdge(i, j);
    }
  }

  std::vector<std::shared_ptr<XNode<>>> res{};
  const std::optional<xgraph::NodePtrVisitor_t<XNode<>>> add_visitor =
      [&res](const std::shared_ptr<XNode<>>& node_ptr) {
        res.push_back(node_ptr);
      };
  xgraph::algorithm::BFS(*graph, s_node, add_visitor);
  REQUIRE(res.size() == 2);

  res.clear();
  xgraph::algorithm::BFS(*graph, graph->GetNode(0), add_visitor);
  REQUIRE(res.size() == 10);
}

TEST_CASE("Graph DFS", "Graph") {
  const auto graph = std::make_shared<xgraph::Graph<>>();

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);
  for (int i = 0; i < N; ++i) {
    graph->AddNode(i);
  }

  // Add edges
  graph->AddEdge("source", "target", 2);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j)
        continue;
      graph->AddEdge(i, j);
    }
  }

  std::vector<std::shared_ptr<XNode<>>> res{};
  const std::optional<xgraph::NodePtrVisitor_t<XNode<>>> add_visitor =
      [&res](const std::shared_ptr<XNode<>>& node_ptr) {
        res.push_back(node_ptr);
      };
  xgraph::algorithm::DFS(*graph, s_node, add_visitor);
  REQUIRE(res.size() == 2);

  res.clear();
  xgraph::algorithm::DFS(*graph, graph->GetNode(0), add_visitor);
  REQUIRE(res.size() == 10);
}

TEST_CASE("Graph BFS", "Graph") {
  const auto graph = std::make_shared<xgraph::Graph<>>();

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);
  for (int i = 0; i < N; ++i) {
    graph->AddNode(i);
  }

  // Add edges
  graph->AddEdge("source", "target", 2);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j)
        continue;
      graph->AddEdge(i, j);
    }
  }

  std::vector<std::shared_ptr<XNode<>>> res{};
  const std::optional<xgraph::NodePtrVisitor_t<XNode<>>> add_visitor =
      [&res](const std::shared_ptr<XNode<>>& node_ptr) {
        res.push_back(node_ptr);
      };
  xgraph::algorithm::BFS(*graph, s_node, add_visitor);
  REQUIRE(res.size() == 2);

  res.clear();
  xgraph::algorithm::BFS(*graph, graph->GetNode(0), add_visitor);
  REQUIRE(res.size() == 10);
}
