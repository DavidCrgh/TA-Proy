#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "setup_ui.h"
#include "evaluation_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/list.h"
#include "../logic/dfa.h"
#include "../logic/controller.h"

// Extra columns for transition table grid
#define HEADER_ROWS 1
#define LEFT_COLS 3
#define RIGHT_COLS 1

#define MAX_TAGS_LEN  20
#define MAX_CB_STRING_LEN 10
#define HEADER_BOTTOM_MARGIN 15
#define ENTRY_CENTER_ALIGNMENT 0.5

// Variable for alphabet
char* symbols_ui = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Global vars for widget refs
static GtkBuilder *builder;

static GtkWidget *window;

static GtkWidget *grid;

static GtkWidget *states_spin;
static GtkWidget *symbols_spin;

static GtkWidget *ok_button;
static GtkWidget *eval_button;
static GtkWidget *quit_button;

char **combo_strings = NULL;

GError *error = NULL;

///////////// PRIVATE FUNCTIONS ///////////////////////////////////////////////

static void next_window(){
    gtk_widget_hide(window);
    init_gui_eval(window);
}

static void on_window_delete_event(GtkWidget *widget, gpointer data)
{
	gtk_widget_destroy(window);
	exit(EXIT_SUCCESS);
}


static void quit_clicked(GtkButton * b, gpointer data) {
    g_signal_emit_by_name(window, "delete-event");
}

static void free_combo_strings() {
    if (combo_strings != NULL) {
        
        freeMatrix((void**) combo_strings, num_states + 1);
    }
}


static void init_combo_strings() {


    combo_strings = (char**) createMatrix(num_states + 1, MAX_CB_STRING_LEN, sizeof(char*));

    for (int i = 0; i <= num_states; i++) {


        if (i == 0) {
            snprintf(combo_strings[i], MAX_CB_STRING_LEN, "-");

        } else {

            snprintf(combo_strings[i], MAX_CB_STRING_LEN, "%d", i);
        }
    }

}

static GtkWidget* create_combobox() {
    GtkWidget *combobox;
    
    combobox = gtk_combo_box_text_new();

    for (int i = 0; i <= num_states; i++) {

        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combobox), NULL, combo_strings[i]);
        gtk_combo_box_set_active(GTK_COMBO_BOX(combobox), 0);
    }


    return combobox;
}


static int get_combobox_value(GtkWidget *cb) {

    int selected_item = gtk_combo_box_get_active(GTK_COMBO_BOX(cb));
    gchar *text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cb));

    return (selected_item == 0 ? -1 : atoi(text) - 1);

}

static void call_message_box(char *message)
{
	GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
														GTK_DIALOG_MODAL,
														GTK_MESSAGE_INFO,
														GTK_BUTTONS_OK,
														"%s", message);
	gtk_dialog_run(GTK_DIALOG(dialog));
	gtk_widget_destroy(dialog);
}

static char *get_alphabet_from_table(GtkGrid *actual_grid)
{
	char *entry_values = (char *) createList(num_symbols + 1, sizeof(char));
	for(int i = 0; i < num_symbols; i++)
	{
		GtkWidget *header_symbol = gtk_grid_get_child_at(actual_grid, i + LEFT_COLS, 0);
		char *symbol;
		char message[100];
		symbol = strdup(gtk_entry_get_text(GTK_ENTRY(header_symbol)));
		if(symbol[0] == '\0')
		{
			sprintf(message, "There is no symbol on column %d", i + 1);
			call_message_box(message);
			return NULL;
		}
		else
		{
			char *ch = strchr(entry_values, symbol[0]);
			if(ch != NULL)
			{
				sprintf(message, "The symbol '%s' is repeated in the table", symbol);
				call_message_box(message);
				return NULL;
			}
			if (i == 0)
				strcpy(entry_values, gtk_entry_get_text(GTK_ENTRY(header_symbol)));
			else
				strcat(entry_values, gtk_entry_get_text(GTK_ENTRY(header_symbol)));	
		}
	}
	return entry_values;
}

static void get_datas(GtkWidget *widget, gpointer data)
{
	GtkGrid *actual_grid = GTK_GRID(grid);

	// Get configuration for automaton
	// 1. Get data of headers row
	char *entry_values = get_alphabet_from_table(actual_grid);
	if(entry_values == NULL)
		return;
	
	// 2. Get state tags and acceptance states
	char **entry_data = (char **) createMatrix(num_states, 30, sizeof(char *));
	
	int *acceptance_states = (int *)createList(num_states, sizeof(int));
	for(int j = 0; j < num_states; j++)
	{
		// Tags
		GtkWidget *wdg = gtk_grid_get_child_at(actual_grid, 0, j + HEADER_ROWS);
		entry_data[j] = strdup(gtk_entry_get_text(GTK_ENTRY(wdg)));
		
		// Acceptance
		wdg = gtk_grid_get_child_at(actual_grid, LEFT_COLS + num_symbols, j + HEADER_ROWS);
		gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wdg));
		acceptance_states[j] = (checked ? 1 : 0); 
	}
	
	// 3. Get values from matrix
	int ** table = (int **)createMatrix(num_states, num_symbols, sizeof(int));
	
	for (int i = 0; i < num_states; i++) 
	{
      	for (int j = 0; j < num_symbols; j++) 
      	{
      		GtkWidget *state_to_visit = gtk_grid_get_child_at(actual_grid, j + LEFT_COLS, i + HEADER_ROWS);
            table[i][j] = get_combobox_value(state_to_visit);
      	}
    }
    // Finish get configuration for automaton
    
    // NOTE 1: the arrays passed to set_machine_config shouldn't be freed manually,
    // otherwise a segfault will occur when executing the automaton. To avoid leaks,
    // the controller frees the memory on repeated calls to set_machine_config
    //
    // NOTE 2: the DFA driver can't be called yet because we haven't received the input
    // string to process

    set_machine_config(
        table, 
        entry_data,         // State tags
        acceptance_states, 
        entry_values,       // Alphabet symbols
        0);                 // Initial state

    next_window();
}

