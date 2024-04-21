#pragma once

#include <cassert>
#include <iostream>

#include "../include/xgraph.hpp"
#include "structure_test.hpp"

namespace xgraph::test {

void shortest_path_test () {
  // auto graph = load_benchmark("../../benchmark/test-sssp-directed");
  auto graph = load_benchmark("../../benchmark/wiki-Talk");
  const auto has_negative = std::any_of(
      graph->Edges().cbegin(), graph->Edges().cend(),
      [](const auto& edge_ptr) { return edge_ptr->Weight() < 0; });
  std::cout << "Has negative: " << std::boolalpha << has_negative << std::endl;
}

}
