#ifndef __SAMPLE_H__
#define __SAMPLE_H__

#include "graph.h"
#include "shadow-finder.h"

u_long sample(const graph &G, const std::vector<shadow> &s, double gamma, double eps, double delta, u_long t);

#endif //__SAMPLE_H__