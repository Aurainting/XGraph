#include <string>

#include <gtest/gtest.h>

#include "../include/xgraph.hpp"


TEST(TestGraphStructure, DiGraph) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  const int n = 10;

  auto graph = std::make_shared<xgraph::DiGraph<>>();
  EXPECT_TRUE(graph->IsDirected());

  // Add nodes
  const auto s_node = std::make_shared<MyNode>("source");
  const auto t_node = std::make_shared<MyNode>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);

  for (int i = 0; i < n; ++i) {
    graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
  }

  EXPECT_EQ(graph->NodeSize(), n + 2);

  // Add edges
  graph->AddEdge(s_node, t_node, 2);

  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }

  EXPECT_EQ(graph->EdgeSize(), n * (n - 1) + 1);

  // Node tests
  EXPECT_TRUE(graph->HasNode("source"));

  EXPECT_TRUE(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

  const auto s_node_test = graph->GetNode("source");
  EXPECT_EQ(*s_node, *s_node_test);

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(n / 2)));
  EXPECT_TRUE(graph->Nodes().contains(some_node_test));

  graph->RemoveNode("source");  // also edges with it
  EXPECT_FALSE(graph->HasNode("source"));

  graph->AddNode(s_node);
  EXPECT_TRUE(graph->HasNode("source"));

  // Edge tests
  EXPECT_FALSE(graph->HasEdge("source", "target", 2));

  graph->AddEdge(s_node, t_node, 2);
  EXPECT_FALSE(graph->HasEdge("source", "target"));
  EXPECT_TRUE(graph->HasEdge("source", "target", 2));
  EXPECT_FALSE(graph->HasEdge("target", "source", 2));

  graph->RemoveEdge("source", "target", 2);
  EXPECT_FALSE(graph->HasEdge("source", "target", 2));

  graph->AddEdge("source", "target", 2);
  EXPECT_TRUE(graph->HasEdge("source", "target", 2));

  EXPECT_TRUE(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

  EXPECT_TRUE(graph->Edges().contains(s_t_edge_test));
  EXPECT_TRUE(graph->Edges().contains(some_edge_test));
  EXPECT_TRUE(graph->Nodes().contains(some_edge_test->Source()));
  EXPECT_TRUE(graph->Nodes().contains(some_edge_test->Target()));

  EXPECT_EQ(graph->EdgeSize("0"), 2 * (n - 1));
  EXPECT_EQ(graph->InEdgeSize("0"), n - 1);
  EXPECT_EQ(graph->OutEdgeSize("0"), n - 1);

  EXPECT_TRUE(graph->OutEdges("0").contains(some_edge_test));

  EXPECT_TRUE(graph->Children("source").contains(t_node));

  EXPECT_TRUE(graph->Predecessor("target").contains(s_node));
  EXPECT_TRUE(graph->Successor("0").contains(some_node_test));

  EXPECT_TRUE(graph->Neighbors("target").contains(s_node));
}

TEST(TestGraphStructure, Graph) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  const int n = 10;

  auto graph = std::make_shared<xgraph::Graph<>>();
  EXPECT_FALSE(graph->IsDirected());

  // Add nodes
  const auto s_node = std::make_shared<MyNode>("source");
  const auto t_node = std::make_shared<MyNode>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);

  for (int i = 0; i < n; ++i) {
    graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
  }
  EXPECT_EQ(graph->NodeSize(), n + 2);

  // Add edges
  graph->AddEdge(s_node, t_node, 2);
  graph->AddEdge(t_node, s_node, 2);  // same as above
  graph->AddEdge(t_node, s_node, 1);  // same nodes with different weight

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }
  EXPECT_EQ(graph->EdgeSize(), n * (n - 1) / 2 + 2);

  // Node tests
  EXPECT_TRUE(graph->HasNode("source"));
  EXPECT_TRUE(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

  const auto s_node_test = graph->GetNode("source");
  EXPECT_EQ(*s_node, *s_node_test);

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(n / 2)));
  EXPECT_TRUE(graph->Nodes().contains(some_node_test));

  graph->RemoveNode("source");
  EXPECT_FALSE(graph->HasNode("source"));

  graph->AddNode(s_node);
  EXPECT_TRUE(graph->HasNode("source"));

  // Edge tests
  EXPECT_FALSE(graph->HasEdge("target", "source", 2));
  EXPECT_FALSE(graph->HasEdge("target", "source", 1));

  graph->AddEdge(s_node, t_node, 2);
  graph->AddEdge(t_node, s_node, 2);  // same as above
  graph->AddEdge(t_node, s_node, 1);  // same nodes with different weight

  EXPECT_TRUE(graph->HasEdge("source", "target", 1));
  EXPECT_TRUE(graph->HasEdge("target", "source", 2));

  EXPECT_TRUE(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

  EXPECT_TRUE(graph->Edges().contains(s_t_edge_test));
  EXPECT_TRUE(graph->Edges().contains(some_edge_test));
  EXPECT_TRUE(graph->Nodes().contains(some_edge_test->Source()));
  EXPECT_TRUE(graph->Nodes().contains(some_edge_test->Target()));

  EXPECT_EQ(graph->EdgeSize("0"), n - 1);
  EXPECT_EQ(graph->InEdgeSize("0"), n - 1);
  EXPECT_EQ(graph->OutEdgeSize("0"), n - 1);

  EXPECT_TRUE(graph->OutEdges("0").contains(some_edge_test));

  EXPECT_TRUE(graph->Children("source").contains(t_node));
  EXPECT_EQ(graph->Children("0").size(), n - 1);

  EXPECT_EQ(graph->Parents("0").size(), n - 1);

  EXPECT_TRUE(graph->Predecessor("target").contains(s_node));
  EXPECT_TRUE(graph->Successor("0").contains(some_node_test));

  EXPECT_TRUE(graph->Neighbors("target").contains(s_node));
  EXPECT_EQ(graph->Neighbors("0").size(), n - 1);
}
