
#ifndef __SHADOW_FINDER_H__
#define __SHADOW_FINDER_H__

#include <vector>
#include <set>
#include "graph.h"

typedef struct shadow_{
    std::vector<u_long> nodes;
    unsigned int shadow_k;
} shadow;

typedef struct shadow_tuple_{
    graph g;
    unsigned int k;
    double edge_density;
    bool is_smaller;
} shadow_tuple;

std::vector<shadow> find_shadows(const graph &G, unsigned int k);

#endif //__SHADOW_FINDER_H__