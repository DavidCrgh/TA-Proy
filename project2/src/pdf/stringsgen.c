#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "stringsgen.h"
#include "logic/common.h"
#include "logic/strsearch.h"
#include "logic/matrix.h"
#include "logic/dfa.h"

#define NUM_STRINGS 5

void build_strings(FILE *out, machine_conf_t *conf, graph *g) {

	fputs("\n\\section{Hileras de ejemplo}\n", out);


	// Accepted strings

	fputs("\n\\subsection{Hileras aceptadas}\n", out);

	fputs("\n\\begin{itemize}\n", out);

	char **solutions = (char **) createMatrix(NUM_STRINGS, STRLEN_MAX, sizeof(char*));
	int found = 0;

	for (int i = 0; i < NUM_STRINGS; i++) {
		solutions[i][0] = '\0';
	}

    get_strings(g, conf, false, NUM_STRINGS, solutions, &found);

	if (found == 0) {

		fputs("\\item No se encontraron hileras.\n", out);

	} else {

		for (int i = 0; i < found; i++) {

			fprintf(out, "\\item ``%s''\n", solutions[i]);

		}

	}
	

	fputs("\n\\end{itemize}\n", out);



	// Rejected strings

	fputs("\n\\subsection{Hileras rechazadas}\n", out);

	fputs("\n\\begin{itemize}\n", out);


	for (int i = 0; i < NUM_STRINGS; i++) {
		solutions[i][0] = '\0';
	}
	found = 0;

	get_strings(g, conf, true, NUM_STRINGS, solutions, &found);

	if (found == 0) {

		fputs("\\item No se encontraron hileras.\n", out);

	} else {

		for (int i = 0; i < found; i++) {

			fprintf(out, "\\item ``%s''\n", solutions[i]);

		}

	}

	fputs("\n\\end{itemize}\n\n", out);


	freeMatrix((void**) solutions, NUM_STRINGS);
}