#include <gtk/gtk.h>
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10

typedef struct {
    GdkRGBA color;
} Ball;

GHashTable *ball_table;

void create_balls() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Ball *ball = g_new(Ball, 1);
        ball->color.red = (gdouble)rand() / RAND_MAX;
        ball->color.green = (gdouble)rand() / RAND_MAX;
        ball->color.blue = (gdouble)rand() / RAND_MAX;
        ball->color.alpha = 1.0;

        char key[16];
        snprintf(key, sizeof(key), "ball%d", i);
        g_hash_table_insert(ball_table, g_strdup(key), ball);
    }
}

void update_colors() {
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, ball_table);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        if (G_IS_OBJECT(value)) {
            Ball *ball = (Ball *)value;
            ball->color.red = (gdouble)rand() / RAND_MAX;
            ball->color.green = (gdouble)rand() / RAND_MAX;
            ball->color.blue = (gdouble)rand() / RAND_MAX;
            GtkWidget *drawing_area = GTK_WIDGET(g_hash_table_lookup(ball_table, key));
            if (drawing_area != NULL) {
                gtk_widget_queue_draw(drawing_area);
            }
        } else {
            g_warning("Invalid value in hash table!");
        }
    }
}



gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
    gint width, height;
    GdkRGBA color;
    GHashTableIter iter;
    gpointer key, value;

    width = gtk_widget_get_allocated_width(widget);
    height = gtk_widget_get_allocated_height(widget);

    cairo_set_source_rgb(cr, 1, 1, 1);  // White background
    cairo_paint(cr);

    g_hash_table_iter_init(&iter, ball_table);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Ball *ball = (Ball *)value;
        color = ball->color;

        // Draw a circle for each ball, place evenly across the area
        double x = (double)(rand() % (width - 40)) + 20;  // Ensure ball is inside the window
        double y = (double)(rand() % (height - 40)) + 20;

        cairo_set_source_rgba(cr, color.red, color.green, color.blue, color.alpha);
        cairo_arc(cr, x, y, 20, 0, 2 * G_PI);
        cairo_fill(cr);
    }

    return FALSE;
}


int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *drawing_area;

    gtk_init(&argc, &argv);

    ball_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hashtable Animation");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 400, 400);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    gtk_widget_show_all(window);

    create_balls();
    update_colors();

    g_timeout_add(1000, (GSourceFunc)update_colors, NULL);

    gtk_main();

    g_hash_table_destroy(ball_table);

    return 0;
}
