#pragma once

#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "edge.hpp"
#include "node.hpp"
#include "utils.hpp"

namespace xgraph {
/*!
 * Forward declaration
 */
template <NodeType Node, EdgeType Edge>
class Graph;

/*!
 * @brief Directed Graph
 * @tparam Node Node class that satisfy `NodeType` concept
 * @tparam Edge Edge class that satisfy `EdgeType` concept
 */
template <NodeType Node = XNode<>, EdgeType Edge = XEdge<>>
class DiGraph {
  using NodePtr = std::shared_ptr<Node>;
  using EdgePtr = std::shared_ptr<Edge>;
  using NodeAdj = std::unordered_map<std::size_t, std::weak_ptr<Edge>>;

 public:
  /*!
   * @brief Default constructor
   */
  DiGraph()
      : _nodes(1, utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>),
        _edges(1, utils::DiEdgePtrHash<Edge>, utils::DiEdgePtrEqual<Edge>),
        _adjacent(),
        _node_name() {}

  /*!
   * @brief Constructor
   * @param node_hash Hash function for node ptr
   * @param node_equal Equal function for two node ptr
   * @param edge_hash Hash function for edge ptr
   * @param edge_equal Equal function for two edge ptr
   */
  DiGraph(const std::function<std::size_t(const NodePtr&)>& node_hash,
          const std::function<bool(const NodePtr&, const NodePtr&)>& node_equal,
          const std::function<std::size_t(const EdgePtr&)>& edge_hash,
          const std::function<bool(const EdgePtr&, const EdgePtr&)>& edge_equal)
      : _nodes(1, node_hash, node_equal),
        _edges(1, edge_hash, edge_equal),
        _adjacent(),
        _node_name() {}

  /*!
   * @brief Copy constructor
   * @param other Other DiGraph
   */
  DiGraph(const DiGraph<Node, Edge>& other)
      : _nodes(1, other._nodes.hash_function(), other._nodes.key_eq()),
        _edges(1, other._edges.hash_function(), other._edges.key_eq()),
        _adjacent(),
        _node_name() {
    // Copy nodes
    for (const auto& n : other.Nodes()) {
      AddNode(n->Id(), n->Data());
    }

    // Copy edges
    for (const auto& e : other.Edges()) {
      AddEdge(e->Source()->Id(), e->Target()->Id(), e->Weight(), e->Data());
    }
  }

  /*!
   * @brief Move constructor
   * @param other Other DiGraph
   */
  DiGraph(DiGraph<Node, Edge>&& other) noexcept
      : _nodes(1, std::move(other._nodes.hash_function()),
               std::move(other._nodes.key_eq())),
        _edges(1, std::move(other._edges.hash_function()),
               std::move(other._edges.key_eq())),
        _adjacent(),
        _node_name() {
    // Copy nodes
    for (const auto& n : other.Nodes()) {
      AddNode(n->Id(), n->Data());
    }

    // Copy edges
    for (const auto& e : other.Edges()) {
      AddEdge(e->Source()->Id(), e->Target()->Id(), e->Weight(), e->Data());
    }
  }

  /*!
   * @brief Copy constructor
   * @param other Other Graph
   */
  explicit DiGraph(const Graph<Node, Edge>& other)
      : _nodes(1, utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>),
        _edges(1, utils::DiEdgePtrHash<Edge>, utils::DiEdgePtrEqual<Edge>),
        _adjacent(),
        _node_name() {
    for (const auto& n : other.Nodes()) {
      AddNode(n->Id(), n->Data());
    }

    for (const auto& e : other.Edges()) {
      AddEdge(e->Source()->Name(), e->Target()->Name(), e->Weight(), e->Data());
      AddEdge(e->Target()->Name(), e->Source()->Name(), e->Weight(), e->Data());
    }
  }

  /*!
   * @brief Move constructor
   * @param other Other Graph
   */
  explicit DiGraph(Graph<Node, Edge>&& other)
      : _nodes(1, utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>),
        _edges(1, utils::DiEdgePtrHash<Edge>, utils::DiEdgePtrEqual<Edge>),
        _adjacent(),
        _node_name() {
    for (const auto& n : other.Nodes()) {
      AddNode(n->Id(), n->Data());
    }

    for (const auto& e : other.Edges()) {
      AddEdge(e->Source()->Name(), e->Target()->Name(), e->Weight(), e->Data());
      AddEdge(e->Target()->Name(), e->Source()->Name(), e->Weight(), e->Data());
    }
  }

