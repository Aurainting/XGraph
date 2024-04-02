#pragma once

#include <vector>

#include "../structure/xgraph.hpp"

namespace xgraph::algo {

enum class SPMethod {
  Dijkstra,
  Bellman_Ford,
};

namespace impl {

void dijkstra() {}

void bellman_ford() {}

}  // namespace impl

template <NodeType Node, EdgeType Edge>
std::vector<std::weak_ptr<Edge>> shortest_path(
    const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
    const std::shared_ptr<Node>& target,
    const SPMethod& method = SPMethod::Dijkstra) {
  switch (method) {
    case SPMethod::Dijkstra:
      impl::dijkstra();
      break;
    case SPMethod::Bellman_Ford:
      impl::bellman_ford();
      break;
  }
}

}  // namespace xgraph::algo
