#include <string>
#include <random>

#include <gtest/gtest.h>

#include "../include/xgraph.hpp"

TEST(TestVisualize, tikz) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  const int n = 10;

  auto graph = std::make_shared<xgraph::DiGraph<>>();
  EXPECT_TRUE(graph->IsDirected());

  // Add nodes
  for (int i = 0; i < n; ++i) {
    graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
  }

  // Add edges
  std::random_device rd {};
  std::default_random_engine gen(rd());
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (std::generate_canonical<double, 12>(gen) < 0.5) {
        graph->AddEdge(std::to_string(i), std::to_string(j));
      }
    }
  }

  // xgraph::visual::show_tikz<MyNode, MyEdge<MyNode>>(*graph, "test.tex");
  xgraph::visual::show_tikz(*graph, "test.tex");
}
