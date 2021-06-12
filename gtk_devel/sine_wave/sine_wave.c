#include <math.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>

#define PI        3.14
#define PI_DEGREE 180

double magnitude = 100;

double          Mid_X, Mid_Y;
GtkWidget       *global_drawing_area;
cairo_t         *global_cr = NULL;
cairo_surface_t *surface = NULL;

static void set_surface_cr(cairo_t *cr)
{
    global_cr = cr;
}

static void
clear_surface (void)
{
  cairo_set_source_rgb (global_cr, 1, 1, 1);
  cairo_paint (global_cr);
}

double sw_sine(double angle)
{
    double x = angle/PI_DEGREE * PI;
    double res = sin(x);
    return res;
}

void sw_line(int x1, int y1, int x2, int y2) {
  // set the line color to blue and line width to 2 points
  cairo_set_source_rgb(global_cr, 0,0,255);
  cairo_set_line_width(global_cr, 2.0);
  // to draw, you move the "cursor" somewhere first...
  cairo_move_to(global_cr, x1, y1);
  // and then call cairo_line_to to draw a line to wherever you want
  cairo_line_to(global_cr, x2, y2);
  // the path you made isn't actually drawn until you call cairo_stroke.
  cairo_stroke(global_cr);
}

void draw_sine_wave()
{
    int angle;
    double x, y;
    int x1=1, y1=Mid_Y, x2=1, y2=Mid_Y;
    cairo_t   *cr;
    //clear_surface();

    for (angle = 1; angle < 400; angle++) {
        x = angle;
        y = sw_sine(x) * magnitude;
        //printf("x=%f y=%f\n",x, y);
        x2 = x;
        y2 = y + Mid_Y;
        sw_line(x1, y1, x2, y2);
        x1 = x2;
        y1 = y2;
    }
}


/* This is the function that actually draws the lines (or anything else).
 * It gets called when the window is displayed to the user.  */
void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  set_surface_cr(cr);
  clear_surface();
  printf("%s\n", __func__);

  sw_line(1, 1, 1, 400);
  sw_line(1, 400, 400, 400);
  sw_line(400, 400, 400, 1);
  sw_line(400, 1, 1, 1);
  Mid_X = 1;
  Mid_Y = 400/2;
  sw_line(Mid_X, Mid_Y, Mid_X +400, Mid_Y);
  draw_sine_wave();
}

/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
  printf("%s\n", __func__);
  if (surface)
    cairo_surface_destroy (surface);
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));

  /* Initialize the surface to white */
  //clear_surface ();

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

static void button1_clicked_increase_cb (GtkButton *button, GtkWidget *other_widget)
{
    magnitude += 10;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Mag+ %d\n", (int)magnitude);
}

static void button2_clicked_decrease_cb (GtkButton *button, GtkWidget *other_widget)
{
    magnitude -= 10;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Mag- %d\n", (int)magnitude);
}


static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *fixed;
  GtkWidget *button1, *button2;


  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "SineWave");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

  /* Fixed Window of Main Window */
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);

  /* Drawing Area of Fixed Window */
  GtkWidget *drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 400, 400);
  gtk_fixed_put(GTK_FIXED(fixed), drawing_area, 1, 1);

   // tell GTK what function to call to draw the window
  g_signal_connect(G_OBJECT(drawing_area), "draw",
            G_CALLBACK(on_draw_event), NULL);
  g_signal_connect (G_OBJECT(drawing_area),"configure-event",
                  G_CALLBACK (configure_event_cb), NULL);

  global_drawing_area = drawing_area;
  button1 = gtk_button_new_with_label ("Mag+");
  g_signal_connect_swapped (button1, "clicked", G_CALLBACK (button1_clicked_increase_cb), drawing_area);
  gtk_fixed_put(GTK_FIXED(fixed), button1, 100, 490);
  gtk_widget_set_size_request(button1, 80, 30);

  button2 = gtk_button_new_with_label ("Mag-");
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (button2_clicked_decrease_cb), drawing_area);
  gtk_fixed_put(GTK_FIXED(fixed), button2, 200, 490);
  gtk_widget_set_size_request(button2, 100, 30);

  gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.sine.ware", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
