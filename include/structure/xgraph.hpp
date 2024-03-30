#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "node.hpp"
#include "edge.hpp"
#include "utils.hpp"
#include "xgraph_impl.hpp"


namespace xgraph {
/*!
 * Directed Graph
 * @tparam Node Node class that satisfy `NodeType` concept
 * @tparam Edge Edge class that satisfy `EdgeType` concept
 */
template <NodeType Node = MyNode, EdgeType Edge = MyEdge<Node>>
class DiGraph {
  using NodePtr = std::shared_ptr<Node>;
  using EdgePtr = std::shared_ptr<Edge>;
  using NodeAdj = std::unordered_map<std::size_t, std::weak_ptr<Edge>>;

 public:
  DiGraph()
      : _nodes(1, utils::NodeHash<Node>, utils::NodeEqual<Node>)
      , _edges(1, utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>)
      , _node_map()
  {}

  void AddNode(const Node &n) { AddNode(std::make_shared<Node>(n)); }

  void AddNode(const std::shared_ptr<Node> &n) { _nodes.insert(n); }

  void AddEdge(const Node &s, const Node &t) {
    AddEdge(std::make_shared<Node>(s), std::make_shared<Node>(t));
  }

  virtual void AddEdge(const std::shared_ptr<Node> &s, const std::shared_ptr<Node> &t) {
    const auto e = std::make_shared<Edge>(s, t);
    _edges.insert(e);
    _node_map[s->Id()][t->Id()] = std::weak_ptr<Edge>(e);
  }

  auto NodeSize() const {
    return _nodes.size();
  }

  auto EdgeSize() const {
    return _edges.size();
  }

 private:
  std::unordered_set<
      NodePtr,
      std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
      _nodes;

  std::unordered_set<
      EdgePtr,
      std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
      _edges;

  std::unordered_map<std::size_t, NodeAdj> _node_map;
};

/*!
 * Undirected Graph
 * @tparam Node
 * @tparam Edge
 */
template <NodeType Node = MyNode, EdgeType Edge = MyEdge<Node>>
class Graph : public DiGraph<Node, Edge> {

};

}  // namespace xgraph
