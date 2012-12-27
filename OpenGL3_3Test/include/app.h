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
// File: app.h

#ifndef OGL3TEST_APP_H_
#define OGL3TEST_APP_H_

#define _USE_MATH_DEFINES

#ifdef WIN32
#define ALLEGRO_NO_MAGIC_MAIN
#endif

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "allegro-5.0.8-monolith-md-debug.lib")
#include <allegro5/allegro.h>
#include <allegro5/allegro_opengl.h>

#include "glm/glm.hpp"

#include "logger.h"

namespace ogl3test {

class App
{
public:
   Logger log_;

   App();
   ~App();

   int run();

private:
   static const GLfloat vbo_data_[18];
   static const GLfloat vbo_uv_data_[12];

   bool init_successful_;

   GLuint vao_id_;
   GLuint vbo_id_;
   GLuint tex_id_;
   GLuint prog_id_;

   GLuint matrix_loc_;

   GLuint ambient_color_loc_;
   GLuint light_color_loc_;
   GLuint light_pos_loc_;
   GLuint light_range_loc_;
   GLuint sampler_loc_;

   glm::mat4 mvp_;


   glm::vec3 ambient_color_;
   glm::vec3 light_color_;
   glm::vec3 light_pos_;
   glm::vec2 light_range_;

   ALLEGRO_DISPLAY *display_;
   ALLEGRO_EVENT_QUEUE *queue_;
	
   bool initOpenGL();
   void startupError(const char *msg);
};

}

#endif
