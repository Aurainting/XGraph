#pragma once

#include <algorithm>
#include <format>
#include <optional>
#include <queue>
#include <unordered_map>
#include <vector>

#include "structure/graph.hpp"
#include "structure/type_traits.hpp"
#include "structure/utils.hpp"

namespace xgraph::algorithm {

template <NodeType Node, EdgeType Edge>
std::vector<std::shared_ptr<Node>>
AStarPath(const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
          const std::shared_ptr<Node>& target,
          const std::optional<Heuristic_t<Node>>& heuristic = std::nullopt) {
  // Type definition
  using ElemType = std::tuple<double,                // priority
                              std::shared_ptr<Node>, // current node
                              double,                // cost to reach
                              std::shared_ptr<Node>  // parent
                              >;
  using ElemCompType = std::function<bool(const ElemType&, const ElemType&)>;

  // Priority queue
  std::priority_queue<ElemType, std::vector<ElemType>, ElemCompType> queue(
      [](const ElemType& lhs, const ElemType& rhs) -> bool {
        return std::get<0>(lhs) > std::get<0>(rhs);
      });
  queue.push({0.0, source, 0.0, nullptr});

  // Maps enqueued nodes to distance of discovered paths and the computed
  // heuristics to target
  std::unordered_map<std::shared_ptr<Node>, std::pair<double, double>,
                     NodePtrHash_t<Node>, NodePtrEqual_t<Node>>
      enqueued{1, utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>};
  // Maps explored nodes to parent closet to the source
  std::unordered_map<std::shared_ptr<Node>, std::shared_ptr<Node>,
                     NodePtrHash_t<Node>, NodePtrEqual_t<Node>>
      explored{1, utils::NodePtrHash<Node>, utils::NodePtrEqual<Node>};

  while (!queue.empty()) {
    const auto [_, cur_node, dist, parent] = queue.top();
    queue.pop();

    if (utils::NodePtrEqual(cur_node, target)) { // find the target
      std::vector path{cur_node};
      auto node = parent;
      while (node) {
        path.push_back(node);
        node = explored[node];
      }
      std::ranges::reverse(path);
      return path;
    }

    if (explored.contains(cur_node)) {
      // Do not override the parent of starting node
      if (!explored.at(cur_node)) {
        continue;
      }

      // Skip bad paths that were enqueued before finding a better one
      if (const auto& [q_cost, h] = enqueued.at(cur_node); q_cost < dist) {
        continue;
      }
    }

    explored[cur_node] = parent;

    for (const auto& out_edge : graph.OutEdges(cur_node->Id())) {
      const auto& cost = out_edge->Weight();
      const auto new_cost = dist + cost;
      const auto& neighbor = out_edge->Target();
      double h{0.0};
      if (enqueued.contains(neighbor)) {
        const auto [q_cost, tmp_h] = enqueued.at(neighbor);
        if (q_cost <= new_cost) {
          continue;
        }
        h = tmp_h;
      } else if (heuristic.has_value()) {
        h = heuristic.value()(neighbor, target);
      }

      enqueued[neighbor] = {new_cost, h};
      queue.emplace(new_cost + h, neighbor, new_cost, cur_node);
    }
  }
  throw std::runtime_error(std::format("Node {} not reachable from {}",
                                       target->Name(), source->Name()));
}

template <NodeType Node, EdgeType Edge>
std::vector<std::shared_ptr<Node>>
AStarPath(const Graph<Node, Edge>& graph, const std::shared_ptr<Node>& source,
          const std::shared_ptr<Node>& target,
          const std::optional<Heuristic_t<Node>>& heuristic = std::nullopt) {
  return AStarPath(DiGraph<Node, Edge>(graph), source, target, heuristic);
}

} // namespace xgraph::algorithm
