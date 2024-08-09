#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "structure/graph.hpp"
#include "algorithm/traversal.hpp"

namespace xgraph::visual {

template <NodeType Node, EdgeType Edge>
void show_tikz(const DiGraph<Node, Edge>& graph, const std::string& file_path) {
  std::ofstream out_file(file_path, std::ios_base::out);

  if (!out_file) {
    std::cerr << "Can not open file: " << file_path << std::endl;
    return;
  }

  out_file << "\\documentclass{article}\n"
           << "\\usepackage{tikz}\n"
           << "\\begin{document}\n"
           << "\\usetikzlibrary{graphs}\n"
           << "\\tikz \\graph {\n";

  // TODO: Draw
  std::unordered_set<std::string> node_done;
  for (const auto& n : graph.Nodes()) {
    xgraph::algorithm::DFS<Node, Edge>(graph, n, [&node_done] (const auto& node_ptr) {
      if (!node_done.contains(node_ptr->Name())) {
        std::cout << "Visit: " << node_ptr->Name() << std::endl;
        node_done.insert(node_ptr->Name());
      }
    });
  }

  out_file << "};\n"
              << "\\end{document}\n";
}

template <NodeType Node, EdgeType Edge>
void show_tikz(const Graph<Node, Edge>& graph, const std::string& file_path) {
  std::ofstream out_file(file_path, std::ios_base::out);

  if (!out_file) {
    std::cerr << "Can not open file: " << file_path << std::endl;
    return;
  }

  // TODO: output tikz file to show graph
}

}  // namespace xgraph::visual
