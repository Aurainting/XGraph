#pragma once

#include <optional>
#include <queue>
#include <stack>

#include "structure/graph.hpp"
#include "structure/type_traits.hpp"

namespace xgraph::algorithm {

template <NodeType Node, EdgeType Edge>
void BFS(const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& start,
         const std::optional<NodePtrVisitor_t<Node>>& func = std::nullopt) {
  std::queue<std::shared_ptr<Node>> q;
  std::unordered_map<std::size_t, bool> visited;

  if (func.has_value()) {
    func.value()(start);
  }
  visited[start->Id()] = true;

  for (const auto& i : graph.Neighbors(start->Name())) {
    q.push(i);
    visited.try_emplace(i->Id(), false);
  }

  while (!q.empty()) {
    const auto n = q.front();
    q.pop();

    if (visited[n->Id()]) {
      continue;
    }

    if (func.has_value()) {
      func.value()(n);
    }

    visited[n->Id()] = true;

    for (const auto& i : graph.Neighbors(n->Name())) {
      q.push(i);
      visited.try_emplace(i->Id(), false);
    }
  }
}

template <NodeType Node, EdgeType Edge>
void BFS(const Graph<Node, Edge>& graph, const std::shared_ptr<Node>& start,
         const std::optional<NodePtrVisitor_t<Node>>& func = std::nullopt) {
  BFS(DiGraph<Node, Edge>(graph), start, func);
}

template <NodeType Node, EdgeType Edge>
void DFS(const DiGraph<Node, Edge>& graph, const std::shared_ptr<Node>& start,
         const std::optional<NodePtrVisitor_t<Node>>& func = std::nullopt) {
  std::stack<std::shared_ptr<Node>> s;
  std::unordered_map<std::size_t, bool> visited;

  if (func.has_value()) {
    func.value()(start);
  }
  visited[start->Id()] = true;

  for (const auto& i : graph.Neighbors(start->Name())) {
    s.push(i);
    visited.try_emplace(i->Id(), false);
  }

  while (!s.empty()) {
    const auto n = s.top();
    s.pop();

    if (visited[n->Id()]) {
      continue;
    }

    if (func.has_value()) {
      func.value()(n);
    }

    visited[n->Id()] = true;

    for (const auto& i : graph.Neighbors(n->Name())) {
      s.push(i);
      visited.try_emplace(i->Id(), false);
    }
  }
}

template <NodeType Node, EdgeType Edge>
void DFS(const Graph<Node, Edge>& graph, const std::shared_ptr<Node>& start,
         const std::optional<NodePtrVisitor_t<Node>>& func = std::nullopt) {
  DFS(DiGraph<Node, Edge>(graph), start, func);
}

template <NodeType Node, EdgeType Edge>
void TopologicalSort(
    const DiGraph<Node, Edge>& graph,
    const std::optional<NodePtrVisitor_t<Node>>& func = std::nullopt) {
  std::unordered_map<std::size_t, std::size_t> indegree_map{};
  std::vector<std::size_t> zero_indegree{};
  for (const auto& n : graph.Nodes()) {
    if (const auto indegree_num = graph.InEdgeSize(n->Id());
        indegree_num == 0) {
      zero_indegree.push_back(n->Id());
    } else {
      indegree_map[n->Id()] = indegree_num;
    }
  }

  while (!zero_indegree.empty()) {
    auto this_generation = std::move(zero_indegree);
    zero_indegree.clear();
    for (const auto& node : this_generation) {
      if (!graph.HasNode(node)) {
        throw std::runtime_error("Graph changed during iteration!");
      }

      if (func.has_value()) {
        func.value()(graph.GetNode(node));
      }

      for (const auto& child : graph.Children(node)) {
        --indegree_map[child->Id()];
        if (indegree_map[child->Id()] == 0) {
          zero_indegree.push_back(child->Id());
          indegree_map.erase(child->Id());
        }
      }
    }
  }
  if (!indegree_map.empty()) {
    throw std::runtime_error(
        "Graph contains a cycle or graph changed during iteration!");
  }
}

} // namespace xgraph::algorithm