static void build_transition_grid(GtkWidget *widget, gpointer data) {

    free_combo_strings();

	num_states = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(states_spin));
    num_symbols = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(symbols_spin));
    
	
	GtkWidget *wdg;
    gchar *text;
    
    // 0. Reset grid widget
	GList *elements = gtk_container_get_children(GTK_CONTAINER(grid));

    GList *iterator;
    for (iterator = elements; iterator != NULL; iterator = g_list_next(iterator)) {
        gtk_widget_destroy(GTK_WIDGET(iterator->data));
    }
    g_list_free(elements);
    
    // 1. Build the header row (2 blank spaces, num_symbol entry boxes, and a "Accept?" label)
    GtkEntry *entries[num_symbols];
    for (int i = 0; i < num_symbols; i++) {
        entries[i] = GTK_ENTRY(gtk_entry_new());
        gtk_entry_set_max_length(entries[i], 1);
        gtk_entry_set_text(entries[i], get_char(symbols_ui[i]));
        gtk_entry_set_alignment(entries[i], ENTRY_CENTER_ALIGNMENT);
        gtk_widget_set_margin_bottom(GTK_WIDGET(entries[i]), HEADER_BOTTOM_MARGIN);

        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(entries[i]), i + LEFT_COLS, 0, 1, 1);
    }

    wdg = gtk_label_new("Accept?");
    gtk_widget_set_margin_bottom(wdg, 15);
    gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + num_symbols, 0, 1, 1); 

    // 2. Build left side columns (1 entry box, 1 label) x num_states
    for (int i = 0; i < num_states; i++){
      
        text = g_strdup_printf("%d", i + 1);
        wdg = gtk_entry_new_with_buffer(gtk_entry_buffer_new(text, -1));
        gtk_entry_set_max_length(GTK_ENTRY(wdg), MAX_TAGS_LEN);
        gtk_entry_set_alignment(GTK_ENTRY(wdg), ENTRY_CENTER_ALIGNMENT);
        gtk_grid_attach(GTK_GRID(grid), wdg, 0, i + HEADER_ROWS, 1, 1);

        if (i == 0) {
            text = g_strdup_printf("<b>%d (START)</b>", i + 1);
            wdg = gtk_label_new(text);
            gtk_label_set_markup(GTK_LABEL(wdg), text);

        } else {
            wdg = gtk_label_new(text);
        }

        gtk_label_set_xalign (GTK_LABEL(wdg), 0.0);
        gtk_grid_attach(GTK_GRID(grid), wdg, 1, i + HEADER_ROWS, 1, 1);

        // 3. Build right side column (1 check box)
        wdg = gtk_check_button_new();
        gtk_widget_set_halign(wdg, GTK_ALIGN_CENTER);
        gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + num_symbols, i + HEADER_ROWS, 1, 1);
    }

    
    // 4. Build state x symbol spinbox matrix

    init_combo_strings();

    for (int i = 0; i < num_states; i++) {
      	for (int j = 0; j < num_symbols; j++) {

            wdg = create_combobox();
          	gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + j, i + HEADER_ROWS, 1, 1);

      	}
    }
    
    gtk_widget_set_sensitive(eval_button, TRUE);
    gtk_widget_show_all(window);
}

void init_widget_refs() {

    // Extract the widget refs from the builder

    window = GTK_WIDGET(gtk_builder_get_object(builder, "setup_window"));

    grid = GTK_WIDGET(gtk_builder_get_object(builder, "setup_grid"));
	
	states_spin = GTK_WIDGET(gtk_builder_get_object(builder, "states_spin"));
    symbols_spin = GTK_WIDGET(gtk_builder_get_object(builder, "symbols_spin"));

    ok_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_ok"));
    eval_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_evaluate"));
    quit_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_quit"));

	// Attach the callbacks for window, buttons and other controls
    g_signal_connect(G_OBJECT(window), "delete-event", G_CALLBACK (on_window_delete_event), NULL);
    g_signal_connect(quit_button, "clicked", G_CALLBACK (quit_clicked), NULL);
    g_signal_connect(eval_button, "clicked", G_CALLBACK (get_datas), NULL);
    g_signal_connect(ok_button, "clicked", G_CALLBACK (build_transition_grid), NULL);
    
}

///////////// PUBLIC FUNCTIONS ////////////////////////////////////////////////

int init_gui(int argc, char *argv[]) {
  
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (gtk_builder_add_from_file (builder, "src/ui/setup.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }

    init_widget_refs();

	g_object_unref(builder);    //Unref builder
    gtk_main();

    return OK;
}
