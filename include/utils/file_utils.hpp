#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace xgraph::utils {
std::vector<std::string> splitString(const std::string& str, char delimiter) {
  std::vector<std::string> tokens{};
  std::stringstream ss(str);
  std::string token{};

  while (std::getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
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

}  // namespace xgraph::utils
