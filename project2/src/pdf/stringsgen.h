#ifndef STRINGSGEN_H
#define STRINGSGEN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logic/common.h"
#include "logic/graph.h"

void build_strings(FILE *out, machine_conf_t *conf, graph *g);

#endif // STRINGSGEN_H