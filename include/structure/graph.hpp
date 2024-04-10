#pragma once

#include <functional>
#include <memory>
#include <string>
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
        _adjacent(),
        _node_name() {}

  DiGraph(const std::function<std::size_t(const NodePtr&)>& node_hash,
          const std::function<bool(const NodePtr&, const NodePtr&)>& node_equal,
          const std::function<std::size_t(const EdgePtr&)>& edge_hash,
          const std::function<bool(const EdgePtr&, const EdgePtr&)>& edge_equal)
      : _nodes(1, node_hash, node_equal),
        _edges(1, edge_hash, edge_equal),
        _adjacent(),
        _node_name() {}

  DiGraph(const DiGraph& other)
      : _nodes(1, other._nodes.hash_function(), other._nodes.key_eq()),
        _edges(1, other._edges.hash_function(), other._edges.key_eq()),
        _adjacent(),
        _node_name() {
    // Copy nodes
    for (const auto& n : other._nodes) {
      _nodes.insert(std::make_shared<MyNode>(n->Name()));
    }

    // Copy edges
    for (const auto& e : other._edges) {
    }

    // Copy adjacent
    for (const auto& i : other._adjacent) {
    }

    // Copy node name
    for (const auto& i : other._node_name) {
    }
  }

  DiGraph& operator=(const DiGraph& other) = delete;

  ~DiGraph() {
    _nodes.clear();
    _edges.clear();
    _adjacent.clear();
    _node_name.clear();
  }

  void AddNode(const NodePtr& n) {
    const auto result = _nodes.insert(n);
    const auto node_ptr = *(result.first);

    // Ensure the validity of the weak pointer.
    _node_name[node_ptr->Name()] = std::weak_ptr<Node>(node_ptr);
  }

  void AddNode(const std::string& name) {
    AddNode(std::make_shared<Node>(name));
  }

  void AddEdge(const NodePtr& s, const NodePtr& t,
               const double w = 1.0) {
    const auto s_node_ptr = *(_nodes.insert(s).first);
    const auto t_node_ptr = *(_nodes.insert(t).first);
    const auto edge_ptr = *(_edges.insert(std::make_shared<Edge>(s_node_ptr, t_node_ptr, w)).first);

    // Ensure the validity of the weak pointer.
    _adjacent[edge_ptr->Source()->Id()][edge_ptr->Target()->Id()] = std::weak_ptr<Edge>(edge_ptr);
  }

  void AddEdge(const std::string& s_name, const std::string& t_name, const double w = 1.0) {
    AddEdge(std::make_shared<Node>(s_name), std::make_shared<Node>(t_name), w);
  }

  NodePtr GetNode(const std::string& name) const {
    if (const auto res = _node_name.find(name); res != _node_name.end()) {
      return res->second.lock();
    }
    return nullptr;
  }

  [[nodiscard]] bool HasNode(const std::string& name) const { return _node_name.contains(name); }

  auto Nodes() const { return _nodes; }

  [[nodiscard]] std::size_t NodeSize() const { return _nodes.size(); }

  EdgePtr GetEdge(const std::string& s_name, const std::string& t_name, const double w = 1.0) const {
    const auto s_node = GetNode(s_name);
    const auto t_node = GetNode(t_name);
    if (s_node && t_node) {
      if (const auto res = _edges.find(std::make_shared<Edge>(s_node, t_node, w));
          res != _edges.end()) {
        return *res;
      }
    }
    return nullptr;
  }

  [[nodiscard]] bool HasEdge(const std::string& s_name, const std::string& t_name, const double w = 1.0) const {
    return GetEdge(s_name, t_name, w) != nullptr;
  }

  auto Edges() const { return _edges; }

  [[nodiscard]] virtual std::size_t EdgeSize() const { return _edges.size(); }

  [[nodiscard]] virtual std::size_t EdgeSize(const std::string& n) const {
    return InEdgeSize(n) + OutEdgeSize(n);
  }

  [[nodiscard]] std::size_t InEdgeSize(const std::string& n) const {
    std::size_t res{0};

    if (const auto node = GetNode(n)) {
      for (const auto& i : _adjacent) {
        res += i.second.count(node->Id());
      }
    }

    return res;
  }

  [[nodiscard]] std::size_t OutEdgeSize(const std::string& n) const {
    const auto node = GetNode(n);

    if (!node || !_adjacent.contains(node->Id())) {
      return 0;
    }

    return _adjacent.at(node->Id()).size();
  }

  auto InEdges(const NodePtr& n) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    for (const auto& i : _adjacent) {
      if (const auto& n_parent = i.second.find(n->Id());
          n_parent != i.second.end()) {
        res.insert(n_parent->second.lock());
      }
    }

    return res;
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
    const auto out_edges = OutEdges(n);
    for (const auto& i : out_edges) {
      res.insert(i->Target());
    }

    return res;
  }

  auto Parents(const NodePtr& n) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    const auto in_edges = InEdges(n);
    for (const auto& i : in_edges) {
      res.insert(i->Source());
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

  std::unordered_map<std::string, std::weak_ptr<Node>> _node_name;
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

  [[nodiscard]] size_t EdgeSize() const override {
    return DiGraph<Node, Edge>::EdgeSize();
  }

  size_t EdgeSize(const NodePtr& n) const override {
    return DiGraph<Node, Edge>::OutEdgeSize(n);
  }
};

}  // namespace xgraph
