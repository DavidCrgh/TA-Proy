#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#include "setup_ui.h"
#include "common.h"

// Extra columns for transition table grid
#define HEADER_ROWS 1
#define LEFT_COLS 2
#define RIGHT_COLS 1

// General global vars
int num_states;
int num_symbols;

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

static void build_transition_grid(GtkWidget *widget, gpointer data) {

    num_states = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(states_spin));
    num_symbols = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(symbols_spin));

    printf("NUM_STATES: %d \t NUM_SYMBOLS: %d\n\n", num_states, num_symbols);

    GtkWidget *wdg;
    gchar *text;
    //GtkAdjustment *adjustment;
    
    // 1. Build the header row (2 blank spaces, num_symbol entry boxes, and a "Accept?" label)

    for (int i = 0; i < num_symbols; i++) {
        wdg = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), wdg, i + LEFT_COLS, 0, 1, 1); 
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
    reset_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_reset"));
    eval_button = GTK_WIDGET(gtk_builder_get_object(builder, "setup_eval"));


    // Attach the callbacks for buttons and other controls
    g_signal_connect(ok_button, "clicked", G_CALLBACK (build_transition_grid), window);
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

    gtk_main();

    // TODO: unref builder object
    // TODO: find out how to kill the process when user closes GUI

    return OK;
}