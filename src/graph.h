//
// Created by filip on 07.12.25.
//

#ifndef GAL_GRAPH_H
#define GAL_GRAPH_H
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <vector>

#define NOT_ORIENTED false
#define ORIENTED true

class graph {
    public:
        std::unordered_map<u_long, std::unordered_set<u_long>> adj;

        std::unordered_set<u_long> &neighbours(u_long u);

        void add_node(u_long u);
        void remove_node(u_long u);
        bool has_node(u_long u) const;
        std::vector<u_long> nodes();
        size_t number_of_nodes() const;

        void add_edge(u_long u, u_long v);
        void add_edge_d(u_long u, u_long v); // DIRECTIONAL
        void remove_edge(u_long u, u_long v);
        bool has_edge(u_long u, u_long v) const;
        size_t number_of_edges(bool oriented) const;
        
        void print_graph();

        graph clone() const;

        graph induced_subgraph(const std::unordered_set<ulong> &nodes) const;
        graph induced_subgraph(const std::vector<u_long> &nodes) const;

        graph();
        explicit graph(const std::unordered_map<u_long, std::unordered_set<u_long>> &adjacency);
        explicit graph(const std::string &filename, bool oriented);
        ~graph();
};


#endif //GAL_GRAPH_H