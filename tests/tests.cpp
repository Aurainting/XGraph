#include "structure_test.hpp"
#include "shortest_path_test.hpp"


int main() {
  xgraph::test::digraph_test();

  xgraph::test::graph_test();

  xgraph::test::shortest_path_test();

  std::cout << "Test Done!" << std::endl;
}