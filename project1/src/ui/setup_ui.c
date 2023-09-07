#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "setup_ui.h"
#include "../logic/common.h"
#include "../logic/matrix.h"
#include "../logic/dfa.h"
#include "../logic/controller.h"

// Extra columns for transition table grid
#define HEADER_ROWS 1
#define LEFT_COLS 2
#define RIGHT_COLS 1

// Global vars for widget refs
GtkBuilder *builder;

GtkWidget *window;

GtkWidget *grid;

GtkWidget *states_spin;
GtkWidget *symbols_spin;

GtkWidget *ok_button;
GtkWidget *reset_button;
GtkWidget *eval_button;

GError *error = NULL;
///////////// PRIVATE FUNCTIONS ///////////////////////////////////////////////


static void get_datas(GtkWidget *widget, gpointer data)
{
	GtkGrid *actual_grid = GTK_GRID(grid);
	// Get configuration for automaton
	// 1. Get data of headers row
	char *entry_values = (char *) malloc(num_symbols + 1);
	for(int i = 0; i < num_symbols; i++)
	{
		GtkWidget *header_symbol = gtk_grid_get_child_at(actual_grid, i + LEFT_COLS, 0);
		if (i == 0)
			strcpy(entry_values, gtk_entry_get_text(GTK_ENTRY(header_symbol)));
		else
			strcat(entry_values, gtk_entry_get_text(GTK_ENTRY(header_symbol)));
	}
	g_print("Symbols to use: %s\n", entry_values);
	
	
	// 2. Get state tags and acceptance states
	char **entry_data = (char **) malloc(num_states * sizeof(char *));
	
	int *acceptance_states = createList(num_states);
	for(int j = 0; j < num_states; j++)
	{
		// Tags
		GtkWidget *wdg = gtk_grid_get_child_at(actual_grid, 0, j + HEADER_ROWS);
		entry_data[j] = strdup(gtk_entry_get_text(GTK_ENTRY(wdg)));
		g_print("Element %d: %s\n", j + HEADER_ROWS, entry_data[j]);
		
		// Acceptance
		wdg = gtk_grid_get_child_at(actual_grid, LEFT_COLS + num_symbols, j + HEADER_ROWS);
		gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(wdg));
		acceptance_states[j] = (checked ? 1 : 0); 
		g_print("\tAcceptance %d: %d\n", j + HEADER_ROWS, acceptance_states[j]);
	}
	
	// 3. Get values from matrix
	int ** table = createMatrix(num_states, num_symbols);
	
	for (int i = 0; i < num_states; i++) 
	{
      	for (int j = 0; j < num_symbols; j++) 
      	{
      		GtkWidget *state_to_visit = gtk_grid_get_child_at(actual_grid, j + LEFT_COLS, i + HEADER_ROWS);
      		table[i][j] = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(state_to_visit)) - 1;
      		g_print("Table[%d][%d]=%d\n", i, j, table[i][j]);
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
        entry_data,         // State labels
        acceptance_states, 
        entry_values,       // Alphabet symbols
        0);                 // Initial state


    /*
    // Post evaluate
    // 4. Call DFA_driver
    char *input = "ababbbabbabbaab";
    int *sequence = createList(strlen(input) + 1);
    int result = dfa_driver(table, acceptance_states, code, input, 0, sequence);
    
    // 4.5 Print sequence and result (optional)
    g_print("Result: %d\n\n", result);
    g_print("Sequence:\n");
    for(int i = 0; i < strlen(input) + 1; i++)
    {
    	g_print("\tElement[%d]: %s\n", i, entry_data[sequence[i]]);
    }
    
    
    // 5. Free memory 
	free(sequence);
	freeMatrix(table, num_states);
	free(acceptance_states);
	for(int i = 0; i < num_states; i++)
	{
		free(entry_data[i]);
	}
	free(entry_data);	
	free(entry_values);
	*/
}

static void build_transition_grid(GtkWidget *widget, gpointer data) {

	num_states = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(states_spin));
    num_symbols = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(symbols_spin));

    g_print("NUM_STATES: %d \t NUM_SYMBOLS: %d\n\n", num_states, num_symbols);
	
	GtkWidget *wdg;
    gchar *text;
    //GtkAdjustment *adjustment;
    
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
        
        gtk_entry_set_text(entries[i], get_char(symbols[i]));
        gtk_grid_attach(GTK_GRID(grid), GTK_WIDGET(entries[i]), i + LEFT_COLS, 0, 1, 1);
    }
    wdg = gtk_label_new("Accept?");
    gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + num_symbols, 0, 1, 1); 

    // 2. Build left side columns (1 entry box, 1 label) x num_states
    for (int i = 0; i < num_states; i++){
      
        text = g_strdup_printf("%d", i + 1);
        wdg = gtk_entry_new_with_buffer(gtk_entry_buffer_new(text, -1));
        gtk_grid_attach(GTK_GRID(grid), wdg, 0, i + HEADER_ROWS, 1, 1);


        wdg = gtk_label_new(text);
        gtk_grid_attach(GTK_GRID(grid), wdg, 1, i + HEADER_ROWS, 1, 1);

        // 3. Build right side column (1 check box)
        wdg = gtk_check_button_new();
        gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + num_symbols, i + HEADER_ROWS, 1, 1);
    }

    
    // 4. Build state x symbol spinbox matrix
    /*char *combobox_strings[num_states + 1];

    combobox_strings[0] = "-";

    for (int i = 0; i < num_states; i++) {
      combobox_strings[i + 1] = (char *) g_strdup_printf("%d", i + 1);
    }*/
    for (int i = 0; i < num_states; i++) {
      	for (int j = 0; j < num_symbols; j++) {

          	//wdg = gtk_drop_down_new_from_strings(combobox_strings);
          	wdg = gtk_spin_button_new_with_range(0, num_states, 1);
          	gtk_grid_attach(GTK_GRID(grid), wdg, LEFT_COLS + j, i + HEADER_ROWS, 1, 1);
      	}
    }
    
    gtk_widget_show_all(window);
}

void init_widget_refs() {

    // Extract the widget refs from the builder

    window = GTK_WIDGET(gtk_builder_get_object(builder, "setup_window"));

    grid = GTK_WIDGET(gtk_builder_get_object(builder, "setup_grid"));
	
	states_spin = GTK_WIDGET(gtk_builder_get_object(builder, "states_spin"));
    symbols_spin = GTK_WIDGET(gtk_builder_get_object(builder, "symbols_spin"));

    ok_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_ok"));
    // reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_reset"));
    eval_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_evaluate"));

	// Attach the callbacks for window, buttons and other controls
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
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

	g_object_unref(builder);	//Unref builder
    gtk_main();

    return OK;
}