  DiGraph& operator=(const DiGraph& other) = delete;

  DiGraph& operator=(DiGraph&& other) = delete;

  /*!
   * @brief Destructor
   */
  virtual ~DiGraph() {
    _nodes.clear();
    _edges.clear();
    _adjacent.clear();
    _node_name.clear();
  }

  /*!
   * @brief Whether the graph is directed
   * @return true
   */
  [[nodiscard]] virtual bool IsDirected() const { return true; }

  /*!
   * @brief Add node ptr (no effect if exists already)
   * @param n Node ptr
   */
  void AddNode(const NodePtr& n) {
    const auto& [node_it, inserted] = _nodes.insert(n);

    if (inserted) {
      // Ensure the validity of the weak pointer.
      _node_name[(*node_it)->Name()] = std::weak_ptr<Node>(*node_it);
    }
  }

  /*!
   * @brief Construct and add node ptr
   * @tparam Args Arguments type (NOT NodePtr) to construct node
   * @param args Arguments to construct node
   */
  template <typename... Args>
    requires(!std::is_same_v<std::decay_t<Args>, NodePtr> && ...)
  void AddNode(Args&&... args) {
    AddNode(std::make_shared<Node>(std::forward<Args>(args)...));
  }

  /*!
   * @brief Remove node
   * @param n Node need to remove
   */
  void RemoveNode(const NodePtr& n) {
    _node_name.erase(n->Name());
    _nodes.erase(n);
  }

  /*!
   * @brief Remove node
   * @tparam T Argument type that can convert to call `GetNode`
   * @param n Node
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  void RemoveNode(T&& n) {
    if (const auto node_ptr = GetNode(std::forward<T>(n));
        node_ptr != nullptr) {
      // Remove edges
      for (const auto& e : Edges(std::forward<T>(n))) {
        RemoveEdge(e);
      }

      // Remove node
      RemoveNode(node_ptr);
    }
  }

  /*!
   * @brief Add edge ptr (no effect if exists already)
   * @param e Edge ptr
   */
  void AddEdge(const EdgePtr& e) {
    const auto& [edge_it, inserted] = _edges.insert(e);

    if (inserted) {
      // Ensure the validity of the weak pointer.
      _adjacent[(*edge_it)->Source()->Id()][(*edge_it)->Target()->Id()] =
          std::weak_ptr<Edge>(*edge_it);
    }
  }

  /*!
   * @brief Construct and add edge ptr
   * @tparam Args Auxiliary arguments type to construct edge
   * @param s_id source node id
   * @param t_id target node id
   * @param args Auxiliary arguments to construct edge
   */
  template <typename... Args>
  void AddEdge(const std::size_t& s_id, const std::size_t& t_id,
               Args&&... args) {
    const auto& s_node_ptr = GetNode(s_id);
    const auto& t_node_ptr = GetNode(t_id);
    AddEdge(std::make_shared<Edge>(std::weak_ptr<Node>(s_node_ptr),
                                   std::weak_ptr<Node>(t_node_ptr),
                                   std::forward<Args>(args)...));
  }

  /*!
   * @brief Construct and add edge ptr
   * @tparam Args Auxiliary arguments type to construct edge
   * @param s_name source node name
   * @param t_name target node name
   * @param args Auxiliary arguments to construct edge
   */
  template <typename... Args>
  void AddEdge(const std::string& s_name, const std::string& t_name,
               Args&&... args) {
    const auto& s_node_ptr = GetNode(s_name);
    const auto& t_node_ptr = GetNode(t_name);
    AddEdge(std::make_shared<Edge>(std::weak_ptr<Node>(s_node_ptr),
                                   std::weak_ptr<Node>(t_node_ptr),
                                   std::forward<Args>(args)...));
  }

  /*!
   * @brief Remove edge
   * @param e Edge need to remove
   */
  void RemoveEdge(const EdgePtr& e) {
    _adjacent.erase(e->Source()->Id());
    _edges.erase(e);
  }

  /*!
   * @brief Remove edge
   * @tparam T Argument type that can convert to call `GetEdge`
   * @param s Source
   * @param t Target
   * @param w Weight
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  void RemoveEdge(T&& s, T&& t, const double w = 1.0) {
    if (const auto edge_ptr =
            GetEdge(std::forward<T>(s), std::forward<T>(t), w);
        edge_ptr != nullptr) {
      RemoveEdge(edge_ptr);
    }
  }

  /*!
   * @brief Get node ptr according to id
   * @param id Node id
   * @return Node ptr if exists else nullptr
   */
  NodePtr GetNode(const std::size_t& id) const {
    if (const auto& res = _nodes.find(std::make_shared<Node>(id));
        res != _nodes.end()) {
      return *res;
    }
    return nullptr;
  }

