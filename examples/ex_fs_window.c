#include "allegro5/allegro5.h"
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "common.c"

static ALLEGRO_DISPLAY *display;
static ALLEGRO_BITMAP *picture;
static ALLEGRO_EVENT_QUEUE *queue;
static ALLEGRO_FONT *font;
static bool big;

static void redraw(void)
{
   ALLEGRO_COLOR color;
   int w = al_get_display_width();
   int h = al_get_display_height();
   int pw = al_get_bitmap_width(picture);
   int ph = al_get_bitmap_height(picture);
   int th = al_get_font_line_height(font);
   float cx =  (w - pw) * 0.5;
   float cy =  (h - ph) * 0.5;

   color = al_map_rgb_f(0.8, 0.7, 0.9);
   al_clear_to_color(color);

   color = al_map_rgb(255, 0, 0);
   al_draw_line(0, 0, w, h, color, 0);
   al_draw_line(0, h, w, 0, color, 0);

   al_draw_bitmap(picture, cx, cy, 0);
   
   al_draw_textf(font, w / 2, cy + ph, ALLEGRO_ALIGN_CENTRE,
      "Press Space to toggle fullscreen");
   al_draw_textf(font, w / 2, cy + ph + th, ALLEGRO_ALIGN_CENTRE,
      "Press Enter to toggle window size");
   al_draw_textf(font, w / 2, cy + ph + th * 2, ALLEGRO_ALIGN_CENTRE,
      "Window: %dx%d (%s)",
      al_get_display_width(), al_get_display_height(),
      (al_get_display_flags() & ALLEGRO_FULLSCREEN_WINDOW) ?
      "fullscreen" : "not fullscreen");
   
   al_flip_display();
}

static void run(void)
{
   ALLEGRO_EVENT event;
   bool quit = false;
   while (!quit) {
      while (al_get_next_event(queue, &event)) {
         if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            quit = true;
         else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
               quit = true;
            else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
               al_toggle_display_flag(ALLEGRO_FULLSCREEN_WINDOW,
                  !(al_get_display_flags() & ALLEGRO_FULLSCREEN_WINDOW));
               redraw();
            }
            else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
               big = !big;
               if (big)
                  al_resize_display(800, 600);
               else
                  al_resize_display(640, 480);
               redraw();
            }
         }
      }
      /* FIXME: Lazy timing */
      al_rest(0.02);
      redraw();
   }
}

int main(void)
{
   if (!al_init()) {
      abort_example("Could not init Allegro.\n");
      return 1;
   }

   al_init_primitives_addon();
   al_install_keyboard();
   al_init_image_addon();
   al_init_font_addon();

   al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
   display = al_create_display(640, 480);
   if (!display) {
      abort_example("Error creating display\n");
      return 1;
   }

   queue = al_create_event_queue();
   al_register_event_source(queue, al_get_keyboard_event_source());
   al_register_event_source(queue, al_get_display_event_source(display));

   picture = al_load_bitmap("data/mysha.pcx");
   if (!picture) {
      abort_example("mysha.pcx not found\n");
      return 1;
   }
   
   font = al_load_font("data/fixed_font.tga", 0, 0);
   if (!font) {
      abort_example("data/fixed_font.tga not found.\n");
      return 1;
   }

   redraw();
   run();

   al_destroy_display(display);

   al_destroy_event_queue(queue);

   return 0;
}

