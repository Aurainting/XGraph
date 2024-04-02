#include <iostream>

#include "include/structure/node.hpp"
#include "include/structure/xgraph.hpp"
#include "include/algorithm/traversal.hpp"
#include "include/algorithm/shortest_path.hpp"


int main() {
  auto g = new xgraph::DiGraph();

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

  auto n0_neigh = g->Neighbors(n_0);

  xgraph::algo::BFS<xgraph::MyNode, xgraph::MyEdge<xgraph::MyNode>>(
      *g, n_0, [](const std::shared_ptr<xgraph::MyNode>& n) {
        xgraph::utils::PrintNode(n);
      });

  xgraph::algo::DFS<xgraph::MyNode, xgraph::MyEdge<xgraph::MyNode>>(
      *g, n_0, [](const std::shared_ptr<xgraph::MyNode>& n) {
        xgraph::utils::PrintNode(n);
      });

  const auto result = xgraph::algo::ShortestPath(*g, n_3, n_2);

  delete g;

  std::cout << "hello, xgraph!" << std::endl;
}
