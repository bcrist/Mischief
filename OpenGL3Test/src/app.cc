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
// File: app.cc

#include "app.h"

#include <sstream>

#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#endif

namespace ogl3test {

App::App()
   : init_successful_(true),
   display_(NULL),
   queue_(NULL)
{
   al_set_org_name("Project Mischief");
   al_set_app_name("OpenGL3 Test");

   if (!al_init())
   {
      startupError("The Allegro engine failed to initialize properly!");
      return;
   }

   if (!al_install_keyboard())
      log_.log("Failed to install keyboard!\n");

   if (!al_install_mouse())
      log_.log("Failed to install mouse!\n");

   if (!initOpenGL())
   {
      startupError("There was a problem creating the OpenGL context!");
      return;
   }

   queue_ = al_create_event_queue();
   if (!queue_)
   {
      startupError("There was a problem creating the event queue!");
      return;
   }

   al_register_event_source(queue_, al_get_keyboard_event_source());
   al_register_event_source(queue_, al_get_mouse_event_source());
   al_register_event_source(queue_, al_get_display_event_source(display_));
}

App::~App()
{
   al_destroy_event_queue(queue_);
   al_destroy_display(display_);
}

bool App::initOpenGL()
{
   al_set_new_window_position(640, 250);

   al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 | ALLEGRO_WINDOWED | ALLEGRO_RESIZABLE);

   al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);

   display_ = al_create_display(640, 480);
   if (!display_)
   {
      al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 0, ALLEGRO_DONTCARE);
      al_set_new_display_option(ALLEGRO_SAMPLES, 0, ALLEGRO_DONTCARE);

      display_ = al_create_display(640, 480);
      if (!display_)
         return false;
   }

   glClearColor(0, 0, 0, 0);
   
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   return true;
}

void App::startupError(const char *msg)
{
   al_show_native_message_box(NULL, "Startup Error", "Error", msg, NULL, ALLEGRO_MESSAGEBOX_ERROR);
   init_successful_ = false;
}

int App::run()
{
    if (!init_successful_)
      return -1;

   while (true) {
      ALLEGRO_EVENT ae;
      bool windowResized = false;
      al_wait_for_event(queue_, &ae);
      do
      {
         switch (ae.type)
         {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
               return 0;

            case ALLEGRO_EVENT_DISPLAY_RESIZE:
               windowResized = true;
               break;

//            case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
//            case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:

            case ALLEGRO_EVENT_KEY_DOWN:
               if (ae.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                  return 0;
               break;

//            case ALLEGRO_EVENT_KEY_UP:
//            case ALLEGRO_EVENT_KEY_CHAR:

            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
               //ui_->uiMouseMove(Point(-9999, -9999));
               break;

            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
               //ui_->uiMouseMove(Point(ae.mouse.x + 0.5f, ae.mouse.y + 0.5f));
               if (ae.mouse.dz != 0 || ae.mouse.dw != 0)
                  //ui_->uiMouseWheel(ae.mouse.dz, ae.mouse.dw);
               break;

//            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
//            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
         }
      } while (al_get_next_event(queue_, &ae));


      if (windowResized)
         al_acknowledge_resize(display_);

      //if (ui_->uiDrawRequested())
      //{
         glClear(GL_COLOR_BUFFER_BIT);
         //ui_->makeDirty(ui_->getAbsoluteRect());
         //ui_->uiDraw();
         al_flip_display();
      //}
   }

   return 0;
}



}

#ifdef WIN32

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   ogl3test::App a;
   return a.run();
}

#else

int main(int argc, char** argv)
{
   ogl3test::App a;
   return a.run();
}

#endif
