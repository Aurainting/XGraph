#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <queue>
#include <random>
#include <vector>

#include "../structure/graph.hpp"

namespace xgraph::algo {

enum class SPMethod {
  Dijkstra,
  Bellman_Ford,
  Random,
  Auto,
};

namespace impl {

/*!
 * @brief
 *
 * @tparam Node
 * @tparam Edge
 * @param graph
 * @param source
 * @param target
 * @param res
 */
template <NodeType Node, EdgeType Edge>
void dijkstra(const DiGraph<Node, Edge>& graph,
              const std::shared_ptr<Node>& source,
              const std::shared_ptr<Node>& target,
              std::vector<std::weak_ptr<Edge>>& res) {
  std::unordered_map<std::size_t, bool> visited;
  std::unordered_map<std::size_t, std::shared_ptr<Edge>> previous;
  std::unordered_map<std::size_t, double> distance;

  for (const auto& v : graph.Nodes()) {
    previous[v->Id()] = nullptr;
    distance[v->Id()] = std::numeric_limits<double>::infinity();
  }

  distance[source->Id()] = 0;

  const auto node_comp = [&distance](const std::shared_ptr<Node>& lhs,
                                     const std::shared_ptr<Node>& rhs) {
    return distance[lhs->Id()] > distance[rhs->Id()];
  };
  auto current_node = std::priority_queue<std::shared_ptr<Node>,
                                          std::vector<std::shared_ptr<Node>>,
                                          decltype(node_comp)>(node_comp);

  current_node.emplace(source);

  while (!current_node.empty() && target && !visited[target->Id()]) {
    const auto n = current_node.top();
    current_node.pop();

    if (visited[n->Id()]) {
      continue;
    }
    visited[n->Id()] = true;

    for (const auto& i : graph.OutEdges(n)) {
      const auto v = i->Target();
      if (!visited[v->Id()] &&
          distance[v->Id()] > distance[n->Id()] + i->Weight()) {
        distance[v->Id()] = distance[n->Id()] + i->Weight();
        previous[v->Id()] = i;

        current_node.emplace(v);
      }
    }
  }

  // Extract path
  if (target) {
    auto reverse_node = previous[target->Id()]->Source();
    res.emplace_back(std::weak_ptr<Edge>(previous[target->Id()]));

    while (reverse_node != source) {
      res.emplace_back(std::weak_ptr<Edge>(previous[reverse_node->Id()]));
      reverse_node = previous[reverse_node->Id()]->Source();
    }

    std::reverse(res.begin(), res.end());
  }
}

/*!
 * @brief
 *
 * @cite A Randomized Algorithm for Single-Source Shortest Path on Undirected
 * Real-Weighted Graphs (https://doi.org/10.1109/FOCS57990.2023.00035)
 *
 * @tparam Node
 * @tparam Edge
 * @param graph
 * @param source
 * @param target
 * @param res
 */
template <NodeType Node, EdgeType Edge>
void randomized_sssp(const Graph<Node, Edge>& graph,
                     const std::shared_ptr<Node>& source,
                     const std::shared_ptr<Node>& target,
                     std::vector<std::weak_ptr<Edge>>& res) {
  const auto n = graph.NodeSize();
  const auto m = graph.EdgeSize();

  const double k = std::sqrt(std::log2(static_cast<double>(n)) /
                             std::log2(std::log2(static_cast<double>(n))));

  std::random_device rd;
  std::default_random_engine gen(rd());

  // Improved Bundle Construction
  std::unordered_set<std::shared_ptr<Node>> R1;
  std::unordered_set<std::shared_ptr<Node>> not_R1;
  std::unordered_set<std::shared_ptr<Node>> R2;

  // Step 1
  for (const auto& i : graph.Nodes()) {
    if (i == source) {
      R1.insert(i);
    } else {
      if (std::generate_canonical<double, 32>(gen) < 1 / k) {
        R1.insert(i);
      } else {
        not_R1.insert(i);
      }
    }
  }

  // Step 2
  bool in_R1{false};
  std::unordered_map<std::size_t, std::vector<std::shared_ptr<Node>>> v_extract {};

  for (const auto& i : not_R1) {
    // Run Dijkstra
    std::unordered_map<std::size_t, bool> visited;
    std::unordered_map<std::size_t, double> distance;

    for (const auto& v : graph.Nodes()) {
      distance[v->Id()] = std::numeric_limits<double>::infinity();
    }

    distance[i->Id()] = 0;

    const auto node_comp = [&distance](const std::shared_ptr<Node>& lhs,
                                       const std::shared_ptr<Node>& rhs) {
      return distance[lhs->Id()] > distance[rhs->Id()];
    };
    auto current_node = std::priority_queue<std::shared_ptr<Node>,
                                            std::vector<std::shared_ptr<Node>>,
                                            decltype(node_comp)>(node_comp);

    current_node.emplace(i);

    while (!current_node.empty()) {
      const auto n = current_node.top();
      current_node.pop();

      if (visited[n->Id()]) {
        continue;
      }
      visited[n->Id()] = true;

      // Stop criteria 1
      if (R1.contains(n)) {
        in_R1 = true;
        break;
      }

      // Step 3_1
      v_extract[n->Id()].emplace_back(n);

      // Stop criteria 2
      if (v_extract[n->Id()].size() >= k * std::log2(k)) {
        // Step 3_2
        R2.insert(i);
        break;
      }
    }
  }

  // Step 4
  R1.merge(R2);  // R = R1

}

template <NodeType Node, EdgeType Edge>
void bellman_ford(const DiGraph<Node, Edge>& graph,
                  const std::shared_ptr<Node>& source,
                  const std::shared_ptr<Node>& target,
                  std::vector<std::weak_ptr<Edge>>& res) {
  std::unordered_map<std::size_t, std::shared_ptr<Edge>> previous;
  std::unordered_map<std::size_t, double> distance;

  for (const auto& n : graph.Nodes()) {
    distance[n->Id()] = std::numeric_limits<double>::infinity();
  }
  distance[source->Id()] = 0;

  for (std::size_t i = 0; i < graph.NodeSize(); ++i) {
    bool flag{false};
    for (const auto& e : graph.Edges()) {
      if (distance[e->Source()->Id()] ==
          std::numeric_limits<double>::infinity()) {
        continue;
      }
      if (distance[e->Target()->Id()] >
          distance[e->Source()->Id()] + e->Weight()) {
        flag = true;
        distance[e->Target()->Id()] = distance[e->Source()->Id()] + e->Weight();
        previous[e->Target()->Id()] = e;
      }
    }
    if (!flag) {
      break;
    }
  }
}

}  // namespace impl

template <NodeType Node, EdgeType Edge>
std::vector<std::weak_ptr<Edge>> ShortestPath(
    const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
    const std::shared_ptr<Node>& target = nullptr,
    const SPMethod& method = SPMethod::Auto) {
  std::vector<std::weak_ptr<Edge>> res{};

  // Choose method
  switch (method) {
    case SPMethod::Dijkstra:
      impl::dijkstra(graph, source, target, res);
      break;

    case SPMethod::Bellman_Ford:
      impl::bellman_ford(graph, source, target, res);
      break;

    case SPMethod::Random:
      impl::randomized_sssp(graph, source, target, res);
      break;

    case SPMethod::Auto:
      if (std::any_of(
              graph.Edges().cbegin(), graph.Edges().cend(),
              [](const auto& edge_ptr) { return edge_ptr->Weight() < 0; })) {
        // With negative weight
        impl::bellman_ford(graph, source, target, res);
      } else {
        // Without negative weight
        if (graph.IsDirected()) {
          impl::dijkstra(graph, source, target, res);
        } else {
          impl::randomized_sssp(graph, source, target, res);
        }
      }
      break;
  }

  return res;
}

}  // namespace xgraph::algo
