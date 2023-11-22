// Test this code with: g++ main_gtk.cpp image.cpp `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -o main_window

#include <iostream>
#include "../headers/image.h"
#include <gtk/gtk.h>

#define BYTES_PER_PIXEL 3

int main()
{
    Image test("../images/kity.jpg");

    cols = test.w;
    rows = test.h;

    tk_init(&argc, &argv);
 
    GdkPixbuf *pb = gdk_pixbuf_new_from_data(
        test.data,
        GDK_COLORSPACE_RGB,     // colorspace (must be RGB)
        0,                      // has_alpha (0 for no alpha)
        8,                      // bits-per-sample (must be 8)
        cols, rows,             // cols, rows
        cols * BYTES_PER_PIXEL, // rowstride
        NULL, NULL              // destroy_fn, destroy_fn_data
    );

    GtkWidget *image = gtk_image_new_from_pixbuf(pb);
    
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Image");
    gtk_window_set_default_size(GTK_WINDOW(window), cols, rows);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_container_add(GTK_CONTAINER(window), image);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    
    gtk_main();

    return 0;  
}