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
// File: game.cc

#include "game.h"

#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#endif

#include <allegro5/allegro_native_dialog.h>
#include <sstream>
#include "logger.h"
#include "configuration.h"

MSC_BEGIN

Game::Game()
      : init_successful_(true),
        al_queue_(NULL)
{
   al_set_org_name("Project Mischief");
   al_set_app_name("Mischief");

   if (!al_init())
   {
      startupError("The Allegro engine failed to initialize properly!");
      return;
   }

   if (!al_install_keyboard())
      Logger::appendLine("Failed to install keyboard!");

   if (!al_install_mouse())
      Logger::appendLine("Failed to install mouse!");

   al_queue_ = al_create_event_queue();
   if (!al_queue_)
   {
      startupError("There was a problem creating the event queue!");
      return;
   }

   al_register_event_source(al_queue_, al_get_keyboard_event_source());
   al_register_event_source(al_queue_, al_get_mouse_event_source());

   
   

   Configuration config = Configuration::load("./mischief_conf.mdb");

   

   display_ = Display::createDisplay(al_queue_, config.display);
   if (!display_)
   {
      display_ = Display::createDisplay(al_queue_, DisplayConfiguration::default_configuration);
      if (!display_)
      {
         startupError("Failed to create a game window!");
      }
   }
}

Game::~Game()
{
   al_destroy_event_queue(al_queue_);
}

int Game::run()
{
   if (!init_successful_)
      return -1;

   while (true) {
      ALLEGRO_EVENT ae;
      bool windowResized = false;
      al_wait_for_event(al_queue_, &ae);
      do
      {
         switch (ae.type)
         {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
               return 0;

            case ALLEGRO_EVENT_KEY_DOWN:
               if (ae.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                  return 0;
               break;

            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
               break;

            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
               break;
         }
      } while (al_get_next_event(al_queue_, &ae));

      al_flip_display();
   }

   return 0;
}


void Game::startupError(const char *msg)
{
   al_show_native_message_box(NULL, "Startup Error", "Error", msg, NULL, ALLEGRO_MESSAGEBOX_ERROR);
   init_successful_ = false;
}

MSC_END

#ifdef WIN32

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR lpCmdLine,
                       int nCmdShow)
{
   UNREFERENCED_PARAMETER(hInstance);
   UNREFERENCED_PARAMETER(hPrevInstance);
   UNREFERENCED_PARAMETER(lpCmdLine);

   mischief::Game g;
   int retval = g.run();
   mischief::Logger::getDefault()->clean();
   return retval;
}

#else

int main(int argc, char** argv)
{
   mischief::Game g;
   int retval = g.run();
   mischief::Logger::getDefault()->clean();
   return retval;
}

#endif