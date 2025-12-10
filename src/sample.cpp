#include "sample.h"
#include <cmath>
#include <random>
#include <algorithm>

u_long combination(long n, unsigned int k)
{
    if(k > n) return 0;
    if(k > n - k) k = n - k; //combination numbers are symmetric
    unsigned long long result = 1;
    for(unsigned int cnt = 1; cnt <= k; cnt++)
    {
        result *= n - cnt + 1;
        result /= cnt;
    }

    return result;
}

bool is_k_clique(const graph &G, const std::vector<u_long> sample)
{
    graph clq = G.induced_subgraph(sample);
    u_long n = clq.number_of_nodes();
    u_long m = clq.number_of_edges(false);

    return m == static_cast<u_long>((n * (n-1)) / 2);
}

u_long sample(const graph &G, const std::vector<shadow> &s, double gamma, double eps, double delta, u_long t)
{
    if( t == 0 )
    {
        double t_doub = ( 20.0 / (gamma * (eps * eps))) * log(1.0/delta);
        t = static_cast<u_long>(std::round(t_doub));
    }

    u_long* weights = new u_long[s.size()];
    std::vector<double> distr;
    distr.resize(s.size());
    u_long weight_sum = 0;

    for(u_long cnt = 0; cnt < s.size(); cnt++)
    {
        shadow sh = s[cnt];
        u_long w = combination(sh.nodes.size(),sh.shadow_k);
        weights[cnt] = w;
        weight_sum += w;
    }

    if( weight_sum == 0)
    {
        return 0.0;
    }

    for(u_long cnt = 0; cnt < distr.size(); cnt++)
    {
        distr[cnt] = static_cast<double>(weights[cnt]) / static_cast<double>(weight_sum);
    }

    u_long estimate = 0;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(distr.begin(),distr.end());

    for(u_long cnt = 0; cnt < t; cnt++)
    {
        int idx = dist(gen);
        shadow sh = s[idx];

        if(sh.nodes.size() < sh.shadow_k) continue;

        std::vector<u_long> tmp = sh.nodes;
        std::shuffle(tmp.begin(), tmp.end(), gen);
        std::vector<u_long> node_sample(tmp.begin(),tmp.begin() + sh.shadow_k);
        
        if (is_k_clique(G,node_sample)) estimate++;
    }
    delete[] weights;

    return (static_cast<double>(estimate) / static_cast<double>(t)) * weight_sum;
}