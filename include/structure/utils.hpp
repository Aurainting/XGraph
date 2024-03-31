#pragma once

#include <iostream>

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

template <NodeType Node>
void PrintNode(const std::shared_ptr<Node>& n, std::ostream& out = std::cout) {
  out << "Node : " << n->Id() << std::endl;
}

template <EdgeType Edge> std::size_t DiEdgeHash(const std::shared_ptr<Edge> &e) {
  const auto hasher = std::hash<std::size_t>{};
  return hasher(e->Source()->Id()) << 2 ^ hasher(e->Target()->Id());
}

template <EdgeType Edge>
bool DiEdgeEqual(const std::shared_ptr<Edge> &lhs,
                 const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs;
}

template <EdgeType Edge> std::size_t EdgeHash(const std::shared_ptr<Edge> &e) {
  const auto hasher = std::hash<std::size_t>{};
  return hasher(e->Source()->Id()) ^ hasher(e->Target()->Id());
}

template <EdgeType Edge>
std::shared_ptr<Edge> SwapEdge(const std::shared_ptr<Edge> &e) {
  return std::make_shared<Edge>(e->Target(), e->Source());
}

template <EdgeType Edge>
bool EdgeEqual(const std::shared_ptr<Edge> &lhs,
               const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs || *(SwapEdge(lhs)) == *rhs;
}

template <EdgeType Edge>
void PrintEdge(const std::shared_ptr<Edge>& e, std::ostream& out = std::cout) {
  out << "Edge : (" << e->Source() << ", " << e->Target() << ")" << std::endl;
}

} // namespace xgraph::utils
