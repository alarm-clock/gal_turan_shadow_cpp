//
// Created by filip on 06.12.25.
//

#ifndef GALPROJEKT_KCLIST_H
#define GALPROJEKT_KCLIST_H
#include "graph.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#define N_KSC 4

u_long exact_k_cliques(graph &g, int k);
void exact_k_cliques_tests(const graph &g);

#endif //GALPROJEKT_KCLIST_H