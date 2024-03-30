#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "node.hpp"
#include "edge.hpp"
#include "utils.hpp"


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
      , _edges(1, utils::DiEdgeHash<Edge>, utils::DiEdgeEqual<Edge>)
      , _node_map()
  {}

  DiGraph(const std::function<std::size_t(const NodePtr&)>& node_hash,
          const std::function<bool(const NodePtr&, const NodePtr&)>& node_equal,
          const std::function<std::size_t(const EdgePtr&)>& edge_hash,
          const std::function<bool(const EdgePtr&, const EdgePtr&)>& edge_equal)
      : _nodes(1, node_hash, node_equal)
      , _edges(1, edge_hash, edge_equal)
      , _node_map()
  {}

  void AddNode(const NodePtr &n) { _nodes.insert(n); }

  virtual void AddEdge(const NodePtr &s, const NodePtr &t) {
    const auto e = std::make_shared<Edge>(s, t);
    _edges.insert(e);
    _node_map[s->Id()][t->Id()] = std::weak_ptr<Edge>(e);
  }

  [[nodiscard]] std::size_t NodeSize() const {
    return _nodes.size();
  }

  [[nodiscard]] virtual std::size_t EdgeSize() const {
    return _edges.size();
  }

  virtual std::size_t EdgeSize(const Node& n) const {
    return InEdgeSize(n) + OutEdgeSize(n);
  }

  std::size_t InEdgeSize(const Node& n) const {
    std::size_t res {0};

    for (const auto& i : _node_map) {
      res += i.second.count(n.Id());
    }

    return res;
  }

  std::size_t OutEdgeSize(const Node& n) const {
    return _node_map.at(n.Id()).size();
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
  using NodePtr = std::shared_ptr<Node>;
  using EdgePtr = std::shared_ptr<Edge>;
  using NodeAdj = std::unordered_map<std::size_t, std::weak_ptr<Edge>>;

 public:
  Graph()
      : DiGraph<Node, Edge>(utils::NodeHash<Node>, utils::NodeEqual<Node>,
                            utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>)
  {}

  void AddEdge(const NodePtr& s, const NodePtr& t) override {
    DiGraph<Node, Edge>::AddEdge(s, t);
    DiGraph<Node, Edge>::AddEdge(t, s);
  }

  [[nodiscard]] size_t EdgeSize() const override {
    return DiGraph<Node, Edge>::EdgeSize();
  }

  size_t EdgeSize(const Node& n) const override {
    return DiGraph<Node, Edge>::OutEdgeSize(n);
  }
};

}  // namespace xgraph
