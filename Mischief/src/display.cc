// Copyright (c) 2012 Project Mischief
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Author: Benjamin Crist
// File: display.cc

#include "display.h"

#include "logger.h"
#include "display_configuration.h"

MSC_BEGIN

Display *Display::createDisplay(ALLEGRO_EVENT_QUEUE *al_queue, const DisplayConfiguration &conf)
{
   al_set_new_display_adapter(conf.display_adapter);
   al_set_new_display_refresh_rate(conf.refresh_rate);

   int flags(0);

   flags |= ALLEGRO_OPENGL;
   //flags |= ALLEGRO_OPENGL_3_0;
   //flags |= ALLEGRO_OPENGL_FORWARD_COMPATIBLE;

   switch (conf.display_mode)
   {
      case DisplayConfiguration::kDISPLAY_FULLSCREEN:
         flags |= ALLEGRO_FULLSCREEN;
         break;

      case DisplayConfiguration::kDISPLAY_FULLSCREEN_WINDOWED:
         flags |= ALLEGRO_FULLSCREEN_WINDOW;
         break;

      default:
         flags |= ALLEGRO_WINDOWED;
   }

   al_set_new_display_flags(flags);

   if (conf.multisamples > 1)
   {
      al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
      al_set_new_display_option(ALLEGRO_SAMPLES, conf.multisamples, ALLEGRO_REQUIRE);
   }
   else
   {
      al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 0, ALLEGRO_DONTCARE);
      al_set_new_display_option(ALLEGRO_SAMPLES, 0, ALLEGRO_DONTCARE);
   }
   
   if (conf.display_mode == DisplayConfiguration::kDISPLAY_WINDOWED)
   {
      ALLEGRO_MONITOR_INFO m_info;
      if (al_get_monitor_info(conf.display_adapter, &m_info))
      {
         int x = 0.5 * (m_info.x1 + m_info.x2 - conf.width);
         int y = 0.5 * (m_info.y1 + m_info.y2 - conf.height);
         al_set_new_window_position(x, y);
      }
      else
      {
         Logger::appendLine("Display: could not retrieve monitor info.");
      }
   }

   ALLEGRO_DISPLAY *al_display = al_create_display(conf.width, conf.height);
   if (!al_display)
      return NULL;

   glClearColor(0, 0, 0, 0);
   
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   al_register_event_source(al_queue, al_get_display_event_source(al_display));
   return new Display(al_display);
}

Display::Display(ALLEGRO_DISPLAY *al_display)
{
   al_display_ = al_display;
}

Display::~Display()
{
   al_destroy_display(al_display_);
}

MSC_END
