#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stringsgen.h"
#include "logic/common.h"
#include "logic/strsearch.h"


void build_strings(FILE *out, machine_conf_t *conf, graph *g) {

    char **strings = get_strings(g, conf, false);

	printf("Accepted strings:\n\n");
	for (int i = 0; i < 5; i++) {
		printf("%d.\t\"%s\"\n", i+1, strings[i]);
	}

    strings = get_strings(g, conf, true);
    
    printf("Rejected strings:\n\n");
	for (int i = 0; i < 5; i++) {
		printf("%d.\t\"%s\"\n", i+1, strings[i]);
	}
}