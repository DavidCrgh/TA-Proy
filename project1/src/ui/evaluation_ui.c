#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "evaluation_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/dfa.h"
#include "../test/test.h"

GtkBuilder *builder;

GtkWidget *evaluation_window;
GtkWidget *evaluate_button;
GtkWidget *final_state_label;
GtkWidget *finish_button;
GtkWidget *reset_button;
GtkWidget *simulate_button;
GtkWidget *states_route_label;
GtkWidget *string_entry;

void init_widget_refs1() {
    evaluation_window = GTK_WIDGET(gtk_builder_get_object(builder, "evaluation_window"));
    evaluate_button = GTK_WIDGET(gtk_builder_get_object(builder, "evaluate_button"));
    final_state_label = GTK_WIDGET(gtk_builder_get_object(builder, "final_state_label"));
    finish_button = GTK_WIDGET(gtk_builder_get_object(builder, "finish_button"));
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    simulate_button = GTK_WIDGET(gtk_builder_get_object(builder, "simulate_button"));
    states_route_label = GTK_WIDGET(gtk_builder_get_object(builder, "states_route_label"));
    string_entry = GTK_WIDGET(gtk_builder_get_object(builder, "string_entry"));

    g_signal_connect(G_OBJECT(evaluation_window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
}

int init_gui1(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // builder = gtk_builder_new();
    builder = gtk_builder_new_from_file("src/ui/evaluation.ui");
    gtk_builder_connect_signals(builder, NULL);
    init_widget_refs1();
    gtk_widget_show(evaluation_window);

    gtk_main();

    return OK;
}

void evaluate_string() {
    char *input = (char *) malloc(gtk_entry_get_text_length (GTK_ENTRY(string_entry)));;
    strcpy(input, gtk_entry_get_text(GTK_ENTRY(string_entry)));

    gtk_label_set_text(GTK_LABEL(states_route_label), input);
}

void on_reset_button_clicked(GtkButton * b) {
    gtk_label_set_text(GTK_LABEL(final_state_label), "");
    gtk_label_set_text(GTK_LABEL(states_route_label), "");
    gtk_entry_set_text(GTK_ENTRY(string_entry), "");
}

void on_evaluate_button_clicked(GtkButton * b) {
    gtk_label_set_text(GTK_LABEL(final_state_label), symbols);
    evaluate_string();
}

void on_finish_button_clicked(GtkButton * b) {
    gtk_widget_destroy(GTK_WIDGET(evaluation_window));
}
