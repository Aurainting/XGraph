#pragma once

#include <functional>
#include <memory>
#include <string_view>
#include <tuple>
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
        _adjacent() {}

  DiGraph(const std::function<std::size_t(const NodePtr&)>& node_hash,
          const std::function<bool(const NodePtr&, const NodePtr&)>& node_equal,
          const std::function<std::size_t(const EdgePtr&)>& edge_hash,
          const std::function<bool(const EdgePtr&, const EdgePtr&)>& edge_equal)
      : _nodes(1, node_hash, node_equal),
        _edges(1, edge_hash, edge_equal),
        _adjacent() {}

  DiGraph(const DiGraph& other)
      : _nodes(1, other._nodes.hash_function(), other._nodes.key_eq()),
        _edges(1, other._edges.hash_function(), other._edges.key_eq()),
        _adjacent() {
    // Copy nodes
    for (const auto& n : other._nodes) {
      _nodes.insert(std::make_shared<MyNode>(n->Name()));
    }

    // Copy edges
    for (const auto& e : other._edges) {
    }

    // Copy node adjacent
    for (const auto& i : other._adjacent) {
    }
  }

  DiGraph& operator=(const DiGraph& other) = delete;

  ~DiGraph() {
    _nodes.clear();
    _edges.clear();
    _adjacent.clear();
  }

  void AddNode(const NodePtr& n) {
    const auto result = _nodes.insert(n);
    const auto node_ptr = *(result.first);
    _node_name.insert({node_ptr->Name(), std::weak_ptr<Node>(node_ptr)});
  }

  virtual void AddEdge(const NodePtr& s, const NodePtr& t) {
    const auto e = std::make_shared<Edge>(s, t);

    if (const auto& i = _edges.find(e); i != _edges.end()) {
      // exist before
      _adjacent[s->Id()][t->Id()] = std::weak_ptr<Edge>(*i);
    } else {
      // new element
      _edges.insert(e);
      _adjacent[s->Id()][t->Id()] = std::weak_ptr<Edge>(e);
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

    for (const auto& i : _adjacent) {
      res += i.second.count(n->Id());
    }

    return res;
  }

  std::size_t OutEdgeSize(const NodePtr& n) const {
    if (!_adjacent.contains(n->Id())) {
      return 0;
    }

    return _adjacent.at(n->Id()).size();
  }

  auto OutEdges(const NodePtr& n) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto& n_child = _adjacent.find(n->Id());
        n_child != _adjacent.end()) {
      for (const auto& i : n_child->second) {
        res.insert(i.second.lock());
      }
    }

    return res;
  }

  auto Children(const NodePtr& n) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add child nodes
    if (const auto& n_child = _adjacent.find(n->Id());
        n_child != _adjacent.end()) {
      for (const auto& i : n_child->second) {
        res.insert(i.second.lock()->Target());
      }
    }

    return res;
  }

  auto Parents(const NodePtr& n) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    for (const auto& i : _adjacent) {
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

  std::unordered_map<std::size_t, NodeAdj> _adjacent;

  std::unordered_map<std::string_view, std::weak_ptr<Node>> _node_name;
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
