#pragma once

#include <functional>
#include <memory>
#include <unordered_set>
#include <vector>

#include "edge.hpp"
#include "utils.hpp"
#include "xgraph_impl.hpp"


namespace xgraph {
template <NodeType Node = MyNode, EdgeType Edge = MyEdge<Node>>
class Graph {
 public:
  Graph()
      : _nodes(1, utils::NodeHash<Node>, utils::NodeEqual<Node>),
        _edges(1, utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>) {}

  void AddNode(const Node &n) { _nodes.insert(std::make_shared<Node>(n)); }

  void AddNode(const std::shared_ptr<Node> &n) { _nodes.insert(n); }

  void AddEdge(const Node &s, const Node &t) {
    _edges.insert(std::make_shared<Edge>(std::make_shared<Node>(s),
                                         std::make_shared<Node>(t)));
  }

  void AddEdge(const std::shared_ptr<Node> &s, const std::shared_ptr<Node> &t) {
    _edges.insert(std::make_shared<Edge>(s, t));
  }

  bool HasPath(const std::shared_ptr<Node> &s,
               const std::shared_ptr<Node> &t) const {}

  std::vector<std::shared_ptr<Edge>> ShortestPath(
      const std::shared_ptr<Node> &s, const std::shared_ptr<Node> &t) const {}

  std::vector<std::shared_ptr<Node>> BFS(
      const std::shared_ptr<Node> &s = nullptr) {}

 private:
  std::unordered_set<std::shared_ptr<Node>,
      std::function<std::size_t(const std::shared_ptr<Node> &)>,
      std::function<bool(const std::shared_ptr<Node> &,
                         const std::shared_ptr<Node> &)>>
      _nodes;
  std::unordered_set<std::shared_ptr<Edge>,
      std::function<std::size_t(const std::shared_ptr<Edge> &)>,
      std::function<bool(const std::shared_ptr<Edge> &,
                         const std::shared_ptr<Edge> &)>>
      _edges;
};

}  // namespace xgraph
