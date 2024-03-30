#include <iostream>

#include "include/structure/node.hpp"
#include "include/structure/xgraph.hpp"

int main() {
  // xgraph::Graph g;
  auto g = new xgraph::Graph();

  // Add Nodes
  const auto n_0 = std::make_shared<xgraph::MyNode>("0");
  const auto n_1 = std::make_shared<xgraph::MyNode>("1");
  const auto n_2 = std::make_shared<xgraph::MyNode>("2");
  const auto n_3 = std::make_shared<xgraph::MyNode>("3");
  const auto n_4 = std::make_shared<xgraph::MyNode>("4");

  g->AddNode(n_0);
  g->AddNode(n_1);
  g->AddNode(n_2);
  g->AddNode(n_3);
  g->AddNode(n_4);

  // Add Edges
  g->AddEdge(n_0, n_1);
  g->AddEdge(n_0, n_2);
  g->AddEdge(n_0, n_3);
  g->AddEdge(n_0, n_4);
  g->AddEdge(n_4, n_0);
  g->AddEdge(n_3, n_4);

  std::cout << g->EdgeSize() << std::endl;

  delete g;

  std::cout << "hello, xgraph!" << std::endl;
}
