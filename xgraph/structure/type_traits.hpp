#pragma once

#include <functional>
#include <memory>

#include "type_concepts.hpp"

namespace xgraph {

// Use for graph structure

/*!
 * @brief Type of hash function of node ptr
 * @tparam Node Input node type
 */
template <NodeType Node>
using NodePtrHash_t = std::function<std::size_t(const std::shared_ptr<Node>&)>;

/*!
 * @brief Type of equal function of two nodes ptr
 * @tparam Node Input node type
 */
template <NodeType Node>
using NodePtrEqual_t = std::function<bool(const std::shared_ptr<Node>&,
                                          const std::shared_ptr<Node>&)>;

/*!
 * @brief Type of hash function of edge ptr
 * @tparam Edge Input edge type
 */
template <EdgeType Edge>
using EdgePtrHash_t = std::function<std::size_t(const std::shared_ptr<Edge>&)>;

/*!
 * @brief Type of hash function of two edges ptr
 * @tparam Edge Input edge type
 */
template <EdgeType Edge>
using EdgePtrEqual_t = std::function<bool(const std::shared_ptr<Edge>&,
                                          const std::shared_ptr<Edge>&)>;

// Use for algorithm

/*!
 * @brief Type of node ptr visitor
 * @tparam Node Input node type
 */
template <NodeType Node>
using NodePtrVisitor_t = std::function<void(const std::shared_ptr<Node>&)>;

/*!
 * @brief Type of heuristic function
 * @tparam Node Input node type
 */
template <NodeType Node>
using Heuristic_t = std::function<double(const std::shared_ptr<Node>&,
                                         const std::shared_ptr<Node>&)>;

} // namespace xgraph
