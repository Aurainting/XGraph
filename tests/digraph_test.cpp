#include <cassert>
#include <iostream>

#include "../include/xgraph.hpp"

auto complete_digraph_test(const int n) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  auto graph = std::make_shared<xgraph::DiGraph<>>();

  // Add nodes
  for (int i = 0; i < n; ++i) {
    graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
  }
  assert(graph->NodeSize() == n);

  // Add edges
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
    }
  }

  return graph;
}

int main() {
  auto g = complete_digraph_test(5);

  auto g_copy = xgraph::DiGraph(*g);

  std::cout << "Test Succeed!" << std::endl;
}
