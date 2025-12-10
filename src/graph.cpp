//
// Created by filip on 07.12.25.
//

#include "graph.h"
#include <fstream>
#include <ranges>

std::unordered_set<u_long> &graph::neighbours(const u_long u) {
    static std::unordered_set<u_long> empty;

    if(adj.find(u) == adj.end())
    {
        //printf("Node %ld is not in graph\n", u);
        return empty;
    }
    
    return adj.find(u)->second;
}

void graph::add_node(const u_long u) {
    adj[u] = std::unordered_set<u_long>();
}

void graph::remove_node(const u_long u)
{
    for(auto v: adj[u])
    {
        remove_edge(v,u); //remove all back edges
    }
    adj.erase(u); //remove node from the graph
}

std::vector<u_long> graph::nodes()
{
    std::vector<u_long> nodes;
    nodes.reserve(adj.size());

    for(const auto& kv: adj)
    {
        nodes.push_back(kv.first);
    }
    return nodes;
}

bool graph::has_node(const u_long u) const {
    return adj.contains(u);
}

size_t graph::number_of_nodes() const {
    return adj.size();
}

void graph::add_edge(const u_long u, const u_long v) {
    add_edge_d(u, v);
    adj[v].insert(u);
}

void graph::add_edge_d(const u_long u, const u_long v) {
    if (!has_node(u))
        add_node(u);
    if (!has_node(v))
        add_node(v);

    //added check so that I won't add duplicit edge
    if(adj[u].find(v) == adj[u].end()) adj[u].insert(v);

}

void graph::print_graph()
{
    for(const auto& kv: adj)
    {
        printf("%ld:\n",kv.first);
        for(u_long node: kv.second)
        {
            printf("%ld ",node);
        }
        printf("\n");
    }
}

void graph::remove_edge(u_long u, u_long v)
{
    if(adj[u].find(v) == adj[u].end()) return;
    adj[u].erase(v);
}

bool graph::has_edge(const u_long u, const u_long v) const {
    return has_node(u) && adj.at(v).contains(u);
}

size_t graph::number_of_edges(const bool oriented) const {
    size_t edges = 0;
    for (const auto& neighbours : adj | std::views::values) {
        edges += neighbours.size();
    }
    if (oriented)
        return edges;
    return edges / 2;
}


graph graph::clone() const {
    return graph(adj);
}

graph graph::induced_subgraph(const std::unordered_set<ulong> &nodes) const {
    std::unordered_map<u_long, std::unordered_set<u_long>> new_adj;
    for (const auto u : nodes) {
        new_adj[u] = std::unordered_set<u_long>();
        for (auto v : adj.at(u)) {
            if (nodes.contains(v))
                new_adj[u].insert(v);
        }
    }

    return graph(new_adj);
}

graph graph::induced_subgraph(const std::vector<u_long> &nodes) const {
    std::unordered_set<ulong> nds_map;

    for(u_long nd: nodes)
    {
        nds_map.insert(nd);
    }
    return induced_subgraph(nds_map);
}


graph::graph() = default;

graph::graph(const std::unordered_map<u_long, std::unordered_set<u_long>> &adjacency) : adj(adjacency) {}

graph::graph(const std::string &filename, bool oriented) {
    std::ifstream file(filename);
    u_long u, v;
    while (file >> u >> v) {
        if (oriented)
            add_edge_d(u, v);
        else
            add_edge(u, v);
    }
}

graph::~graph() = default;