
#include <stdio.h>
#include "turan-shadow.h"
#include "kclist.h"
#include "graph.h"
#include <iostream>
#include <chrono>

prog_params parse_arguments(int argc, char* argv[])
{
    prog_params params = {DEF_FILE,KCLIST,DEF_K,DEF_EPS,DEF_DEL,DEF_T,false,true,false};

    bool k_st = false;
    bool alg_st = false;
    bool file_st = false;
    bool eps_st = false;
    bool del_st = false;
    bool t_st = false;
    bool test_st = false;

    for(int cnt = ARGSTART; cnt < argc; cnt++)
    {
        if(argv[cnt][0] == '-')
        {
            switch (argv[cnt][1])
            {
            case 'h':
                std::cerr << "Program that implements algorithms for counting k cliques. Precise k-clist and aproximation algorithm Turan-Shadow." << std::endl;
                std::cout << "Usage:" << std::endl;
                std::cout << "\tGALprojekt -f <filename> -k <number k> {-c|-s} [-t <number t>] [-e <epsilon>] [-d <delta>] [-r]" << std::endl << std::endl;
                std::cout << "\t-f <filename>\tPath to file that contains graph" << std::endl;
                std::cout << "\t-k <number k>\tSize of cliques that will be counted." << std::endl;
                std::cout << "\t-c\t\tUse k-clist algorithm" << std::endl;
                std::cout << "\t-s\t\tUse Turan-Shadow algorithm" << std::endl;
                std::cout << "\t-t\t\tNumber of random samples used during the sampling in Turan-Shadow. Optional, if not specified number of samples will be calculated from values epsilon and delta" << std::endl;
                std::cout << "\t-e <epsilon>\tMaximal error, used in Turan-Shadow. Optional, defaults to 0.01" << std::endl;
                std::cout << "\t-d <delta>\tDensity treshold, used in Turan-Shadow. Optional, defaults to 0.1" << std::endl;
                std::cout << "\t-r\t\tRuns specified algorithm for k = [5,7,10,15]. In Turan-Shadow parameter k specifies number of runs for each k, for k-clist it is not used but you still need to specify it :(" << std::endl;
                params.help = true;
                return params;
                break;
            
            case 'f':
                if(file_st)
                {
                    params.correct = false;
                    std::cerr << "Only one file can be specified" << std::endl;
                    return params;
                }
                params.file_name = argv[++cnt];
                file_st = true;
                break;
            
            case 'k':
                if(k_st)
                {
                    params.correct = false;
                    std::cerr << "k was already specified" << std::endl;
                    return params;
                }
                params.k = std::stoi(argv[++cnt]);
                k_st = true;
                break;

            case 'c':
                if(alg_st)
                {
                    params.correct = false;
                    std::cerr << "Algorithm was already specified" << std::endl;
                    return params;
                }
                params.algorithm = KCLIST;
                alg_st = true;
                break;
            
            case 's':
                if(alg_st)
                {
                    params.correct = false;
                    std::cerr << "Algorithm was already specified" << std::endl;
                    return params;
                }
                params.algorithm = TURANS;
                alg_st = true;
                break;

            case 't':
                if(t_st)
                {
                    params.correct = false;
                    std::cerr << "t was already specified" << std::endl;
                    return params;
                }
                params.t = std::stoul(argv[++cnt]);
                t_st = true;
                break;

            case 'e':
                if(eps_st)
                {
                    params.correct = false;
                    std::cerr << "Epsilon was already specified" << std::endl;
                    return params;
                }
                params.eps = std::stod(argv[++cnt]);
                eps_st = true;
                break;

            case 'd':
                if(del_st)
                {
                    params.correct = false;
                    std::cerr << "Delta was already specified" << std::endl;
                    return params;
                }
                params.delta = std::stod(argv[++cnt]);
                del_st = true;
                break;

            case 'r':
                if(test_st)
                {
                    params.correct = false;
                    std::cerr << "Test was already specified" << std::endl;
                    return params;
                }
                params.t_tests = true;
                test_st = true;
                break;

            default:
                std::cerr << "Unknown option. Use \"-h\" for help." << std::endl;
                params.correct = false;
                return params;
                break;
            }

        } else {
            fprintf(stderr, "Unknown arguments. Use \"-h\" for help");
            params.correct = false;
            return params;
        }
    }
    if(!k_st || !file_st || !alg_st )
    {
        params.correct = false;
        std::cerr << "Algorithm, path to file, and k must be specified! Use \"-h\" for help" << std::endl;
    }

    return params;
}

void one_time_run(const graph &g, prog_params params)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    if(params.algorithm == KCLIST)
    {
        u_long number = exact_k_cliques(const_cast<graph&>(g), params.k);
        std::cout << "Number of " << params.k << "-cliques: " << number << std::endl;
    } else {
        u_long number = turan_shadow(g,static_cast<unsigned int>(params.k),params.eps,params.delta,params.t);
        std::cout << "Estimated number of " << params.k << "-cliques: " << number << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_time = end - start;

    std::cout << "Elapsed time: " << elapsed_time.count() << " seconds\n";
}

void tests(const graph &g, prog_params params)
{
    if(params.algorithm == TURANS)
    {
        turan_shadow_tests(g,params.k,params.eps,params.delta,params.t);
    } else {
        exact_k_cliques_tests(g);
    }
}

int main(int argc, char* argv[])
{
    prog_params params = parse_arguments(argc, argv);

    if(params.help) return 0;
    if(!params.correct) return 1;

    printf("Loading graph...\n");
    graph g(params.file_name,NOT_ORIENTED);
    std::cout << "Graph loaded!" << std::endl;
    std::cout << "Vertex count: " << g.number_of_nodes() << std::endl;
    std::cout << "Edge count: " << g.number_of_edges(NOT_ORIENTED) << std::endl << std::endl;

    if(params.t_tests)
    {
        tests(g,params);
    } else {
        one_time_run(g,params);
    }
    return 0;
}