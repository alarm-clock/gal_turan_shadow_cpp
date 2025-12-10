#include "shadow-finder.h"

double calc_edge_density(const graph &g)
{
    size_t n = g.number_of_nodes();
    size_t m = g.number_of_edges(false);
    return (n > 1) ? static_cast<double>(2*m) / (static_cast<double>(n * (n - 1))) : 0.0;
}

shadow_tuple create_tuple(const graph &g, unsigned int k)
{
    double edge_density = calc_edge_density(g);
    bool is_smaller = edge_density <= 1.0 - (1.0 / static_cast<double>(k-1));

    shadow_tuple tuple = {.g = g, .k = k, .edge_density = edge_density, .is_smaller = is_smaller};
    return tuple;
}

u_long pop_set_element(std::unordered_set<u_long> &set)
{
    auto it = set.begin();
    u_long val = *it;
    set.erase(it);
    return val;
}

std::vector<u_long> degeneracy_ordering(const graph &G)
{
    graph H = G.clone();
    std::unordered_map<u_long, u_long> degrees;
    size_t max_d = 0;

    for(const auto& kv : G.adj)
    { 
        u_long nd = kv.first;
        size_t degree = const_cast<graph&>(G).neighbours(nd).size(); //kv.second.size();
        degrees[nd] = degree;
        if(degree > max_d) max_d = degree;
    }

    std::vector<std::unordered_set<u_long>> priority_buckets(max_d + 1);

    for(const auto& kv: G.adj)
    {
        u_long nd = kv.first;
        priority_buckets[degrees[nd]].insert(nd);
    }
    std::vector<u_long> deg_ordering;
    u_long nd_degree = 0;

    for(size_t i = 0; i < G.number_of_nodes(); i++)
    {
        while( nd_degree <= max_d && priority_buckets[nd_degree].empty()) nd_degree++;

        u_long node = pop_set_element(priority_buckets[nd_degree]);
        deg_ordering.push_back(node);

        for(auto u: H.neighbours(node))
        {
            priority_buckets[degrees[u]].erase(u);
            degrees[u]--;
            priority_buckets[degrees[u]].insert(u);

            if(degrees[u] < nd_degree)
            {
                nd_degree = degrees[u];
            }
        }
        H.remove_node(node);
    }
    return deg_ordering;
}

graph create_degeneracy_dag(const std::vector<u_long> &ordering, const graph &G)
{
    std::unordered_map<u_long, u_long> pos_arr;

    for(size_t position = 0; position < ordering.size(); position++)
    {
        pos_arr[ordering[position]] = position;
    }
    graph dag;

    for( const auto& kv: G.adj)
    {
        u_long u = kv.first;

        for(u_long v: kv.second)
        {
            if( pos_arr[u] < pos_arr[v]) dag.add_edge_d(u,v);
            else dag.add_edge_d(v,u);
        }
    }
    return dag;
}


std::vector<shadow> find_shadows(const graph &G, unsigned int k)
{
    std::vector<shadow> s;
    std::vector<shadow_tuple> t;
    t.push_back(create_tuple(G,k));

    while(!t.empty()){

        shadow_tuple working_tuple = t.back();
        t.pop_back();
        //printf("%ld\n",t.size());

        if(!working_tuple.is_smaller) continue;

        std::vector<u_long> ordering = degeneracy_ordering(working_tuple.g);
        graph dag = create_degeneracy_dag(ordering,working_tuple.g);

        for(const auto& kv: working_tuple.g.adj)
        {
  
            u_long node = kv.first;

            auto neighbors = dag.neighbours(node);
            if(neighbors.empty()) continue;

            graph out_neighborhood = G.induced_subgraph(neighbors);
            if(out_neighborhood.number_of_nodes() <  1) continue;
            
            if( working_tuple.k <= 2 || calc_edge_density(out_neighborhood) > 1.0 - (1.0 / static_cast<double>(working_tuple.k - 2)))
            {
                shadow sh = {.nodes = out_neighborhood.nodes(), .shadow_k = working_tuple.k - 1 };
                s.push_back(sh);
            } else {
                shadow_tuple tup = create_tuple(out_neighborhood,working_tuple.k - 1);
                t.push_back(tup);
            }
        }
    }
    return s;
}