#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "evaluation_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/list.h"
#include "../logic/controller.h"

#define HEADER_ROWS 4
#define LEFT_COLS 0
#define RIGHT_COLS 1

#define MAX_TAGS_LEN  20
#define MAX_CB_STRING_LEN 10
#define HEADER_BOTTOM_MARGIN 15
#define ENTRY_CENTER_ALIGNMENT 0.5

static GtkBuilder *builder;

static GtkWidget *back_button;
static GtkWidget *evaluation_window;
static GtkWidget *evaluate_button;
static GtkWidget *grid;
static GtkWidget *prev_window;
static GtkWidget *quit_button;
static GtkWidget *reset_button;
static GtkWidget *string_entry;

static void build_transition_grid(GtkWidget *widget, gpointer data) {
    gchar *text;

	GList *elements = gtk_container_get_children(GTK_CONTAINER(grid));

    GList *iterator;
    for (iterator = elements; iterator != NULL; iterator = g_list_next(iterator)) {
        gtk_widget_destroy(GTK_WIDGET(iterator->data));
    }
    g_list_free(elements);

    GtkLabel *acceptance_label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(acceptance_label), "<b>Acceptance final state</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(acceptance_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(acceptance_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(acceptance_label), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(acceptance_label), 2, 1, 1, 1);

    GtkLabel *acceptance_result_label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(acceptance_result_label), "<b>Accepted</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(acceptance_result_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(acceptance_result_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(acceptance_result_label), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(acceptance_result_label), 2, 2, 1, 1);

    GtkLabel *route_label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(route_label), "<b>Route</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(route_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(route_label), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(route_label), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(route_label), 2, 3, 1, 1);

    //Header's labels
    GtkLabel *header_labels[3];

    header_labels[0] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[0]), "<b>Current State</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(header_labels[0]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(header_labels[0]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(header_labels[0]), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(header_labels[0]), LEFT_COLS, HEADER_ROWS, 1, 1);

    header_labels[1] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[1]), "<b>Symbol</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(header_labels[1]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(header_labels[1]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(header_labels[1]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_bottom(GTK_WIDGET(header_labels[1]), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(header_labels[1]), 1 + LEFT_COLS, HEADER_ROWS, 1, 1);

    header_labels[2] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[2]), "<b>Next state</b>");
    gtk_widget_set_margin_bottom(GTK_WIDGET(header_labels[2]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_end(GTK_WIDGET(header_labels[2]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_top(GTK_WIDGET(header_labels[2]), HEADER_BOTTOM_MARGIN);
    gtk_widget_set_margin_bottom(GTK_WIDGET(header_labels[2]), HEADER_BOTTOM_MARGIN);
    gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(header_labels[2]), 2 + LEFT_COLS, HEADER_ROWS, 1, 1);

    GtkLabel *current_state_labels[10];
    GtkLabel *symbols_labels[10];
    GtkLabel *next_state_labels[10];

    for (int i = 0; i < 10; i++) {
        text = g_strdup_printf("%d", i);
        current_state_labels[i] = GTK_LABEL(gtk_label_new(text));
        gtk_widget_set_margin_bottom(GTK_WIDGET(current_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_end(GTK_WIDGET(current_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_top(GTK_WIDGET(current_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(current_state_labels[i]), LEFT_COLS, HEADER_ROWS + i + 1, 1, 1);

        symbols_labels[i] = GTK_LABEL(gtk_label_new(text));
        gtk_widget_set_margin_bottom(GTK_WIDGET(symbols_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_end(GTK_WIDGET(symbols_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_top(GTK_WIDGET(symbols_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(symbols_labels[i]), LEFT_COLS + 1, HEADER_ROWS + i + 1, 1, 1);


        next_state_labels[i] = GTK_LABEL(gtk_label_new(text));
        gtk_widget_set_margin_bottom(GTK_WIDGET(next_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_end(GTK_WIDGET(next_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_widget_set_margin_top(GTK_WIDGET(next_state_labels[i]), HEADER_BOTTOM_MARGIN);
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(next_state_labels[i]), LEFT_COLS + 2, HEADER_ROWS + i + 1, 1, 1);
    }
    
    gtk_widget_show_all(evaluation_window);
}

void init_widgets() {
    back_button = GTK_WIDGET(gtk_builder_get_object(builder, "back_button"));
    evaluation_window = GTK_WIDGET(gtk_builder_get_object(builder, "evaluation_window"));
    evaluate_button = GTK_WIDGET(gtk_builder_get_object(builder, "evaluate_button"));
    grid = GTK_WIDGET(gtk_builder_get_object(builder, "results_grid"));
    quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "finish_button"));
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "reset_button"));
    string_entry = GTK_WIDGET(gtk_builder_get_object(builder, "string_entry"));

    g_signal_connect(evaluate_button, "clicked", G_CALLBACK (build_transition_grid), NULL);
}

int init_gui_eval(GtkWidget *previous_window) {
	prev_window = previous_window;
    builder = gtk_builder_new_from_file("src/ui/evaluation.ui");
    gtk_builder_connect_signals(builder, NULL);

    init_widgets();
    gtk_widget_show(evaluation_window);

    gtk_main();

    return OK;
}

/*
void display_results(){
    char *input = (char *) malloc(gtk_entry_get_text_length (GTK_ENTRY(string_entry)));;
    strcpy(input, gtk_entry_get_text(GTK_ENTRY(string_entry)));
    int len_input = strlen(input);

    int *sequence = (int*)createList(len_input + 1, sizeof(int));
    fillList(sequence, len_input + 1);

    int result = execute_machine(input, sequence);
    
    g_print("Result: %d\n\n", result);

    if(result == 1) {
        gtk_label_set_text(GTK_LABEL(final_state_label), "Accepted");
    } else {
        gtk_label_set_text(GTK_LABEL(final_state_label), "Rejected");
    }

    char states_route[(strlen(input) + 1)*20];
    int index = 0;

    char **state_labels = get_state_labels();

    for (int i=0; i < strlen(input) + 1; i++) {
        g_print("\tElement[%d]: %s\n", i, state_labels[sequence[i]]);
        index += sprintf(&states_route[index], "%s\n", state_labels[sequence[i]]);
    }
        
    gtk_label_set_text(GTK_LABEL(states_route_label), states_route);
    //build_transition_grid();
}
*/

void on_evaluation_window_delete_event(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(GTK_WIDGET(prev_window));
	gtk_widget_destroy(GTK_WIDGET(evaluation_window));
	exit(EXIT_SUCCESS);
}

void on_evaluate_button_clicked(GtkButton * b) {
    //display_results();
}

void on_reset_button_clicked(GtkButton * b) {
    //gtk_label_set_text(GTK_LABEL(final_state_label), "");
    //gtk_label_set_text(GTK_LABEL(states_route_label), "");
    gtk_entry_set_text(GTK_ENTRY(string_entry), "");
}

void on_back_button_clicked(GtkButton *b)
{
	gtk_widget_show(prev_window);
	gtk_widget_destroy(GTK_WIDGET(evaluation_window));
}

void on_quit_button_clicked(GtkButton * b) {
    g_signal_emit_by_name(evaluation_window, "delete-event");
}

void on_string_entry_insert_text(
                        GtkEntry *entry,
                        const gchar *text,
                        gint length,
                        gint *position,
                        gpointer data) 
{
    GtkEditable *editable = GTK_EDITABLE (entry);
    int i, count = 0;
    gchar *result = g_new (gchar, length);

    char *alphabet = get_symbols();

    for (i=0; i < length; i++)
    {
        //Verify if the char is in the alphabet
        if (!strchr(alphabet, text[i]))
            continue;
        result[count++] = text[i];
    }

    if (count > 0)
    {
        g_signal_handlers_block_by_func(G_OBJECT (editable), G_CALLBACK (on_string_entry_insert_text), data);
        gtk_editable_insert_text(editable, result, count, position);
        
        g_signal_handlers_unblock_by_func(G_OBJECT (editable), G_CALLBACK (on_string_entry_insert_text), data);
    }

    g_signal_stop_emission_by_name(G_OBJECT (editable), "insert_text");
    g_free (result);
}

