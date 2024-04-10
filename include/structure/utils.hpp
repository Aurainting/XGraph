#pragma once

#include <iostream>

#include "edge.hpp"

namespace xgraph::utils {

template <NodeType Node>
std::size_t NodeHash(const std::shared_ptr<Node> &n) {
  return std::hash<std::size_t>{}(n->Id()) ^
         std::hash<std::string>{}(n->Name());
}

template <NodeType Node>
bool NodeEqual(const std::shared_ptr<Node> &lhs,
               const std::shared_ptr<Node> &rhs) {
  return *lhs == *rhs;
}

template <NodeType Node>
void PrintNode(const std::shared_ptr<Node> &n, std::ostream &out = std::cout) {
  out << "Node : " << n->Name() << " (id = " << n->Id() << ")" << std::endl;
}

template <EdgeType Edge>
std::size_t DiEdgeHash(const std::shared_ptr<Edge> &e) {
  return NodeHash(e->Source()) << 2 ^ NodeHash(e->Target()) ^ std::hash<double>{}(e->Weight());
}

template <EdgeType Edge>
bool DiEdgeEqual(const std::shared_ptr<Edge> &lhs,
                 const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs;
}

template <EdgeType Edge>
std::size_t EdgeHash(const std::shared_ptr<Edge> &e) {
  return NodeHash(e->Source()) ^ NodeHash(e->Target()) ^ std::hash<double>{}(e->Weight());
}

template <EdgeType Edge>
std::shared_ptr<Edge> ReverseEdge(const std::shared_ptr<Edge> &e) {
  return std::make_shared<Edge>(e->Target(), e->Source(), e->Weight());
}

template <EdgeType Edge>
bool EdgeEqual(const std::shared_ptr<Edge> &lhs,
               const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs || *(ReverseEdge(lhs)) == *rhs;
}

template <EdgeType Edge>
void PrintEdge(const std::shared_ptr<Edge> &e, std::ostream &out = std::cout) {
  out << "Edge : (" << e->Source()->Name() << ", " << e->Target()->Name() << ")"
      << std::endl;
}

}  // namespace xgraph::utils
