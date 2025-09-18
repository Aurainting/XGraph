#include <catch2/catch_test_macros.hpp>
#include <string>

#include "xgraph"

using xgraph::XEdge;
using xgraph::XNode;

static constexpr int N = 10;

TEST_CASE("DiGraph Structure", "DiGraph") {
  const auto graph = std::make_shared<xgraph::DiGraph<>>();
  REQUIRE(graph->IsDirected());

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);

  for (int i = 0; i < N; ++i) {
    graph->AddNode(i);
  }

  REQUIRE(graph->NodeSize() == N + 2);

  // Add edges
  graph->AddEdge("source", "target", 2);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j)
        continue;
      graph->AddEdge(i, j);
    }
  }

  auto graph_copy(std::make_shared<xgraph::DiGraph<>>(*graph));
  REQUIRE(graph->EdgeSize() == N * (N - 1) + 1);
  REQUIRE(graph_copy->EdgeSize() == N * (N - 1) + 1);

  // Node tests
  REQUIRE(graph->HasNode("source"));

  REQUIRE(graph->HasNode(std::to_string(static_cast<int>(N / 2))));

  const auto s_node_test = graph->GetNode("source");
  REQUIRE(*s_node == *s_node_test);
  REQUIRE(xgraph::utils::NodePtrEqual(s_node, s_node_test));

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(N / 2)));
  REQUIRE(graph->Nodes().contains(some_node_test));

  graph->RemoveNode("source"); // also edges with it
  REQUIRE_FALSE(graph->HasNode("source"));
  REQUIRE(graph_copy->NodeSize() == N + 2);

  graph->AddNode(s_node);
  REQUIRE(graph->HasNode("source"));

  // Edge tests
  REQUIRE_FALSE(graph->HasEdge("source", "target", 2));

  graph->AddEdge("source", "target", 2);
  REQUIRE_FALSE(graph->HasEdge("source", "target"));
  REQUIRE(graph->HasEdge("source", "target", 2));
  REQUIRE_FALSE(graph->HasEdge("target", "source", 2));
  REQUIRE(graph_copy->EdgeSize() == N * (N - 1) + 1);

  graph->RemoveEdge("source", "target", 2);
  REQUIRE_FALSE(graph->HasEdge("source", "target", 2));

  graph->AddEdge("source", "target", 2);
  REQUIRE(graph->HasEdge("source", "target", 2));

  REQUIRE(graph->HasEdge(std::string("0"),
                         std::to_string(static_cast<int>(N / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge(std::string("0"), std::to_string(static_cast<int>(N / 2)));

  REQUIRE(graph->Edges().contains(s_t_edge_test));
  REQUIRE(graph->Edges().contains(some_edge_test));
  REQUIRE(graph->Nodes().contains(some_edge_test->Source()));
  REQUIRE(graph->Nodes().contains(some_edge_test->Target()));

  REQUIRE(graph->EdgeSize("0") == 2 * (N - 1));
  REQUIRE(graph->InEdgeSize("0") == N - 1);
  REQUIRE(graph->OutEdgeSize("0") == N - 1);

  REQUIRE(graph->OutEdges("0").contains(some_edge_test));

  REQUIRE(graph->Children("source").contains(t_node));

  REQUIRE(graph->Predecessor("target").contains(s_node));
  REQUIRE(graph->Successor("0").contains(some_node_test));

  REQUIRE(graph->Neighbors("target").contains(s_node));
}

TEST_CASE("Graph Structure", "Graph") {
  const auto graph = std::make_shared<xgraph::Graph<>>();
  REQUIRE_FALSE(graph->IsDirected());

  // Add nodes
  const auto s_node = std::make_shared<XNode<>>("source");
  const auto t_node = std::make_shared<XNode<>>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);

  for (int i = 0; i < N; ++i) {
    graph->AddNode(std::make_shared<XNode<>>(std::to_string(i)));
  }
  REQUIRE(graph->NodeSize() == N + 2);

  // Add edges
  graph->AddEdge("source", "target", 2);
  graph->AddEdge("target", "source", 2); // same as above
  graph->AddEdge("target", "source", 1); // same nodes with different weight

  for (int i = 0; i < N; ++i) {
    for (int j = i + 1; j < N; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }

  auto graph_copy(std::make_shared<xgraph::Graph<>>(*graph));
  REQUIRE(graph->EdgeSize() == N * (N - 1) / 2 + 2);
  REQUIRE(graph_copy->EdgeSize() == N * (N - 1) / 2 + 2);

  // Node tests
  REQUIRE(graph->HasNode("source"));
  REQUIRE(graph->HasNode(std::to_string(static_cast<int>(N / 2))));

  const auto s_node_test = graph->GetNode("source");
  REQUIRE(*s_node == *s_node_test);
  REQUIRE(xgraph::utils::NodePtrEqual(s_node, s_node_test));

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(N / 2)));
  REQUIRE(graph->Nodes().contains(some_node_test));

  graph->RemoveNode("source");
  REQUIRE_FALSE(graph->HasNode("source"));
  REQUIRE(graph_copy->NodeSize() == N + 2);

  graph->AddNode(s_node);
  REQUIRE(graph->HasNode("source"));

  // Edge tests
  REQUIRE_FALSE(graph->HasEdge("target", "source", 2));
  REQUIRE_FALSE(graph->HasEdge("target", "source", 1));

  graph->AddEdge("source", "target", 2);
  graph->AddEdge("target", "source", 2); // same as above
  graph->AddEdge("target", "source", 1); // same nodes with different weight
  REQUIRE(graph_copy->EdgeSize() == N * (N - 1) / 2 + 2);

  REQUIRE(graph->HasEdge("source", "target", 1));
  REQUIRE(graph->HasEdge("target", "source", 2));

  REQUIRE(graph->HasEdge(std::string("0"),
                         std::to_string(static_cast<int>(N / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge(std::string("0"), std::to_string(static_cast<int>(N / 2)));

  REQUIRE(graph->Edges().contains(s_t_edge_test));
  REQUIRE(graph->Edges().contains(some_edge_test));
  REQUIRE(graph->Nodes().contains(some_edge_test->Source()));
  REQUIRE(graph->Nodes().contains(some_edge_test->Target()));

  REQUIRE(graph->EdgeSize("0") == N - 1);
  REQUIRE(graph->InEdgeSize("0") == N - 1);
  REQUIRE(graph->OutEdgeSize("0") == N - 1);

  REQUIRE(graph->OutEdges("0").contains(some_edge_test));

  REQUIRE(graph->Children("source").contains(t_node));
  REQUIRE(graph->Children("0").size() == N - 1);

  REQUIRE(graph->Parents("0").size() == N - 1);

  REQUIRE(graph->Predecessor("target").contains(s_node));
  REQUIRE(graph->Successor("0").contains(some_node_test));

  REQUIRE(graph->Neighbors("target").contains(s_node));
  REQUIRE(graph->Neighbors("0").size() == N - 1);
}
