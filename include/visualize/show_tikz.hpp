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

  // TODO: format nodes and edges

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
