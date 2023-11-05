#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logic/controller.h"
#include "logic/dfa.h"
#include"logic/common.h"
#include "logic/graph.h"
#include "pdfbuilder.h"
#include "components.h"
#include "graphgen.h"
#include "stringsgen.h"
#include "regexgen.h"


#define BASE_PATH "./src/pdf/latex/base.tex"
#define OUT_PATH "./out/out.tex"
#define OUT_REGEX_PATH "./out/out_regex.tex"

void copy_from_static(char *src, FILE *dst) {
    char c;
    FILE *file = fopen(src,"r");
    if (file == NULL) {
        printf("Cannot open file %s \n", src);
    }

    c = fgetc(file);
    while (c != EOF) {
        fputc(c, dst);
        c = fgetc(file);
    }

    fclose(file);
}

void build_pdf() {
    FILE *out; // Handle for generated out.tex file
    FILE *out_regex; // Handle for generated out_regex.tex file
    
    // Create pdflatex output directory if not exists and out file
    struct stat st = {0};

    if (stat("out", &st) == -1) {
        mkdir("out", 0700);
    }

    out = fopen(OUT_PATH, "w");
    if (out == NULL) {
        printf("Cannot open file %s \n", OUT_PATH);
    }

    out_regex = fopen(OUT_REGEX_PATH, "w");
    if(out == NULL)
    {
        printf("Cannot open file %s \n", OUT_REGEX_PATH);
    }

    // Copy base.tex contents into output file
    copy_from_static(BASE_PATH, out);

    machine_conf_t *conf = get_conf();
    graph graph = create_graph(conf->table, conf->accept, conf->labels, conf->symbols);

    // Call function to build section 2 (DFA formal definition)
    build_components(out, conf);
    
    // Call function to build section 3 (DFA Graph)
    build_graph(out, conf, &graph);

    // Call function to build sections 4 and 5 (strings)
    //build_strings(out, conf, &graph);

    // Call function to build section 6 (Regex)
    build_regex(out, out_regex, conf);
    copy_from_static(OUT_REGEX_PATH, out);

    // Close the document and file handler
    fputs("\n \\end{document}\\\\", out);
    fclose(out);
    
    // Invoke dot2tex, pdflatex and evince to create and show the PDF
    system("cd out && pdflatex --shell-escape out.tex");
    // NOTE: it might be necessary to invoke pdflatex twice
    system("evince out/out.pdf &");
}
