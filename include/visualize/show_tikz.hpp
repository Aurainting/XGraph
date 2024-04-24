#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "../structure/graph.hpp"

namespace xgraph::visual {

template <NodeType Node, EdgeType Edge>
void show_tikz(const DiGraph<Node, Edge>& graph, const std::string& file_path) {
  std::ofstream out_file(file_path, std::ios_base::out);

  if (!out_file) {
    std::cerr << "Can not open file: " << file_path << std::endl;
    return;
  }
}

}  // namespace xgraph::visual
