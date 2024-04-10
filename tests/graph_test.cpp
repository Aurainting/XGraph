#include <cassert>
#include <iostream>

#include "../include/xgraph.hpp"

void digraph_test(const int n = 10) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  auto graph = std::make_shared<xgraph::DiGraph<>>();

  // Add nodes
  const auto s_node = std::make_shared<MyNode>("source");
  const auto t_node = std::make_shared<MyNode>("target");

  graph->AddNode(s_node);
  graph->AddNode(t_node);

  for (int i = 0; i < n; ++i) {
    graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
  }
  assert(graph->NodeSize() == n + 2);

  // Add edges
  graph->AddEdge(s_node, t_node, 2);

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }
  assert(graph->EdgeSize() == n * (n-1) / 2 + 1);

  // Node tests
  assert(graph->HasNode("source"));
  assert(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

  const auto s_node_test = graph->GetNode("source");
  assert(*s_node == *s_node_test);

  const auto some_node_test = graph->GetNode(std::to_string(static_cast<int>(n / 2)));
  assert(graph->Nodes().contains(some_node_test));

  // Edge tests
  assert(!graph->HasEdge("source", "target"));
  assert(graph->HasEdge("source", "target", 2));
  assert(!graph->HasEdge("target", "source", 2));

  assert(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test = graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

  assert(graph->Edges().contains(s_t_edge_test));
  assert(graph->Edges().contains(some_edge_test));
  assert(graph->Nodes().contains(some_edge_test->Source()));
  assert(graph->Nodes().contains(some_edge_test->Target()));

  assert(graph->EdgeSize("0") == n - 1);
  assert(graph->InEdgeSize("0") == 0);
  assert(graph->OutEdgeSize("0") == n - 1);

  assert(graph->OutEdges("0").contains(some_edge_test));

  assert(graph->Children("source").contains(t_node));

  assert(graph->Neighbors("target").contains(s_node));

  std::cout << "DiGraph with " << n << " nodes test succeed!" << std::endl;
}

void graph_test(const int n = 10) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  auto graph = std::make_shared<xgraph::DiGraph<>>();

  // Add nodes

  std::cout << "Graph with " << n << " nodes test succeed!" << std::endl;
}

int main() {
  digraph_test();

  graph_test();

  std::cout << "Test Done!" << std::endl;
}
