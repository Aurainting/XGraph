#pragma once

#include <vector>
#include <numeric>
#include <queue>

#include "../structure/xgraph.hpp"

namespace xgraph::algo {

enum class SPMethod {
  Dijkstra,
  Bellman_Ford,
};

namespace impl {

template <NodeType Node, EdgeType Edge>
void dijkstra(
    const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
    const std::shared_ptr<Node>& target, std::vector<std::weak_ptr<Edge>>& res) {
  std::unordered_map<std::size_t, bool> visited;
  std::unordered_map<std::size_t, std::size_t> previous;
  std::unordered_map<std::size_t, double> distance;

  for (const auto& v : graph.Nodes()) {
    previous[v->Id()] = -1;
    distance[v->Id()] = std::numeric_limits<double>::infinity();
  }

  distance[source->Id()] = 0;

  const auto node_comp = [&distance] (const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) {
    return distance[lhs->Id()] > distance[rhs->Id()];
  };
  auto current_node = std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(node_comp)>(node_comp);

  current_node.emplace(source);

  while (!current_node.empty()) {
    const auto n = current_node.top();
    current_node.pop();

    if (visited[n->Id()]) {
      continue;
    }
    visited[n->Id()] = true;

    for (const auto& i : graph.OutEdges(n)) {
      const auto v = i->Target();
      if (!visited[v->Id()] && distance[v->Id()] > distance[n->Id()] + i->Weight()) {
        distance[v->Id()] + distance[n->Id()] + i->Weight();
        previous[v->Id()] = n->Id();

        current_node.push(v);
      }
    }
  }
}

template <NodeType Node, EdgeType Edge>
void bellman_ford(
    const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
    const std::shared_ptr<Node>& target, std::vector<std::weak_ptr<Edge>>& res) {}

}  // namespace impl

template <NodeType Node, EdgeType Edge>
std::vector<std::weak_ptr<Edge>> ShortestPath(
    const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
    const std::shared_ptr<Node>& target,
    const SPMethod& method = SPMethod::Dijkstra) {
  std::vector<std::weak_ptr<Edge>> res {};

  // Choose method
  switch (method) {
    case SPMethod::Dijkstra:
      impl::dijkstra(graph, source, target, res);
      break;
    case SPMethod::Bellman_Ford:
      impl::bellman_ford(graph, source, target, res);
      break;
  }

  return res;
}

}  // namespace xgraph::algo
