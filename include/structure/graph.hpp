#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

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

  [[nodiscard]] virtual bool IsDirected() const { return true; }

  void AddNode(const NodePtr& n) {
    const auto result = _nodes.insert(n);
    const auto node_ptr = *(result.first);

    // Ensure the validity of the weak pointer.
    _node_name[node_ptr->Name()] = std::weak_ptr<Node>(node_ptr);
  }

  void AddNode(const std::string& name) {
    AddNode(std::make_shared<Node>(name));
  }

  void AddEdge(const NodePtr& s, const NodePtr& t, const double w = 1.0) {
    const auto s_node_ptr = *(_nodes.insert(s).first);
    const auto t_node_ptr = *(_nodes.insert(t).first);

    if (s_node_ptr == t_node_ptr) {
      return;
    }

    const auto edge_ptr = *(
        _edges.insert(std::make_shared<Edge>(s_node_ptr, t_node_ptr, w)).first);

    // Ensure the validity of the weak pointer.
    _adjacent[edge_ptr->Source()->Id()][edge_ptr->Target()->Id()] =
        std::weak_ptr<Edge>(edge_ptr);
  }

  void AddEdge(const std::string& s_name, const std::string& t_name,
               const double w = 1.0) {
    AddEdge(std::make_shared<Node>(s_name), std::make_shared<Node>(t_name), w);
  }

  NodePtr GetNode(const std::string& name) const {
    if (const auto res = _node_name.find(name); res != _node_name.end()) {
      return res->second.lock();
    }
    return nullptr;
  }

  [[nodiscard]] bool HasNode(const std::string& name) const {
    return GetNode(name) != nullptr;
  }

  auto Nodes() const { return _nodes; }

  [[nodiscard]] std::size_t NodeSize() const { return Nodes().size(); }

  EdgePtr GetEdge(const std::string& s_name, const std::string& t_name,
                  const double w = 1.0) const {
    const auto s_node = GetNode(s_name);
    const auto t_node = GetNode(t_name);
    if (s_node && t_node) {
      if (const auto res =
            _edges.find(std::make_shared<Edge>(s_node, t_node, w));
          res != _edges.end()) {
        return *res;
      }
    }
    return nullptr;
  }

  [[nodiscard]] bool HasEdge(const std::string& s_name,
                             const std::string& t_name,
                             const double w = 1.0) const {
    return GetEdge(s_name, t_name, w) != nullptr;
  }

  auto Edges() const { return _edges; }

  auto Edges(const std::string& name) const {
    auto res = DiGraph<Node, Edge>::InEdges(name);
    res.merge(DiGraph<Node, Edge>::OutEdges(name));

    return res;
  }

  virtual std::unordered_set<
      EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  InEdges(const std::string& name) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto node = GetNode(name)) {
      for (const auto& i : _adjacent) {
        if (const auto& n_parent = i.second.find(node->Id());
            n_parent != i.second.end()) {
          res.insert(n_parent->second.lock());
        }
      }
    }

    return res;
  }

  virtual std::unordered_set<
      EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  OutEdges(const std::string& name) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto node = GetNode(name)) {
      if (const auto& n_child = _adjacent.find(node->Id());
          n_child != _adjacent.end()) {
        for (const auto& i : n_child->second) {
          res.insert(i.second.lock());
        }
      }
    }

    return res;
  }

  [[nodiscard]] std::size_t EdgeSize() const { return Edges().size(); }

  [[nodiscard]] std::size_t EdgeSize(const std::string& name) const {
    return Edges(name).size();
  }

  [[nodiscard]] std::size_t InEdgeSize(const std::string& name) const {
    return InEdges(name).size();
  }

  [[nodiscard]] std::size_t OutEdgeSize(const std::string& name) const {
    return OutEdges(name).size();
  }

  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add child nodes
    const auto out_edges = DiGraph<Node, Edge>::OutEdges(name);
    for (const auto& i : out_edges) {
      res.insert(i->Target());
    }

    return res;
  }

  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    const auto in_edges = DiGraph<Node, Edge>::InEdges(name);
    for (const auto& i : in_edges) {
      res.insert(i->Source());
    }

    return res;
  }

  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_parents = DiGraph<Node, Edge>::Parents(name);

    std::queue<NodePtr> q(first_parents.cbegin(), first_parents.cend());
    res.merge(first_parents);

    // Add predecessor in a FIFO manner
    while (!q.empty()) {
      const auto n = q.front();
      auto n_parents = DiGraph<Node, Edge>::Parents(n->Name());

      for (auto& p : n_parents) {
        if (!res.contains(p)) {
          q.push(p);
        }
      }
      res.merge(n_parents);

      q.pop();
    }

    return res;
  }

  virtual
  std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_children = DiGraph<Node, Edge>::Children(name);

    std::queue<NodePtr> q(first_children.cbegin(), first_children.cend());
    res.merge(first_children);

    // Add successor in a FIFO manner
    while (!q.empty()) {
      const auto n = q.front();
      auto n_children = DiGraph<Node, Edge>::Children(n->Name());

      for (auto& p : n_children) {
        if (!res.contains(p)) {
          q.push(p);
        }
      }
      res.merge(n_children);

      q.pop();
    }

    return res;
  }

  auto LinkNodes(const std::string& name) const {
    auto res = DiGraph<Node, Edge>::Predecessor(name);
    res.merge(DiGraph<Node, Edge>::Successor(name));

    return res;
  }

  auto Neighbors(const std::string& name) const {
    auto res = DiGraph<Node, Edge>::Children(name);
    res.merge(DiGraph<Node, Edge>::Parents(name));

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

 public:
  Graph()
      : DiGraph<Node, Edge>(utils::NodeHash<Node>, utils::NodeEqual<Node>,
                            utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>) {}

  [[nodiscard]] bool IsDirected() const override { return false; }

  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  InEdges(const std::string& n) const override {
    return DiGraph<Node, Edge>::Edges(n);
  }

  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  OutEdges(const std::string& n) const override {
    return DiGraph<Node, Edge>::Edges(n);
  }

  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::string& n) const override {
    return DiGraph<Node, Edge>::Neighbors(n);
  }

  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::string& n) const override {
    return DiGraph<Node, Edge>::Neighbors(n);
  }

  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::string& n) const override {
    return DiGraph<Node, Edge>::LinkNodes(n);
  }

  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::string& n) const override {
    return DiGraph<Node, Edge>::LinkNodes(n);
  }
};

}  // namespace xgraph
