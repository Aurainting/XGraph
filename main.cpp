#include "include/node.hpp"
#include "include/xgraph.hpp"
#include <iostream>

int main() {
    xgraph::Graph g;

    // Add Nodes
    xgraph::MyNode n_0 {"0"};
    xgraph::MyNode n_1 {"1"};
    xgraph::MyNode n_2 {"2"};
    xgraph::MyNode n_3 {"3"};
    xgraph::MyNode n_4 {"4"};

    g.AddNode(std::make_shared<xgraph::MyNode>(n_0));
    g.AddNode(std::make_shared<xgraph::MyNode>(n_1));
    g.AddNode(n_2);
    g.AddNode(n_3);
    g.AddNode(n_4);

    // Add Edges
    g.AddEdge(std::make_shared<xgraph::MyNode>(n_0), std::make_shared<xgraph::MyNode>(n_1));
    g.AddEdge(n_0, n_2);
    g.AddEdge(n_0, n_3);
    g.AddEdge(n_0, n_4);

    std::cout << "hello, world!" << std::endl;
}
