#pragma once

#include <iostream>

#include "edge.hpp"

namespace xgraph::utils {

/*!
 * @brief Hash function of node ptr
 * @tparam Node Input node type
 * @param n Input node ptr
 * @return Node id
 */
template <NodeType Node>
std::size_t NodePtrHash(const std::shared_ptr<Node>& n) {
  return n->Id();
}

/*!
 * @brief Equal function of two nodes ptr
 * @tparam Node Input node type
 * @param lhs Left node ptr
 * @param rhs Right node ptr
 * @return Whether two nodes ptr are equal
 */
template <NodeType Node>
bool NodePtrEqual(const std::shared_ptr<Node>& lhs,
                  const std::shared_ptr<Node>& rhs) {
  return *lhs == *rhs;
}

/*!
 * @brief Print node information
 * @tparam Node Input node type
 * @param n Input node
 * @param out Output stream
 */
template <NodeType Node>
void PrintNode(const Node& n, std::ostream& out = std::cout) {
  out << "Node : " << n.Name() << std::endl;
}

/*!
 * @brief Hash function of edge ptr in digraph
 * @tparam Edge Input edge type
 * @param e Input edge ptr
 * @return Edge hash value
 */
template <EdgeType Edge>
std::size_t DiEdgePtrHash(const std::shared_ptr<Edge>& e) {
  return NodePtrHash(e->Source()) << 2 ^ NodePtrHash(e->Target()) ^
         std::hash<double>{}(e->Weight());
}

/*!
 * @brief Equal function of two edges ptr in digraph
 * @tparam Edge Input edge type
 * @param lhs Left edge ptr
 * @param rhs Right edge ptr
 * @return Whether two edges ptr are equal
 */
template <EdgeType Edge>
bool DiEdgePtrEqual(const std::shared_ptr<Edge>& lhs,
                    const std::shared_ptr<Edge>& rhs) {
  return *lhs == *rhs;
}

/*!
 * @brief Hash function of edge ptr in graph
 * @tparam Edge Input edge type
 * @param e Input edge ptr
 * @return Edge hash value
 */
template <EdgeType Edge>
std::size_t EdgePtrHash(const std::shared_ptr<Edge>& e) {
  return NodePtrHash(e->Source()) ^ NodePtrHash(e->Target()) ^
         std::hash<double>{}(e->Weight());
}

/*!
 * @brief Reverse edge's source and target
 * @tparam Edge Input edge type
 * @param e Input edge ptr
 * @return Reversed edge ptr
 */
template <EdgeType Edge>
std::shared_ptr<Edge> ReverseEdgePtr(const std::shared_ptr<Edge>& e) {
  return std::make_shared<Edge>(e->Target(), e->Source(), e->Weight());
}

/*!
 * @brief Equal function of two edges ptr in graph
 * @tparam Edge Input edge type
 * @param lhs Left edge ptr
 * @param rhs Right edge ptr
 * @return Whether two edges ptr are equal
 */
template <EdgeType Edge>
bool EdgePtrEqual(const std::shared_ptr<Edge>& lhs,
                  const std::shared_ptr<Edge>& rhs) {
  return *lhs == *rhs || *ReverseEdgePtr(lhs) == *rhs;
}

/*!
 * @brief Print edge information
 * @tparam Edge Input edge type
 * @param e Input edge
 * @param out Output stream
 */
template <EdgeType Edge>
void PrintEdge(const Edge& e, std::ostream& out = std::cout) {
  out << "Edge : (" << e.Source()->Name() << ", " << e.Target()->Name() << ")"
      << std::endl;
}

} // namespace xgraph::utils
