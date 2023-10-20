#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "pdfbuilder.h"

#define BASE_PATH "./src/pdf/latex/base.tex"
#define OUT_PATH "./out/out.tex"

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

    struct stat st = {0};

    if (stat("out", &st) == -1) {
        mkdir("out", 0700);
    }


    // Create out file or truncate existing one
    out = fopen(OUT_PATH, "w");
    if (out == NULL) {
        printf("Cannot open file %s \n", OUT_PATH);
    }

    // Copy base.tex contents into output file
    copy_from_static(BASE_PATH, out);

    // TODO: call function(s) to build each section of the file (parts 2 - 5)

    // Close the document and file handler
    fputs("\n \\end{document}\\\\", out);
    fclose(out);


    // Invoke pdflatex and evince to create and show the PDF
    system("pdflatex -output-directory out ./out/out.tex");
    system("evince out/out.pdf &");
}