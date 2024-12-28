#pragma once

#include <algorithm>
#include <cmath>
#include <numeric>
#include <queue>
#include <random>
#include <vector>

#include "../structure/graph.hpp"

namespace xgraph::algorithm {
enum class SPMethod {
    Dijkstra,
    Bellman_Ford,
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
template<NodeType Node, EdgeType Edge>
void dijkstra(const DiGraph<Node, Edge> &graph,
              const std::shared_ptr<Node> &source,
              const std::shared_ptr<Node> &target,
              std::vector<std::weak_ptr<Edge> > &res) {
    std::unordered_map<std::size_t, bool> visited;
    std::unordered_map<std::size_t, std::shared_ptr<Edge> > previous;
    std::unordered_map<std::size_t, double> distance;

    for (const auto &v: graph.Nodes()) {
        previous[v->Id()] = nullptr;
        distance[v->Id()] = std::numeric_limits<double>::infinity();
    }

    distance[source->Id()] = 0;

    const auto node_comp = [&distance](const std::shared_ptr<Node> &lhs,
                                       const std::shared_ptr<Node> &rhs) {
        return distance[lhs->Id()] > distance[rhs->Id()];
    };
    auto current_node = std::priority_queue<std::shared_ptr<Node>,
        std::vector<std::shared_ptr<Node> >,
        decltype(node_comp)>(node_comp);

    current_node.emplace(source);

    while (!current_node.empty() && target && !visited[target->Id()]) {
        const auto n = current_node.top();
        current_node.pop();

        if (visited[n->Id()]) {
            continue;
        }
        visited[n->Id()] = true;

        for (const auto &i: graph.OutEdges(n)) {
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

template<NodeType Node, EdgeType Edge>
void bellman_ford(const DiGraph<Node, Edge> &graph,
                  const std::shared_ptr<Node> &source,
                  const std::shared_ptr<Node> &target,
                  std::vector<std::weak_ptr<Edge> > &res) {
    std::unordered_map<std::size_t, std::shared_ptr<Edge> > previous;
    std::unordered_map<std::size_t, double> distance;

    for (const auto &n: graph.Nodes()) {
        distance[n->Id()] = std::numeric_limits<double>::infinity();
    }
    distance[source->Id()] = 0;

    for (std::size_t i = 0; i < graph.NodeSize(); ++i) {
        bool flag{false};
        for (const auto &e: graph.Edges()) {
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
} // namespace impl

template<NodeType Node, EdgeType Edge>
std::vector<std::weak_ptr<Edge> > ShortestPath(
    const DiGraph<Node, Edge> &graph, const std::shared_ptr<Node> &source,
    const std::shared_ptr<Node> &target = nullptr,
    const SPMethod &method = SPMethod::Dijkstra) {
    std::vector<std::weak_ptr<Edge> > res{};

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
} // namespace xgraph::algorithm
