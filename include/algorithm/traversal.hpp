#pragma once

#include <queue>
#include <stack>
#include <utility>

#include "../structure/graph.hpp"

namespace xgraph::algorithm {
template<NodeType Node, EdgeType Edge>
void BFS(const DiGraph<Node, Edge> &graph, const std::shared_ptr<Node> &start,
         const std::function<void(const std::shared_ptr<Node> &)> &func =
                 std::identity{}) {
    std::queue<std::shared_ptr<Node> > q;
    std::unordered_map<std::size_t, bool> visited;

    for (const auto &i: graph.Neighbors(start->Name())) {
        q.push(i);
        visited[i->Id()] = false;
    }

    while (!q.empty()) {
        const auto n = q.front();
        q.pop();

        if (visited[n->Id()]) {
            continue;
        }

        func(n);
        visited[n->Id()] = true;

        for (const auto &i: graph.Neighbors(n->Name())) {
            q.push(i);
            visited.try_emplace(i->Id(), false);
        }
    }
}

template<NodeType Node, EdgeType Edge>
void BFS(const Graph<Node, Edge> &graph, const std::shared_ptr<Node> &start,
         const std::function<void(const std::shared_ptr<Node> &)> &func =
                 std::identity{}) {
    BFS(DiGraph<Node, Edge>(graph), start, func);
}

template<NodeType Node, EdgeType Edge>
void DFS(const DiGraph<Node, Edge> &graph, const std::shared_ptr<Node> &start,
         const std::function<void(const std::shared_ptr<Node> &)> &func =
                 std::identity{}) {
    std::stack<std::shared_ptr<Node> > s;
    std::unordered_map<std::size_t, bool> visited;

    for (const auto &i: graph.Neighbors(start->Name())) {
        s.push(i);
        visited[i->Id()] = false;
    }

    while (!s.empty()) {
        const auto n = s.top();
        s.pop();

        if (visited[n->Id()]) {
            continue;
        }

        func(n);
        visited[n->Id()] = true;

        for (const auto &i: graph.Neighbors(n->Name())) {
            s.push(i);
            visited.try_emplace(i->Id(), false);
        }
    }
}

template<NodeType Node, EdgeType Edge>
void DFS(const Graph<Node, Edge> &graph, const std::shared_ptr<Node> &start,
         const std::function<void(const std::shared_ptr<Node> &)> &func =
                 std::identity{}) {
    DFS(DiGraph<Node, Edge>(graph), start, func);
}
} // namespace xgraph::algorithm
