#include <string>

#include <catch2/catch_test_macros.hpp>

#include "xgraph.hpp"


TEST_CASE("DiGraph Structure", "DiGraph") {
    using xgraph::MyEdge;
    using xgraph::MyNode;

    const int n = 10;

    auto graph = std::make_shared<xgraph::DiGraph<> >();
    REQUIRE(graph->IsDirected());

    // Add nodes
    const auto s_node = std::make_shared<MyNode>("source");
    const auto t_node = std::make_shared<MyNode>("target");

    graph->AddNode(s_node);
    graph->AddNode(t_node);

    for (int i = 0; i < n; ++i) {
        graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
    }

    REQUIRE(graph->NodeSize() == n + 2);

    // Add edges
    graph->AddEdge(s_node, t_node, 2);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            graph->AddEdge(std::to_string(i), std::to_string(j));
        }
    }

    REQUIRE(graph->EdgeSize() == n * (n - 1) + 1);

    // Node tests
    REQUIRE(graph->HasNode("source"));

    REQUIRE(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

    const auto s_node_test = graph->GetNode("source");
    REQUIRE(*s_node == *s_node_test);
    REQUIRE(xgraph::utils::NodeEqual(s_node, s_node_test));

    const auto some_node_test =
            graph->GetNode(std::to_string(static_cast<int>(n / 2)));
    REQUIRE(graph->Nodes().contains(some_node_test));

    graph->RemoveNode("source"); // also edges with it
    REQUIRE_FALSE(graph->HasNode("source"));

    graph->AddNode(s_node);
    REQUIRE(graph->HasNode("source"));

    // Edge tests
    REQUIRE_FALSE(graph->HasEdge("source", "target", 2));

    graph->AddEdge(s_node, t_node, 2);
    REQUIRE_FALSE(graph->HasEdge("source", "target"));
    REQUIRE(graph->HasEdge("source", "target", 2));
    REQUIRE_FALSE(graph->HasEdge("target", "source", 2));

    graph->RemoveEdge("source", "target", 2);
    REQUIRE_FALSE(graph->HasEdge("source", "target", 2));

    graph->AddEdge("source", "target", 2);
    REQUIRE(graph->HasEdge("source", "target", 2));

    REQUIRE(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

    const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
    const auto some_edge_test =
            graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

    REQUIRE(graph->Edges().contains(s_t_edge_test));
    REQUIRE(graph->Edges().contains(some_edge_test));
    REQUIRE(graph->Nodes().contains(some_edge_test->Source()));
    REQUIRE(graph->Nodes().contains(some_edge_test->Target()));

    REQUIRE(graph->EdgeSize("0") == 2 * (n - 1));
    REQUIRE(graph->InEdgeSize("0") == n - 1);
    REQUIRE(graph->OutEdgeSize("0") == n - 1);

    REQUIRE(graph->OutEdges("0").contains(some_edge_test));

    REQUIRE(graph->Children("source").contains(t_node));

    REQUIRE(graph->Predecessor("target").contains(s_node));
    REQUIRE(graph->Successor("0").contains(some_node_test));

    REQUIRE(graph->Neighbors("target").contains(s_node));
}

TEST_CASE("Graph Structure", "Graph") {
    using xgraph::MyEdge;
    using xgraph::MyNode;

    const int n = 10;

    auto graph = std::make_shared<xgraph::Graph<> >();
    REQUIRE_FALSE(graph->IsDirected());

    // Add nodes
    const auto s_node = std::make_shared<MyNode>("source");
    const auto t_node = std::make_shared<MyNode>("target");

    graph->AddNode(s_node);
    graph->AddNode(t_node);

    for (int i = 0; i < n; ++i) {
        graph->AddNode(std::make_shared<MyNode>(std::to_string(i)));
    }
    REQUIRE(graph->NodeSize() == n + 2);

    // Add edges
    graph->AddEdge(s_node, t_node, 2);
    graph->AddEdge(t_node, s_node, 2); // same as above
    graph->AddEdge(t_node, s_node, 1); // same nodes with different weight

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            graph->AddEdge(std::to_string(i), std::to_string(j));
        }
    }
    REQUIRE(graph->EdgeSize() == n * (n - 1) / 2 + 2);

    // Node tests
    REQUIRE(graph->HasNode("source"));
    REQUIRE(graph->HasNode(std::to_string(static_cast<int>(n / 2))));

    const auto s_node_test = graph->GetNode("source");
    REQUIRE(*s_node == *s_node_test);
    REQUIRE(xgraph::utils::NodeEqual(s_node, s_node_test));

    const auto some_node_test =
            graph->GetNode(std::to_string(static_cast<int>(n / 2)));
    REQUIRE(graph->Nodes().contains(some_node_test));

    graph->RemoveNode("source");
    REQUIRE_FALSE(graph->HasNode("source"));

    graph->AddNode(s_node);
    REQUIRE(graph->HasNode("source"));

    // Edge tests
    REQUIRE_FALSE(graph->HasEdge("target", "source", 2));
    REQUIRE_FALSE(graph->HasEdge("target", "source", 1));

    graph->AddEdge(s_node, t_node, 2);
    graph->AddEdge(t_node, s_node, 2); // same as above
    graph->AddEdge(t_node, s_node, 1); // same nodes with different weight

    REQUIRE(graph->HasEdge("source", "target", 1));
    REQUIRE(graph->HasEdge("target", "source", 2));

    REQUIRE(graph->HasEdge("0", std::to_string(static_cast<int>(n / 2))));

    const auto s_t_edge_test = graph->GetEdge("source", "target", 2);
    const auto some_edge_test =
            graph->GetEdge("0", std::to_string(static_cast<int>(n / 2)));

    REQUIRE(graph->Edges().contains(s_t_edge_test));
    REQUIRE(graph->Edges().contains(some_edge_test));
    REQUIRE(graph->Nodes().contains(some_edge_test->Source()));
    REQUIRE(graph->Nodes().contains(some_edge_test->Target()));

    REQUIRE(graph->EdgeSize("0") == n - 1);
    REQUIRE(graph->InEdgeSize("0") == n - 1);
    REQUIRE(graph->OutEdgeSize("0") == n - 1);

    REQUIRE(graph->OutEdges("0").contains(some_edge_test));

    REQUIRE(graph->Children("source").contains(t_node));
    REQUIRE(graph->Children("0").size() == n - 1);

    REQUIRE(graph->Parents("0").size() == n - 1);

    REQUIRE(graph->Predecessor("target").contains(s_node));
    REQUIRE(graph->Successor("0").contains(some_node_test));

    REQUIRE(graph->Neighbors("target").contains(s_node));
    REQUIRE(graph->Neighbors("0").size() == n - 1);
}