  /*!
   * @brief Get node ptr according to name
   * @param name Node name
   * @return Node ptr if exists else nullptr
   */
  NodePtr GetNode(const std::string& name) const {
    if (const auto& res = _node_name.find(name); res != _node_name.end()) {
      return res->second.lock();
    }
    return nullptr;
  }

  /*!
   * @brief Whether there has the node
   * @tparam T Argument type that can convert to call `GetNode`
   * @param arg Argument refer to node
   * @return True if there has the node else false
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  [[nodiscard]] bool HasNode(T&& arg) const {
    return GetNode(std::forward<T>(arg)) != nullptr;
  }

  /*!
   * @brief Get const nodes
   * @return Nodes
   */
  auto Nodes() const { return _nodes; }

  /*!
   * @brief Get size of nodes
   * @return Size of nodes
   */
  [[nodiscard]] std::size_t NodeSize() const { return Nodes().size(); }

  /*!
   * @brief Get edge ptr
   * @tparam T Argument type that can convert to call `GetNode`
   * @param s Source node
   * @param t Target node
   * @param w Weight
   * @return Edge ptr if exists else nullptr
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  EdgePtr GetEdge(T&& s, T&& t, const double w = 1.0) const {
    const auto s_node_ptr = GetNode(std::forward<T>(s));
    const auto t_node_ptr = GetNode(std::forward<T>(t));
    if (s_node_ptr && t_node_ptr) {
      if (const auto& res = _edges.find(
              std::make_shared<Edge>(std::weak_ptr<Node>(s_node_ptr),
                                     std::weak_ptr<Node>(t_node_ptr), w));
          res != _edges.end()) {
        return *res;
      }
    }
    return nullptr;
  }

  /*!
   * @brief Whether there has the edge
   * @tparam T Argument type that can convert to call `GetEdge`
   * @param s Source node
   * @param t Target node
   * @param w Weight
   * @return True if there has the edge else false
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  [[nodiscard]] bool HasEdge(T&& s, T&& t, const double w = 1.0) const {
    return GetEdge(std::forward<T>(s), std::forward<T>(t), w) != nullptr;
  }

  /*!
   * @brief Get const edges
   * @return Edges
   */
  auto Edges() const { return _edges; }

  /*!
   * @brief Get edges bind to the node (in edges and out edges)
   * @tparam T Argument type that can convert to string or size_t
   * @param n Node
   * @return Edges
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  auto Edges(T&& n) const {
    auto res = DiGraph<Node, Edge>::InEdges(std::forward<T>(n));
    res.merge(DiGraph<Node, Edge>::OutEdges(std::forward<T>(n)));

    return res;
  }

  /*!
   * @brief Get in edges from the node
   * @param id Node id
   * @return In edges
   */
  virtual std::unordered_set<
      EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  InEdges(const std::size_t& id) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto node = GetNode(id)) {
      for (const auto& i : _adjacent) {
        if (const auto& n_parent = i.second.find(node->Id());
            n_parent != i.second.end()) {
          res.insert(n_parent->second.lock());
        }
      }
    }

