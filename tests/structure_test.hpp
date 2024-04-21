#pragma once

#include <cassert>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>

#include "../include/xgraph.hpp"
#include "../include/utils/file_utils.hpp"


namespace xgraph::test {

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
    for (int j = 0; j < n; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }
  assert(graph->EdgeSize() == n * (n - 1) + 1);

  // Node tests
  assert(graph->HasNode("source"));
  assert(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

  const auto s_node_test = graph->GetNode("source");
  assert(*s_node == *s_node_test);

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(n / 2)));
  assert(graph->Nodes().contains(some_node_test));

  // Edge tests
  assert(!graph->HasEdge("source", "target"));
  assert(graph->HasEdge("source", "target", 2));
  assert(!graph->HasEdge("target", "source", 2));

  assert(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

  assert(graph->Edges().contains(s_t_edge_test));
  assert(graph->Edges().contains(some_edge_test));
  assert(graph->Nodes().contains(some_edge_test->Source()));
  assert(graph->Nodes().contains(some_edge_test->Target()));

  assert(graph->EdgeSize("0") == 2 * (n - 1));
  assert(graph->InEdgeSize("0") == n - 1);
  assert(graph->OutEdgeSize("0") == n - 1);

  assert(graph->OutEdges("0").contains(some_edge_test));

  assert(graph->Children("source").contains(t_node));

  assert(graph->Neighbors("target").contains(s_node));

  std::cout << "DiGraph with " << n << " nodes test succeed!" << std::endl;
}

void graph_test(const int n = 10) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  auto graph = std::make_shared<xgraph::Graph<>>();

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
  graph->AddEdge(t_node, s_node, 2);  // same as above
  graph->AddEdge(t_node, s_node, 1);  // same nodes with different weight

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      graph->AddEdge(std::to_string(i), std::to_string(j));
    }
  }
  assert(graph->EdgeSize() == n * (n - 1) / 2 + 2);

  // Node tests
  assert(graph->HasNode("source"));
  assert(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

  const auto s_node_test = graph->GetNode("source");
  assert(*s_node == *s_node_test);

  const auto some_node_test =
      graph->GetNode(std::to_string(static_cast<int>(n / 2)));
  assert(graph->Nodes().contains(some_node_test));

  // Edge tests
  assert(graph->HasEdge("source", "target", 1));
  assert(graph->HasEdge("target", "source", 2));

  assert(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

  const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
  const auto some_edge_test =
      graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

  assert(graph->Edges().contains(s_t_edge_test));
  assert(graph->Edges().contains(some_edge_test));
  assert(graph->Nodes().contains(some_edge_test->Source()));
  assert(graph->Nodes().contains(some_edge_test->Target()));

  assert(graph->EdgeSize("0") == n - 1);
  assert(graph->InEdgeSize("0") == n - 1);
  assert(graph->OutEdgeSize("0") == n - 1);

  assert(graph->OutEdges("0").contains(some_edge_test));

  assert(graph->Children("source").contains(t_node));
  assert(graph->Children("0").size() == n - 1);

  assert(graph->Parents("0").size() == n - 1);

  assert(graph->Neighbors("target").contains(s_node));
  assert(graph->Neighbors("0").size() == n - 1);

  std::cout << "Graph with " << n << " nodes test succeed!" << std::endl;
}

auto load_benchmark(const std::string& dir_path) {
  using xgraph::MyEdge;
  using xgraph::MyNode;

  auto graph = std::make_shared<xgraph::DiGraph<>>();

  if (dir_path.contains("undirected")) {
    graph = std::make_shared<xgraph::Graph<>>();
  }

  const auto& file_name = dir_path.substr(dir_path.rfind('/') + 1);

  std::ifstream node_file(dir_path + "/" + file_name + ".v");

  if (!node_file.is_open()) {
    std::cerr << "Can not open node file for " << file_name << std::endl;
    return graph;
  }

  std::ifstream edge_file(dir_path + "/" + file_name + ".e");

  if (!edge_file.is_open()) {
    std::cerr << "Can not open edge file for " << file_name << std::endl;
    return graph;
  }

  std::cout << "Begin load benchmark " << file_name << "\n";

  // Add nodes
  std::string line {};

  std::chrono::milliseconds node_load_t {0};
  while (std::getline(node_file, line)) {
    auto start = std::chrono::high_resolution_clock::now();

    graph->AddNode(line);

    auto end = std::chrono::high_resolution_clock::now();
    node_load_t += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  }

  // Add edges
  std::chrono::milliseconds edge_load_t {0};
  while (std::getline(edge_file, line)) {
    const auto split_res = xgraph::utils::splitString(line, ' ');

    auto start = std::chrono::high_resolution_clock::now();

    if (split_res.size() == 2) {
      graph->AddEdge(split_res[0], split_res[1]);
    } else if (split_res.size() == 3) {
      graph->AddEdge(split_res[0], split_res[1], std::stod(split_res[2]));
    }

    auto end = std::chrono::high_resolution_clock::now();
    edge_load_t += std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  }

  std::cout << "Nodes load time: " << node_load_t << "\n";
  std::cout << "Edges load time: " << edge_load_t << "\n";
  std::cout << "Load done." << std::endl;

  return graph;
}

}
