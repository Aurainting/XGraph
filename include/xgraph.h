#pragma once

#include "edge.h"
#include "utils.h"
#include <unordered_set>
#include <functional>

namespace xgraph {
    template <NodeType Node = MyNode,
            EdgeType Edge = MyEdge<Node>>
    class Graph {
    public:
        Graph() : _nodes(1, utils::NodeHash<Node>, utils::NodeEqual<Node>)
                , _edges(1, utils::EdgeHash<Edge>, utils::EdgeEqual<Edge>)
        {

        }

        void AddNode(const std::shared_ptr<Node>& n) {
            _nodes.insert(n);
        }

        void AddEdge(const std::shared_ptr<Node>& s,
                     const std::shared_ptr<Node>& t) {
            _edges.insert(s, t);
        }

    private:
        std::unordered_set<
                std::shared_ptr<Node>,
                std::function<std::size_t(const std::shared_ptr<Node>&)>,
                std::function<bool(const std::shared_ptr<Node>&, const std::shared_ptr<Node>&)>
        > _nodes;
        std::unordered_set<
                std::shared_ptr<Edge>,
                std::function<std::size_t(const std::shared_ptr<Edge>&)>,
                std::function<bool(const std::shared_ptr<Edge>&, const std::shared_ptr<Edge>&)>
        > _edges;
    };

} // namespace xgraph