    return res;
  }

  /*!
   * @brief Get in edges from the node
   * @param name Node name
   * @return In edges
   */
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

  /*!
   * @brief Get out edges from the node
   * @param id Node id
   * @return Out edges
   */
  virtual std::unordered_set<
      EdgePtr, std::function<std::size_t(const EdgePtr&)>,
      std::function<bool(const EdgePtr&, const EdgePtr&)>>
  OutEdges(const std::size_t& id) const {
    decltype(_edges) res(1, _edges.hash_function(), _edges.key_eq());

    if (const auto node = GetNode(id)) {
      if (const auto& n_child = _adjacent.find(node->Id());
          n_child != _adjacent.end()) {
        for (const auto& i : n_child->second) {
          res.insert(i.second.lock());
        }
      }
    }

    return res;
  }

  /*!
   * @brief Get out edges from the node
   * @param name Node name
   * @return Out edges
   */
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

  /*!
   * @brief Get size of all edges
   * @return Size of edges
   */
  [[nodiscard]] std::size_t EdgeSize() const { return Edges().size(); }

  /*!
   * @brief Get size of edges bind to the node
   * @tparam T Argument type that can convert to call `Edges`
   * @param n Node
   * @return Size of edges bind to the node
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  [[nodiscard]] std::size_t EdgeSize(T&& n) const {
    return Edges(std::forward<T>(n)).size();
  }

  /*!
   * @brief Get size of in edges from the node
   * @tparam T Argument type that can convert to string or size_t
   * @param n Node
   * @return Size of in edges
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  [[nodiscard]] std::size_t InEdgeSize(T&& n) const {
    return InEdges(std::forward<T>(n)).size();
  }

  /*!
   * @brief Get size of out edges from the node
   * @tparam T Argument type that can convert to string or size_t
   * @param n Node
   * @return Size of out edges
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  [[nodiscard]] std::size_t OutEdgeSize(T&& n) const {
    return OutEdges(std::forward<T>(n)).size();
  }

  /*!
   * @brief Get all parents nodes ptr from the node
   * @param id Node id
   * @return Parents nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::size_t& id) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    for (const auto in_edges = DiGraph<Node, Edge>::InEdges(id);
         const auto& i : in_edges) {
      res.insert(i->Source());
    }

    return res;
  }

  /*!
   * @brief Get all parents nodes ptr from the node
   * @param name Node name
   * @return Parents nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add parent nodes
    for (const auto in_edges = DiGraph<Node, Edge>::InEdges(name);
         const auto& i : in_edges) {
      res.insert(i->Source());
    }

    return res;
  }

  /*!
   * @brief Get all children nodes ptr from the node
   * @param id Node id
   * @return Children nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::size_t& id) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add child nodes
    for (const auto& out_edges = DiGraph<Node, Edge>::OutEdges(id);
         const auto& i : out_edges) {
      res.insert(i->Target());
    }

    return res;
  }

  /*!
   * @brief Get all children nodes ptr from the node
   * @param name Node name
   * @return Children nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Add child nodes
    for (const auto& out_edges = DiGraph<Node, Edge>::OutEdges(name);
         const auto& i : out_edges) {
      res.insert(i->Target());
    }

    return res;
  }

  /*!
   * @brief Get all predecessors of the node
   * @param id Node id
   * @return Predecessors nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::size_t& id) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_parents = DiGraph<Node, Edge>::Parents(id);
    res.merge(first_parents);

    std::queue<NodePtr> q(first_parents.cbegin(), first_parents.cend());

    // Add predecessor in a FIFO manner
    while (!q.empty()) {
      const auto n = q.front();

      auto n_parents = DiGraph<Node, Edge>::Parents(n->Id());

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

  /*!
   * @brief Get all predecessors of the node
   * @param name Node name
   * @return Predecessors nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_parents = DiGraph<Node, Edge>::Parents(name);
    res.merge(first_parents);

    std::queue<NodePtr> q(first_parents.cbegin(), first_parents.cend());

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

  /*!
   * @brief Get all successors of the node
   * @param id Node id
   * @return Successors nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::size_t& id) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_children = DiGraph<Node, Edge>::Children(id);
    res.merge(first_children);

    std::queue<NodePtr> q(first_children.cbegin(), first_children.cend());

    // Add successor in a FIFO manner
    while (!q.empty()) {
      const auto n = q.front();

      auto n_children = DiGraph<Node, Edge>::Children(n->Id());

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

  /*!
   * @brief Get all successors of the node
   * @param name Node name
   * @return Successors nodes
   */
  virtual std::unordered_set<
      NodePtr, std::function<std::size_t(const NodePtr&)>,
      std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::string& name) const {
    decltype(_nodes) res(1, _nodes.hash_function(), _nodes.key_eq());

    // Initialize the queue
    auto first_children = DiGraph<Node, Edge>::Children(name);
    res.merge(first_children);

    std::queue<NodePtr> q(first_children.cbegin(), first_children.cend());

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

  /*!
   * @brief Get the lineage of the node (predecessors and successors)
   * @tparam T Argument type that can convert to string or size_t
   * @param n Node
   * @return Lineage
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  auto NodeLineage(T&& n) const {
    auto res = DiGraph<Node, Edge>::Predecessor(std::forward<T>(n));
    res.merge(DiGraph<Node, Edge>::Successor(std::forward<T>(n)));

    return res;
  }

  /*!
   * @brief Get all neighbors of the node (parents and children)
   * @tparam T Argument type that can convert to string or size_t
   * @param n Node
   * @return Neighbors
   */
  template <typename T>
    requires(std::convertible_to<T, std::string> ||
             std::convertible_to<T, std::size_t>)
  auto Neighbors(T&& n) const {
    auto res = DiGraph<Node, Edge>::Parents(std::forward<T>(n));
    res.merge(DiGraph<Node, Edge>::Children(std::forward<T>(n)));

    return res;
  }

 private:
  //! @brief Nodes owner
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
      _nodes;

  //! @brief Edges owner
  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
                     std::function<bool(const EdgePtr&, const EdgePtr&)>>
      _edges;

  //! @brief Adjacent
  std::unordered_map<std::size_t, NodeAdj> _adjacent;

  //! @brief Node name mapping
  std::unordered_map<std::string, std::weak_ptr<Node>> _node_name;
};

