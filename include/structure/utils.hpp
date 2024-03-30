#pragma once

#include "edge.hpp"

namespace xgraph::utils {

template <NodeType Node> std::size_t NodeHash(const std::shared_ptr<Node> &n) {
  return std::hash<std::size_t>{}(n->Id());
}

template <NodeType Node>
bool NodeEqual(const std::shared_ptr<Node> &lhs,
               const std::shared_ptr<Node> &rhs) {
  return *lhs == *rhs;
}

template <EdgeType Edge> std::size_t EdgeHash(const std::shared_ptr<Edge> &e) {
  const auto hasher = std::hash<std::size_t>{};
  return hasher(e->Source()->Id()) << 2 ^ hasher(e->Target()->Id());
}

template <EdgeType Edge>
bool EdgeEqual(const std::shared_ptr<Edge> &lhs,
               const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs;
}

} // namespace xgraph::utils
