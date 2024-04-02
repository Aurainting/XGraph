#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "edge.hpp"
#include "node.hpp"
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
      : _nodes(1, utils::NodeHash<Node>, utils::NodeEqual<Node>),
        _edges(1, utils::DiEdgeHash<Edge>, utils::DiEdgeEqual<Edge>),
        _node_map() {}

  DiGraph(const std::function<std::size_t(const NodePtr&)>& node_hash,
          const std::function<bool(const NodePtr&, const NodePtr&)>& node_equal,
          const std::function<std::size_t(const EdgePtr&)>& edge_hash,
          const std::function<bool(const EdgePtr&, const EdgePtr&)>& edge_equal)
      : _nodes(1, node_hash, node_equal),
        _edges(1, edge_hash, edge_equal),
        _node_map() {}

  void AddNode(const NodePtr& n) { _nodes.insert(n); }

  virtual void AddEdge(const NodePtr& s, const NodePtr& t) {
    const auto e = std::make_shared<Edge>(s, t);

    if (const auto& i = _edges.find(e); i != _edges.end()) {
      // exist before
      _node_map[s->Id()][t->Id()] = std::weak_ptr<Edge>(*i);
    } else {
      // new element
      _edges.insert(e);
      _node_map[s->Id()][t->Id()] = std::weak_ptr<Edge>(e);
    }
  }

  auto Nodes() const { return _nodes; }

  [[nodiscard]] std::size_t NodeSize() const { return _nodes.size(); }

  auto Edges() const { return _edges; }

  [[nodiscard]] virtual std::size_t EdgeSize() const { return _edges.size(); }

  virtual std::size_t EdgeSize(const NodePtr& n) const {
    return InEdgeSize(n) + OutEdgeSize(n);
  }

  std::size_t InEdgeSize(const NodePtr& n) const {
    std::size_t res{0};

    for (const auto& i : _node_map) {
      res += i.second.count(n->Id());
    }

    return res;
  }

  std::size_t OutEdgeSize(const NodePtr& n) const {
    if (!_node_map.contains(n->Id())) {
      return 0;
    }

    return _node_map.at(n->Id()).size();
  }

  auto OutEdges(const NodePtr& n) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto& n_child = _node_map.find(n->Id());
        n_child != _node_map.end()) {
      for (const auto& i : n_child->second) {
        res.insert(i.second.lock());
      }
    }

    return res;
  }

  auto Children(const NodePtr& n) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add child nodes
    if (const auto& n_child = _node_map.find(n->Id());
        n_child != _node_map.end()) {
      for (const auto& i : n_child->second) {
        res.insert(i.second.lock()->Target());
      }
    }

    return res;
  }

  auto Parents(const NodePtr& n) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    for (const auto& i : _node_map) {
      if (const auto& n_parent = i.second.find(n->Id());
          n_parent != i.second.end()) {
        res.insert(n_parent->second.lock()->Source());
      }
    }

    return res;
  }

  auto Neighbors(const NodePtr& n) const {
    auto res = Children(n);
    res.merge(Parents(n));

    return res;
  }

 private:
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
      _nodes;

  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
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
                            utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>) {}

  void AddEdge(const NodePtr& s, const NodePtr& t) override {
    DiGraph<Node, Edge>::AddEdge(s, t);
    DiGraph<Node, Edge>::AddEdge(t, s);
  }

  [[nodiscard]] size_t EdgeSize() const override {
    return DiGraph<Node, Edge>::EdgeSize();
  }

  size_t EdgeSize(const NodePtr& n) const override {
    return DiGraph<Node, Edge>::OutEdgeSize(n);
  }
};

}  // namespace xgraph