/*!
 * @brief Undirected Graph
 * @tparam Node Node class that satisfy `NodeType` concept
 * @tparam Edge Edge class that satisfy `EdgeType` concept
 */
template <NodeType Node = XNode<>, EdgeType Edge = XEdge<>>
class Graph : public DiGraph<Node, Edge> {
  using NodePtr = std::shared_ptr<Node>;
  using EdgePtr = std::shared_ptr<Edge>;

 public:
  /*!
   * @brief Default constructor
   */
  Graph()
      : DiGraph<Node, Edge>(utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>,
                            utils::EdgePtrHash<Edge>,
                            utils::EdgePtrEqual<Edge>) {}

  /*!
   * @brief Default Destructor
   */
  ~Graph() override = default;

  /*!
   * @brief Whether the graph is directed
   * @return false
   */
  [[nodiscard]] bool IsDirected() const override { return false; }

  /*!
   * @brief Get in edges from the node
   * @param id Node id
   * @return In edges
   */
  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
                     std::function<bool(const EdgePtr&, const EdgePtr&)>>
  InEdges(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::Edges(id);
  }

  /*!
   * @brief Get in edges from the node
   * @param name Node name
   * @return In edges
   */
  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
                     std::function<bool(const EdgePtr&, const EdgePtr&)>>
  InEdges(const std::string& name) const override {
    return DiGraph<Node, Edge>::Edges(name);
  }

  /*!
   * @brief Get out edges from the node
   * @param id Node id
   * @return Out edges
   */
  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
                     std::function<bool(const EdgePtr&, const EdgePtr&)>>
  OutEdges(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::Edges(id);
  }

  /*!
   * @brief Get out edges from the node
   * @param name Node name
   * @return Out edges
   */
  std::unordered_set<EdgePtr, std::function<std::size_t(const EdgePtr&)>,
                     std::function<bool(const EdgePtr&, const EdgePtr&)>>
  OutEdges(const std::string& name) const override {
    return DiGraph<Node, Edge>::Edges(name);
  }

  /*!
   * @brief Get all parents nodes ptr from the node
   * @param id Node id
   * @return Parents nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::Neighbors(id);
  }

  /*!
   * @brief Get all parents nodes ptr from the node
   * @param name Node name
   * @return Parents nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Parents(const std::string& name) const override {
    return DiGraph<Node, Edge>::Neighbors(name);
  }

  /*!
   * @brief Get all children nodes ptr from the node
   * @param id Node id
   * @return Children nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::Neighbors(id);
  }

  /*!
   * @brief Get all children nodes ptr from the node
   * @param name Node name
   * @return Children nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Children(const std::string& name) const override {
    return DiGraph<Node, Edge>::Neighbors(name);
  }

  /*!
   * @brief Get all predecessors of the node
   * @param id Node id
   * @return Predecessors nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::NodeLineage(id);
  }

  /*!
   * @brief Get all predecessors of the node
   * @param name Node name
   * @return Predecessors nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Predecessor(const std::string& name) const override {
    return DiGraph<Node, Edge>::NodeLineage(name);
  }

  /*!
   * @brief Get all successors of the node
   * @param id Node id
   * @return Successors nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::size_t& id) const override {
    return DiGraph<Node, Edge>::NodeLineage(id);
  }

  /*!
   * @brief Get all successors of the node
   * @param name Node name
   * @return Successors nodes
   */
  std::unordered_set<NodePtr, std::function<std::size_t(const NodePtr&)>,
                     std::function<bool(const NodePtr&, const NodePtr&)>>
  Successor(const std::string& name) const override {
    return DiGraph<Node, Edge>::NodeLineage(name);
  }
};
}  // namespace xgraph
