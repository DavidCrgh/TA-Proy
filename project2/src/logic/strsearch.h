#ifndef STRSEARCH_H
#define STRSEARCH_H

#include <stdbool.h>

#include "graph.h"

#define STRLEN_MAX 30

void get_strings(graph *g, int *accept, bool complement, int num_str, char **out_strings, int *out_found);

#endif // STRSEARCH_H