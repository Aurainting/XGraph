#pragma once

#include <iostream>

#include "node.hpp"
#include "edge.hpp"

namespace xgraph::utils {

/*!
 * @brief Hash function of node
 * @tparam Node Input node type
 * @param n Input node
 * @return Node id
 */
template <NodeType Node>
std::size_t NodeHash(const std::shared_ptr<Node> &n) {
  return n->Id();
}

/*!
 * @brief Equal function of two nodes
 * @tparam Node Input node type
 * @param lhs Left node
 * @param rhs Right node
 * @return Boolean
 */
template <NodeType Node>
bool NodeEqual(const std::shared_ptr<Node> &lhs,
               const std::shared_ptr<Node> &rhs) {
  return *lhs == *rhs;
}

/*!
 * @brief Print node information
 * @tparam Node Input node type
 * @param n Input node
 * @param out Output stream
 */
template <NodeType Node>
void PrintNode(const std::shared_ptr<Node> &n, std::ostream &out = std::cout) {
  out << "Node : " << n->Name() << std::endl;
}

/*!
 * @brief Hash function of edge in digraph
 * @tparam Edge Input edge type
 * @param e Input edge
 * @return Edge hash
 */
template <EdgeType Edge>
std::size_t DiEdgeHash(const std::shared_ptr<Edge> &e) {
  return NodeHash(e->Source()) << 2 ^ NodeHash(e->Target()) ^ std::hash<double>{}(e->Weight());
}

/*!
 * @brief Equal function of edge in digraph
 * @tparam Edge Input edge type
 * @param lhs Left edge
 * @param rhs Right edge
 * @return Boolean
 */
template <EdgeType Edge>
bool DiEdgeEqual(const std::shared_ptr<Edge> &lhs,
                 const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs;
}

/*!
 * @brief Hash function of edge in graph
 * @tparam Edge Input edge type
 * @param e Input edge
 * @return Edge hash
 */
template <EdgeType Edge>
std::size_t EdgeHash(const std::shared_ptr<Edge> &e) {
  return NodeHash(e->Source()) ^ NodeHash(e->Target()) ^ std::hash<double>{}(e->Weight());
}

/*!
 * @brief Reverse edge's source and target
 * @tparam Edge Input edge type
 * @param e Input edge
 * @return Reversed edge
 */
template <EdgeType Edge>
std::shared_ptr<Edge> ReverseEdge(const std::shared_ptr<Edge> &e) {
  return std::make_shared<Edge>(e->Target(), e->Source(), e->Weight());
}

/*!
 * @brief Equal function of edge in graph
 * @tparam Edge Input edge type
 * @param lhs Left edge
 * @param rhs Right edge
 * @return Boolean
 */
template <EdgeType Edge>
bool EdgeEqual(const std::shared_ptr<Edge> &lhs,
               const std::shared_ptr<Edge> &rhs) {
  return *lhs == *rhs || *(ReverseEdge(lhs)) == *rhs;
}

/*!
 * @brief Print edge information
 * @tparam Edge Input edge type
 * @param e Input edge
 * @param out Output stream
 */
template <EdgeType Edge>
void PrintEdge(const std::shared_ptr<Edge> &e, std::ostream &out = std::cout) {
  out << "Edge : (" << e->Source()->Name() << ", " << e->Target()->Name() << ")"
      << std::endl;
}

}  // namespace xgraph::utils
