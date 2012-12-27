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
// File: game.h

#ifndef MISCHIEF_GAME_H_
#define MISCHIEF_GAME_H_
#include "mischief_std.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "allegro-5.0.8-monolith-md-debug.lib")
#include <allegro5/allegro.h>

#include "display.h"

MSC_BEGIN

class Game
{
public:
   Game();
   ~Game();

   int run();

private:
   ALLEGRO_EVENT_QUEUE *al_queue_;
   Display *display_;

   bool init_successful_;

   void startupError(const char *msg);

   DISALLOW_COPY_AND_ASSIGN(Game);
};

MSC_END

#endif
