#include <math.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <cairo.h>
double Mid_X, Mid_Y;
cairo_t *global_cr;
double magnitude=180;
gpointer g_user_data;
void *ptr;
static cairo_surface_t *surface = NULL;

static void                           
clear_surface (void)                  
{                                     
  cairo_t *cr;                        
                                      
  cr = cairo_create (surface);        
                                      
  cairo_set_source_rgb (cr, 1, 1, 1); 
  cairo_paint (cr);                   
                                      
  cairo_destroy (cr);                 
}                                     

double sw_sine(double angle)
{
    double x = angle/180 * 3.14;
    double res = sin(x);
    return res;
}

void sw_line(int x1, int y1, int x2, int y2) {
  // to draw, you move the "cursor" somewhere first...
  cairo_move_to(global_cr, x1, y1);
  // and then call cairo_line_to to draw a line to wherever you want
  cairo_line_to(global_cr, x2, y2);
  // the path you made isn't actually drawn until you call cairo_stroke.
  //cairo_paint(global_cr);
  cairo_stroke(global_cr);
}

void draw_sine_wave(gpointer data)
{
    int angle;
    double x, y;
    int x1=1, y1=Mid_Y, x2=1, y2=Mid_Y;
    cairo_t   *cr;
    clear_surface();
    if (global_cr) {
       //cairo_destroy(global_cr);
    }
    //global_cr = gdk_cairo_create (gtk_widget_get_window (w->da));
    //cairo_scale (global_cr, 100, 100);
    
     cr = cairo_create (surface);               
                                            
    global_cr = cr;

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
    cairo_fill (cr);                           
    cairo_destroy (cr);                        
}


/* This is the function that actually draws the lines (or anything else).
 * It gets called when the window is displayed to the user.  */
void on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);
  printf("%s\n", __func__);
  // set the line color to blue and line width to 2 points
  cairo_set_source_rgb(cr, 0,0,255);
  cairo_set_line_width(cr, 2.0);
  global_cr = cr;

  sw_line(1, 1, 1, 400);
  sw_line(1, 400, 400, 400);
  sw_line(400, 400, 400, 1);
  sw_line(400, 1, 1, 1);
  Mid_X = 1;
  Mid_Y = 400/2;
  sw_line(Mid_X, Mid_Y, Mid_X +400, Mid_Y);
  g_user_data = user_data;
  draw_sine_wave(user_data);
}

/* Create a new surface of the appropriate size to store our scribbles */          
static gboolean                                                                 
configure_event_cb (GtkWidget         *widget,                                  
                    GdkEventConfigure *event,                                   
                    gpointer           data)                                    
{                                                                               
  if (surface)                                                                  
    cairo_surface_destroy (surface);                                            
                                                                                
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),  
                                               CAIRO_CONTENT_COLOR,             
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));
                                                                                
  /* Initialize the surface to white */                                         
  clear_surface ();                                                             
                                                                                
  /* We've handled the configure event, no need for further processing. */         
  return TRUE;                                                                  
}                       

static void button1_clicked_increase_cb (GtkButton *button, GtkWidget *other_widget)
{
    clear_surface();
    gtk_widget_queue_draw (other_widget);
    magnitude += 10;
    gtk_widget_queue_draw_area (other_widget, 0, 0, 0, 0);
}

static void button2_clicked_decrease_cb (GtkButton *button, GtkWidget *other_widget)
{
    clear_surface();
    gtk_widget_queue_draw (other_widget);
    magnitude -= 10;
    gtk_widget_queue_draw_area (other_widget, 0, 0, 0, 0);
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
  
  fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);

  GtkWidget *drawing_area = gtk_drawing_area_new ();
  gtk_widget_set_size_request (drawing_area, 400, 400);
  ///gtk_fixed_pack_start (GTK_BOX (box), , TRUE, TRUE, 0);
  gtk_fixed_put(GTK_FIXED(fixed), drawing_area, 1, 1);

   // tell GTK what function to call to draw the window
  g_signal_connect(G_OBJECT(drawing_area), "draw",
            G_CALLBACK(on_draw_event), NULL);
  g_signal_connect (G_OBJECT(drawing_area),"configure-event",
                  G_CALLBACK (configure_event_cb), NULL);

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
