#include <math.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>

#define PI        3.14
#define PI_DEGREE 180

#define WINDOW_HORZ_SIZE  1000
#define WINDOW_VERT_SIZE  300

#define DA_HORZ_SIZE  3000
#define DA_VERT_SIZE  WINDOW_VERT_SIZE

double magnitude = 100;
double frequency = 1;

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

    for (angle = 1; angle < DA_HORZ_SIZE; angle++) {
        x = angle;
        y = sw_sine(x*frequency) * magnitude;
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

  sw_line(1, 1, 1, DA_VERT_SIZE);
  sw_line(1, DA_VERT_SIZE, DA_HORZ_SIZE, DA_VERT_SIZE);
  sw_line(DA_HORZ_SIZE, DA_VERT_SIZE, DA_HORZ_SIZE, 1);
  sw_line(DA_HORZ_SIZE, 1, 1, 1);
  Mid_X = 1;
  Mid_Y = DA_VERT_SIZE/2;
  sw_line(Mid_X, Mid_Y, Mid_X + DA_HORZ_SIZE, Mid_Y);
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
    magnitude += 3;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Mag+ %d\n", (int)magnitude);
}

static void button2_clicked_decrease_cb (GtkButton *button, GtkWidget *other_widget)
{
    magnitude -= 3;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Mag- %d\n", (int)magnitude);
}

static void button3_clicked_increase_cb (GtkButton *button, GtkWidget *other_widget)
{
    frequency += 1;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Freq+ %d\n", (int)frequency);
}

static void button4_clicked_decrease_cb (GtkButton *button, GtkWidget *other_widget)
{
    frequency -= 1;
    gtk_widget_queue_draw (global_drawing_area);
    printf("Pressed Freq- %d\n", (int)frequency);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *main_vbox;
  GtkWidget *frame_horz;
  GtkWidget *fixed;
  GtkWidget *button1, *button2, *button3, *button4;
  GtkWidget *scrolled_window;
  GtkWidget *button_box;
  GtkWidget *grid;


  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "SineWave");
  gtk_window_set_default_size (GTK_WINDOW (window), WINDOW_HORZ_SIZE, WINDOW_VERT_SIZE);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);

  grid = gtk_grid_new ();
  gtk_container_add (GTK_CONTAINER (window), grid);

  main_vbox = gtk_box_new (FALSE, 0);
  gtk_widget_set_size_request (main_vbox, WINDOW_HORZ_SIZE, WINDOW_VERT_SIZE-100);
  //gtk_container_add (GTK_CONTAINER (window), main_vbox);
  gtk_grid_attach (GTK_GRID (grid), main_vbox, 0, 0, 10, 10);

  frame_horz = gtk_frame_new ("Horizontal Button Boxes");
  gtk_widget_set_size_request (frame_horz, WINDOW_HORZ_SIZE, WINDOW_VERT_SIZE-100);
  gtk_box_pack_start (GTK_BOX (main_vbox), frame_horz, TRUE, TRUE, 10);

  scrolled_window = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_size_request (scrolled_window, WINDOW_HORZ_SIZE, WINDOW_VERT_SIZE-100);
  gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 10);
  /* the policy is one of GTK_POLICY AUTOMATIC, or GTK_POLICY_ALWAYS.
   * GTK_POLICY_AUTOMATIC will automatically decide whether you need
   * scrollbars, whereas GTK_POLICY_ALWAYS will always leave the scrollbars
   * there.  The first one is the horizontal scrollbar, the second,
   * the vertical. */
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window),
                                GTK_POLICY_AUTOMATIC, GTK_POLICY_NEVER);
  gtk_container_add(GTK_CONTAINER(frame_horz), scrolled_window);
  gtk_widget_show (scrolled_window);

  /* Drawing Area of Fixed Window */
  GtkWidget *drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, DA_HORZ_SIZE, DA_VERT_SIZE);
  gtk_container_add (GTK_CONTAINER (scrolled_window), drawing_area);

   // tell GTK what function to call to draw the window
  g_signal_connect(G_OBJECT(drawing_area), "draw",
            G_CALLBACK(on_draw_event), NULL);
  g_signal_connect (G_OBJECT(drawing_area),"configure-event",
                  G_CALLBACK (configure_event_cb), NULL);
  global_drawing_area = drawing_area;

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  gtk_widget_set_size_request (button_box, 100, 100);
  gtk_grid_attach (GTK_GRID (grid), button_box, 5, 11, 1, 1);

  button1 = gtk_button_new_with_label ("Mag+");
  g_signal_connect_swapped (button1, "clicked", G_CALLBACK (button1_clicked_increase_cb), window);
  gtk_container_add (GTK_CONTAINER (button_box), button1);
  gtk_widget_set_size_request(button1, 80, 30);

  button2 = gtk_button_new_with_label ("Mag-");
  g_signal_connect_swapped (button2, "clicked", G_CALLBACK (button2_clicked_decrease_cb), window);
  gtk_container_add (GTK_CONTAINER (button_box), button2);
  gtk_widget_set_size_request(button2, 80, 30);

  button3 = gtk_button_new_with_label ("Feq+");
  g_signal_connect_swapped (button3, "clicked", G_CALLBACK (button3_clicked_increase_cb), window);
  gtk_container_add (GTK_CONTAINER (button_box), button3);
  gtk_widget_set_size_request(button1, 80, 30);

  button4 = gtk_button_new_with_label ("Freq-");
  g_signal_connect_swapped (button4, "clicked", G_CALLBACK (button4_clicked_decrease_cb), window);
  gtk_container_add (GTK_CONTAINER (button_box), button4);
  gtk_widget_set_size_request(button4, 80, 30);


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
