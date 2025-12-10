
#ifndef __TURAN_H__
#define __TURAN_H__

#include "graph.h"
#include "shadow-finder.h"
#include "sample.h"

#define KCLIST true
#define TURANS false
#define DEF_K 5
#define DEF_EPS 0.01
#define DEF_DEL 0.1
#define DEF_T 0
#define DEF_FILE ""
#define ARGSTART 1

#define N_KS 4

typedef struct _params_{
    std::string file_name;
    bool algorithm;
    int k;
    double eps;
    double delta;
    u_long t;
    bool t_tests;
    bool correct;
    bool help;
} prog_params;

u_long turan_shadow(const graph &g, unsigned int k, double eps, double delta, u_long t);
void turan_shadow_tests(const graph &g, int n_runs, double eps, double delta, u_long t);

#endif //__TURAN_H__