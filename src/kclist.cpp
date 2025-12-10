//
// Created by filip on 06.12.25.
//

#include "kclist.h"
#include <ranges>
#include <chrono>

graph make_directed_graph(graph &g) {
    auto new_graph = graph();

    for (const auto& [u, adj] : g.adj) {
        new_graph.add_node(u);
        for (auto v : adj) {
            if (u < v)
                new_graph.adj[u].insert(v);
        }
    }

    return new_graph;
}

u_long listing(const int l, graph &g, const int k, const int clique) {
    u_long output = 0;
    if (l == 2) {
        // Just counting not saving
        // For saving go through each edge and add to new_clique vector the u and v
        for (const auto& adj: g.adj | std::views::values) {
            for (size_t i = 0; i < adj.size(); i++) {
                int new_clique = clique;
                new_clique += 2;
                if (new_clique == k)
                    output++;
            }
        }
    } else {
        // Just for counting, for saving add u (key) to new_clique
        for (const auto key: g.adj | std::views::keys) {
            int new_clique = clique;
            new_clique++;
            if (g.has_node(key) && !g.neighbours(key).empty()) {
                auto new_graph = g.induced_subgraph(g.neighbours(key));
                output += listing(l - 1, new_graph, k, new_clique);
            }
        }
    }

    return output;
}

u_long exact_k_cliques(graph &g, const int k) {
    graph directed_g = make_directed_graph(g);
    return listing(k, directed_g, k, 0);
}


void exact_k_cliques_tests(const graph &g)
{
    unsigned int k_sizes[N_KSC] = {5,7,10,15};
    std::vector<u_long> results;
    std::vector<double> elapsed_times;

    for(int cnt = 0; cnt < N_KSC; cnt++)
    {
        std::cout << "Starting run for k = " << k_sizes[cnt] << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
            
        u_long number = exact_k_cliques(const_cast<graph&>(g),k_sizes[cnt]);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end - start;

        std::cout << "Number of " << k_sizes[cnt] << "-cliques is " << number <<" in " << elapsed_time.count() << " seconds" << std::endl;
        results.push_back(number);
        elapsed_times.push_back(elapsed_time.count());
    }

    std::cout << std::endl << "===================== Test results ========================" << std::endl;

    for(int cnt = 0; cnt < N_KSC; cnt++)
    {
        std::cout << "Results for k = " << k_sizes[cnt] << ": " << results[cnt] << " , " << elapsed_times[cnt] << std::endl;
    }
}