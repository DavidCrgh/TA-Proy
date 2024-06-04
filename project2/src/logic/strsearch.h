#ifndef STRSEARCH_H
#define STRSEARCH_H

#include <stdbool.h>

#include "graph.h"
#include "common.h"

#define STRLEN_MAX 30

void get_strings(graph *g, machine_conf_t *conf, bool complement, int num_str, char **out_strings, int *out_found);

#endif // STRSEARCH_H