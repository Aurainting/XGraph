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
template <NodeType Node = MyNode, EdgeType Edge = MyEdge<Node>>
class Graph {
  using NodePtr = std::shared_ptr<Node>;
  using NodeWPtr = std::weak_ptr<Node>;
  using EdgePtr = std::shared_ptr<Edge>;
  using EdgeWPtr = std::weak_ptr<Edge>;

  using NodeAdj = std::unordered_map<
      NodeWPtr,
      EdgeWPtr,
      std::function<std::size_t(const NodeWPtr&)>,
      std::function<bool(const NodeWPtr&, const NodeWPtr&)>>;

 public:
  Graph()
      : _nodes(1, utils::NodeHash<Node>, utils::NodeEqual<Node>)
      , _edges(1, utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>)
      , _node_map(1, utils::NodeWHash<Node>, utils::NodeWEqual<Node>)
  {}

  void AddNode(const Node &n) { AddNode(std::make_shared<Node>(n)); }

  void AddNode(const std::shared_ptr<Node> &n) { _nodes.insert(n); }

  void AddEdge(const Node &s, const Node &t) {
    AddEdge(std::make_shared<Node>(s), std::make_shared<Node>(t));
  }

  void AddEdge(const std::shared_ptr<Node> &s, const std::shared_ptr<Node> &t) {
    const auto e = std::make_shared<Edge>(s, t);

    _edges.insert(e);

    if (_node_map.contains(NodeWPtr(s))) {
      _node_map[NodeWPtr(s)][NodeWPtr(t)] = EdgeWPtr(e);
    } else {
      auto node_adj = NodeAdj(1, utils::NodeWHash<Node>, utils::NodeWEqual<Node>);
      node_adj[NodeWPtr(t)] = EdgeWPtr(e);
      _node_map[NodeWPtr(s)] = node_adj;
    }

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

  std::unordered_map<
      NodeWPtr,
      NodeAdj,
      std::function<std::size_t(const NodeWPtr &)>,
      std::function<bool(const NodeWPtr &,
                         const NodeWPtr &)>>
      _node_map;
};

}  // namespace xgraph
