#ifndef GRAPHGEN_H
#define GRAPHGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logic/common.h"
#include "logic/graph.h"

void build_graph(FILE *out, machine_conf_t *conf, graph *g);

#endif // COMPONENTS_H
