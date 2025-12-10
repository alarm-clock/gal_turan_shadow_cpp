#include "turan-shadow.h"
#include <vector>
#include <cmath>
#include <chrono>
#include <iostream>

u_long factorial(unsigned int k)
{
    if(k == 1) return k;
    return k * factorial(k-1); 
}

double calc_gamma(const std::vector<shadow> &s)
{
    double max = 0.0;

    for(shadow sh: s)
    {
        double val = (std::pow(sh.shadow_k, sh.shadow_k - 2) / static_cast<double>(factorial(sh.shadow_k))) * (std::pow(sh.nodes.size(), 2));
        if (max < val) max = val;
    }

    return 1.0 / max;
}

u_long turan_shadow(const graph &g, unsigned int k, double eps, double delta, u_long t)
{
    std::vector<shadow> shadows = find_shadows(g,k);
    double gamma = calc_gamma(shadows);
    u_long number = sample(g, shadows, gamma, eps, delta, t);
    return number;
}


void turan_shadow_tests(const graph &g, int n_runs, double eps, double delta, u_long t)
{
    unsigned int k_sizes[N_KS] = {5,7,10,15};
    std::vector<std::vector<u_long>> results;
    std::vector<std::vector<double>> elapsed_times;

    for(int cnt = 0; cnt < N_KS; cnt++)
    {
        std::vector<u_long> run_res;
        std::vector<double> run_times;

        for(int run = 0; run < n_runs; run++)
        {
            std::cout << "Starting run " << run + 1 << "/" << n_runs << " for k = " << k_sizes[cnt] << std::endl;

            auto start = std::chrono::high_resolution_clock::now();
            
            u_long number = turan_shadow(g,k_sizes[cnt],eps,delta,t);

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = end - start;

            std::cout << "Estimated number of " << k_sizes[cnt] << "-cliques is " << number <<" in " << elapsed_time.count() << " seconds" << std::endl;

            run_res.push_back(number);
            run_times.push_back(elapsed_time.count());
        }

        results.push_back(run_res);
        elapsed_times.push_back(run_times);
    }

    std::cout << std::endl << "===================== Test results ========================" << std::endl;

    for(int cnt = 0; cnt < N_KS; cnt++)
    {
        std::cout << "Results for k = " << k_sizes[cnt] << ":" << std::endl;

        for(u_long res: results[cnt])
        {
            std::cout << res << " , ";
        }
        std::cout << std::endl;
        for(double time: elapsed_times[cnt])
        {
            std::cout << time << " , ";
        }
        std::cout << std::endl;
    }
}

