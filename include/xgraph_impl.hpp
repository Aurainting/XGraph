#pragma once

#include "edge.hpp"
#include "node.hpp"
#include "type_concepts.hpp"

#include <functional>
#include <memory>
#include <queue>
#include <vector>

namespace xgraph::impl {
template <NodeType Node>
void _bfs(const std::shared_ptr<Node> &s,
          const std::function<void(const std::shared_ptr<Node> &)> &func) {
  auto q = std::queue<std::shared_ptr<Node>>();
}

} // namespace xgraph::impl
