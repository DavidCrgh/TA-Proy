#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "components.h"
#include "logic/common.h"


void build_transition_table(FILE *out, machine_conf_t *conf) {

    fputs("\\begin{mdframed}\n", out);
    fputs("\\textbf{Función de transición ($\\delta$):}\\newline\n", out);


    fprintf(out, "\\begin{longtable}[l]{c|*{%d}{c}}\n", num_symbols);

    // Build header row
    fputs("$\\times$ & ", out);

    for (int i = 0; i < num_symbols; i++) {
        fputc(conf->symbols[i], out);

        if (i != num_symbols - 1) fputs(" & ", out);
    }

    fputs("\\\\\n\\hline \n", out);


    // Now build the rows for each state

    int index;

    for (int i = 0; i < num_states; i++) {

        fprintf(out, "%s & ", conf->labels[i]);


        for (int j = 0; j < num_symbols; j++) {

            index = conf->table[i][j];

            if (index == -1) {
                fputs("-", out);
            } else {
                fprintf(out, "%s", conf->labels[index]);
            }

            if (j != num_symbols - 1) fputs(" & ", out);

        }

        if (i != num_states - 1) fputs("\\\\\n", out);

    }


    fputs("\\end{longtable}\n", out);
    fputs("\\end{mdframed}\n", out);
}


void build_components(FILE *out, machine_conf_t *conf) {

    // States (Q)

    fputs("\\begin{mdframed}\n", out);
    
    fputs("\\textbf{Estados:}\\newline\n$Q = $\\{", out);

    for (int i = 0; i < num_states; i++) {
        fprintf(out, "%s", conf->labels[i]);

        if (i != num_states - 1) fputs(", ", out);
    }

    fputs("\\}\\newline\n\n", out);

    fputs("\\end{mdframed}\n", out);


	// Alphabet (Sigma)

    fputs("\\begin{mdframed}\n", out);

    fputs("\\textbf{Alfabeto:}\\newline\n$\\Sigma = $\\{", out);

    for (int i = 0; i < num_symbols; i++) {
        fputc(conf->symbols[i], out);
        if (i != num_symbols - 1) fputs(", ", out);
    }

    fputs("\\}\\newline\n\n", out);

    fputs("\\end{mdframed}\n", out);
	
	
	// Transition function (delta)
    build_transition_table(out, conf);
    
    
    // Initial state (q_0)

    fputs("\\begin{mdframed}\n", out);

    fputs("\\textbf{Estado inicial:}\\newline\n", out);
    fprintf(out, "$q_0 = $%s\\newline\n\n", conf->labels[0]);

    fputs("\\end{mdframed}\n", out);
    

    // Acceptance states (F)

    fputs("\\begin{mdframed}\n", out);

    fputs("\\textbf{Estados de aceptación:}\\newline\n$F = $\\{", out);
	
    int printcomma = 0;
    for (int i = 0; i < num_states; i++) {

        if (conf->accept[i] == 1) {
            if (printcomma) fputs(", ", out);
            fprintf(out, "%s", conf->labels[i]);
            printcomma = 1;
        }
    }

    fputs("\\}\\newline\n\n", out);

    fputs("\\end{mdframed}\n", out);
}
