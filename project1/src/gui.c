#include <gtk/gtk.h>

#include "gui.h"
#include "common.h"

// Global vars for widget refs
GtkBuilder *builder;
GtkWidget *window;
GError *error = NULL;

int init_gui(int argc, char *argv[]) {

    gtk_init(&argc, &argv);

    builder = gtk_builder_new ();
    
    if (gtk_builder_add_from_file (builder, "src/setup.ui", &error) == 0)
    {
      g_printerr ("Error loading file: %s\n", error->message);
      g_clear_error (&error);
      return 1;
    }
    

    // Here you should get the ref to all the widgets you need to query from the builder
    // To do this simply call gtk_builder_get_object(builder, "your_widget_name"), cast it to the relevant widget type,
    // and save it to a variable

    window = GTK_WIDGET(gtk_builder_get_object(builder, "setup_window"));

    gtk_main();

    return OK;
}