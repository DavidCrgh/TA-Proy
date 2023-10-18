#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "evaluation_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/list.h"
#include "../logic/controller.h"

#define LEFT_COLS 0
#define LABEL_MARGIN 15
#define EPSILON "\u03B5"

static GtkBuilder *builder_eval;

static GtkWidget *back_button;
static GtkWidget *evaluation_window;
static GtkWidget *evaluate_button;
static GtkWidget *evaluation_grid;
static GtkWidget *prev_window;
static GtkWidget *quit_button;
static GtkWidget *reset_button;
static GtkWidget *string_entry;

void set_label_margins(GtkLabel *label, int margin){
    gtk_widget_set_margin_top(GTK_WIDGET(label), margin);
    gtk_widget_set_margin_bottom(GTK_WIDGET(label), margin);
    gtk_widget_set_margin_start(GTK_WIDGET(label), margin);
    gtk_widget_set_margin_end(GTK_WIDGET(label), margin);
}

void cleanGrid(GtkWidget *widget, gpointer data){
    GList *elements = gtk_container_get_children(GTK_CONTAINER(evaluation_grid));

    GList *iterator;
    for (iterator = elements; iterator != NULL; iterator = g_list_next(iterator)) {
        gtk_widget_destroy(GTK_WIDGET(iterator->data));
    }
    g_list_free(elements);
}
static void build_transition_grid(GtkWidget *widget, gpointer data) {
	cleanGrid(widget, data);

    char *input = (char *) malloc(gtk_entry_get_text_length (GTK_ENTRY(string_entry)));;
    strcpy(input, gtk_entry_get_text(GTK_ENTRY(string_entry)));
    int len_input = strlen(input);

    int *sequence = (int*)createList(len_input + 1, sizeof(int));
    fillList(sequence, len_input + 1);

    int result = execute_machine(input, sequence);
    
    //Add labels and headers to the evaluation_grid
    GtkLabel *acceptance_label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(acceptance_label), "<b>Acceptance state</b>");
    set_label_margins(acceptance_label, LABEL_MARGIN);

    GtkLabel *acceptance_result_label = GTK_LABEL(gtk_label_new(NULL));
    set_label_margins(acceptance_result_label, LABEL_MARGIN);
    

    GtkLabel *route_label = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(route_label), "<b>Route</b>");
    set_label_margins(route_label, LABEL_MARGIN);

    GtkLabel *header_labels[3];

    header_labels[0] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[0]), "<b>Current state</b>");
    set_label_margins(header_labels[0], LABEL_MARGIN);

    header_labels[1] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[1]), "<b>Symbol</b>");
    set_label_margins(header_labels[1], LABEL_MARGIN);

    header_labels[2] = GTK_LABEL(gtk_label_new(NULL));
    gtk_label_set_markup(GTK_LABEL(header_labels[2]), "<b>Next state</b>");
    set_label_margins(header_labels[2], LABEL_MARGIN);


    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(acceptance_label), 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(acceptance_result_label), 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(route_label), 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(header_labels[0]), LEFT_COLS, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(header_labels[1]), 1 + LEFT_COLS, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(header_labels[2]), 2 + LEFT_COLS, 4, 1, 1);

    //Display result
    if(result == 1) {
        gtk_label_set_text(GTK_LABEL(acceptance_result_label), "Accepted");
    } else {
        gtk_label_set_text(GTK_LABEL(acceptance_result_label), "Rejected");
    }

    int number_states = strlen(input) + 2;

    GtkLabel *current_state_labels[number_states];
    GtkLabel *symbols_labels[number_states];
    GtkLabel *next_state_labels[number_states];

    char **state_labels = get_state_labels();
    gchar *text;
    int flag = 0;
    for (int i = 0; i < number_states && flag == 0; i++) {
        if (sequence[i] == -1)
            flag = 1;
            
        if(sequence[i-1] >= 0){
            if(i == 0){
                current_state_labels[i] = GTK_LABEL(gtk_label_new("-"));
            } else {
                current_state_labels[i] = GTK_LABEL(gtk_label_new(state_labels[sequence[i-1]]));
            }
        } else {
            current_state_labels[i] = GTK_LABEL(gtk_label_new("-"));
        }
        set_label_margins(current_state_labels[i], LABEL_MARGIN);
        gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(current_state_labels[i]), LEFT_COLS, 4 + i + 1, 1, 1);

        if(i == 0){
            symbols_labels[i] = GTK_LABEL(gtk_label_new(EPSILON));
        } else if(i == (number_states - 1)){
            symbols_labels[i] = GTK_LABEL(gtk_label_new("-"));
        } else {
            text = g_strdup_printf("%c", input[i-1]);
            symbols_labels[i] = GTK_LABEL(gtk_label_new(text));
        }
        set_label_margins(symbols_labels[i], LABEL_MARGIN);
        gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(symbols_labels[i]), LEFT_COLS + 1, 4 + i + 1, 1, 1);

        if(sequence[i] >= 0){
            if(i < (number_states - 1)){
                next_state_labels[i] = GTK_LABEL(gtk_label_new(state_labels[sequence[i]]));
            } else {
                next_state_labels[i] = GTK_LABEL(gtk_label_new("-"));
            }
        } else {
            next_state_labels[i] = GTK_LABEL(gtk_label_new("-"));
        }
        set_label_margins(next_state_labels[i], LABEL_MARGIN);
        gtk_grid_attach(GTK_GRID(evaluation_grid), GTK_WIDGET(next_state_labels[i]), LEFT_COLS + 2, 4 + i + 1, 1, 1);
    }
    
    gtk_widget_show_all(evaluation_window);
}

void init_widgets() {
    back_button = GTK_WIDGET(gtk_builder_get_object(builder_eval, "back_button"));
    evaluation_window = GTK_WIDGET(gtk_builder_get_object(builder_eval, "evaluation_window"));
    evaluate_button = GTK_WIDGET(gtk_builder_get_object(builder_eval, "evaluate_button"));
    evaluation_grid = GTK_WIDGET(gtk_builder_get_object(builder_eval, "results_grid"));
    quit_button = GTK_WIDGET(gtk_builder_get_object(builder_eval, "finish_button"));
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder_eval, "reset_button"));
    string_entry = GTK_WIDGET(gtk_builder_get_object(builder_eval, "string_entry"));

    g_signal_connect(evaluate_button, "clicked", G_CALLBACK (build_transition_grid), NULL);

}

int init_gui_eval(GtkWidget *previous_window) {
	prev_window = previous_window;
    builder_eval = gtk_builder_new_from_file("src/ui/evaluation.ui");
    gtk_builder_connect_signals(builder_eval, NULL);

    init_widgets();
    gtk_widget_show(evaluation_window);

    gtk_main();

    return OK;
}

void on_evaluation_window_delete_event(GtkWidget *widget, gpointer data)
{
	free_config();
	gtk_widget_destroy(GTK_WIDGET(prev_window));
    gtk_widget_destroy(GTK_WIDGET(evaluation_window));
	exit(EXIT_SUCCESS);
}

void on_reset_button_clicked(GtkWidget *widget, gpointer data) {
    cleanGrid(widget, data);
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

