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
#include <vector>
#include <cmath>


#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#endif

#include "stb_image.h"

namespace ogl3test {

const GLfloat App::vbo_data_[] = {
   -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
   -1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
   -1.0f,  1.0f, 0.0f
};

const GLfloat App::vbo_uv_data_[] = {
   0.0f, 1.0f,
   1.0f, 1.0f,
   1.0f, 0.0f,
   0.0f, 1.0f,
   1.0f, 0.0f,
   0.0f, 0.0f
};

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
   glDeleteProgram(prog_id_);
   glDeleteTextures(1, &tex_id_);
   glDeleteBuffers(2, &vbo_id_);
   glDeleteVertexArrays(1, &vao_id_);

   al_destroy_event_queue(queue_);
   al_destroy_display(display_);
}

bool App::initOpenGL()
{
   al_set_new_window_position(600, 200);

   al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_OPENGL_3_0 | ALLEGRO_OPENGL_FORWARD_COMPATIBLE | ALLEGRO_WINDOWED);

   al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
   al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);

   display_ = al_create_display(600, 600);
   if (!display_)
   {
      al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 0, ALLEGRO_DONTCARE);
      al_set_new_display_option(ALLEGRO_SAMPLES, 0, ALLEGRO_DONTCARE);

      display_ = al_create_display(600, 600);
      if (!display_)
         return false;
   }

   glClearColor(0, 0, 0, 0);
   
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Set up VAO
   glGenVertexArrays(1, &vao_id_);
   glBindVertexArray(vao_id_);

   // Set up VBO
   glGenBuffers(2, &vbo_id_);
   glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
   glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_data_), vbo_data_, GL_STATIC_DRAW);

   glBindBuffer(GL_ARRAY_BUFFER, vbo_id_ + 1); // uv buffer
   glBufferData(GL_ARRAY_BUFFER, sizeof(vbo_uv_data_), vbo_uv_data_, GL_STATIC_DRAW);

   // Set up GLSL program
   GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
   GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

   std::ostringstream vs_oss;
   std::ostringstream fs_oss;

   // vertex shader
   vs_oss << "#version 330 core" << std::endl;
   vs_oss << "layout(location = 0) in vec3 vert_pos_model;" << std::endl;
   vs_oss << "layout(location = 1) in vec2 vert_uv;" << std::endl;

   vs_oss << "out vec2 uv;" << std::endl;
   vs_oss << "out vec3 pos;" << std::endl;

   vs_oss << "uniform mat4 mvp;" << std::endl;

   vs_oss << "void main() {" << std::endl;
   vs_oss << "   gl_Position = mvp * vec4(vert_pos_model, 1);" << std::endl;
   vs_oss << "   pos = vert_pos_model;" << std::endl;
   vs_oss << "   uv = vert_uv;" << std::endl;
   vs_oss << "}" << std::endl;

   // fragment shader
   fs_oss << "#version 330 core" << std::endl;

   fs_oss << "in vec2 uv;" << std::endl;
   fs_oss << "in vec3 pos;" << std::endl;

   fs_oss << "out vec3 color;" << std::endl;

   fs_oss << "uniform vec3 ambient_color;" << std::endl;
   fs_oss << "uniform vec3 light_color;" << std::endl;
   fs_oss << "uniform vec3 light_pos;" << std::endl;
   fs_oss << "uniform vec2 light_range;" << std::endl;
   fs_oss << "uniform sampler2D tex_sampler;" << std::endl;

   fs_oss << "void main() {" << std::endl;
   fs_oss << "   float len = length(light_pos - pos);" << std::endl;
   fs_oss << "   len = 1.0 / (len * len);" << std::endl;
   //fs_oss << "   len = smoothstep(light_range.x, light_range.y, len);" << std::endl;
   //fs_oss << "   len = clamp((len - light_range.x) / (light_range.y - light_range.x), 0.0, 1.0);" << std::endl;
   //fs_oss << "   len = len * len * len * (3.0 - 2.0 * len);" << std::endl;
   fs_oss << "   vec3 tex_color = texture(tex_sampler, uv).rgb;" << std::endl;
   fs_oss << "   color = ambient_color * tex_color + len * light_color * tex_color;" << std::endl;
   fs_oss << "}" << std::endl;

   GLint result = GL_FALSE;
   int infolog_len;
   std::string shader_code;
   const GLchar *c_shader_code;

   // compile vertex shader
   shader_code = vs_oss.str();
   c_shader_code = shader_code.c_str();
   log_.log("Vertex shader:\n");
   log_.log(c_shader_code);
   glShaderSource(vert_shader_id, 1, &c_shader_code, NULL);
   glCompileShader(vert_shader_id);

   glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
   glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &infolog_len);
   char *vert_shader_err_msg = new char[std::max(1, infolog_len)];
   glGetShaderInfoLog(vert_shader_id, infolog_len, NULL, vert_shader_err_msg);
   log_.log(vert_shader_err_msg);
   log_.log("\n");
   delete[] vert_shader_err_msg;

   // compile fragment shader
   shader_code = fs_oss.str();
   c_shader_code = shader_code.c_str();
   log_.log("Fragment shader:\n");
   log_.log(c_shader_code);
   glShaderSource(frag_shader_id, 1, &c_shader_code, NULL);
   glCompileShader(frag_shader_id);

   glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
   glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &infolog_len);
   char *frag_shader_err_msg = new char[std::max(1, infolog_len)];
   glGetShaderInfoLog(frag_shader_id, infolog_len, NULL, frag_shader_err_msg);
   log_.log(frag_shader_err_msg);
   log_.log("\n");
   delete[] frag_shader_err_msg;

   // Link GLSL program
   prog_id_ = glCreateProgram();
   glAttachShader(prog_id_, vert_shader_id);
   glAttachShader(prog_id_, frag_shader_id);
   glLinkProgram(prog_id_);
 
   glGetProgramiv(prog_id_, GL_LINK_STATUS, &result);
   glGetProgramiv(prog_id_, GL_INFO_LOG_LENGTH, &infolog_len);
   char *prog_err_msg = new char[std::max(1, infolog_len)];
   glGetProgramInfoLog(prog_id_, infolog_len, NULL, prog_err_msg);
   log_.log(prog_err_msg);
   log_.log("\n");
   delete[] prog_err_msg;

   // delete shader objects
   glDeleteShader(vert_shader_id);
   glDeleteShader(frag_shader_id);

   mvp_ = glm::mat4(1.0f);
   matrix_loc_ = glGetUniformLocation(prog_id_, "mvp");

   ambient_color_ = glm::vec3(0.45f);
   ambient_color_loc_ = glGetUniformLocation(prog_id_, "ambient_color");

   light_color_ = glm::vec3(0.03, 0.09, 0.07);
   light_color_loc_ = glGetUniformLocation(prog_id_, "light_color");

   light_pos_ = glm::vec3(-999, -999, 0.25);
   light_pos_loc_ = glGetUniformLocation(prog_id_, "light_pos");

   light_range_ = glm::vec2(0.75, 0.1);
   light_range_loc_ = glGetUniformLocation(prog_id_, "light_range");

   sampler_loc_ = glGetUniformLocation(prog_id_, "tex_sampler");

   // Load texture
   int w, h, n;
   unsigned char *data = stbi_load("copper.png", &w, &h, &n, 4);
   glGenTextures(1, &tex_id_);
   glBindTexture(GL_TEXTURE_2D, tex_id_);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
 
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   stbi_image_free(data);

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

            case ALLEGRO_EVENT_KEY_DOWN:
               if (ae.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                  return 0;
               break;

            case ALLEGRO_EVENT_MOUSE_LEAVE_DISPLAY:
               light_pos_.x = -999.0f;
               light_pos_.y = -999.0f;
               break;

            case ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY:
            case ALLEGRO_EVENT_MOUSE_AXES:
            case ALLEGRO_EVENT_MOUSE_WARPED:
               light_pos_.x = (ae.mouse.x / 300.0f) - 1.0f;
               light_pos_.y = 1.0f - (ae.mouse.y / 300.0f);

               break;
         }
      } while (al_get_next_event(queue_, &ae));

      glClear(GL_COLOR_BUFFER_BIT);
      
      glUseProgram(prog_id_);

      glUniformMatrix4fv(matrix_loc_, 1, GL_FALSE, &mvp_[0][0]);

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, tex_id_);
      glUniform1i(sampler_loc_, 0);

      glUniform3fv(ambient_color_loc_, 3, &ambient_color_.x);
      glUniform3fv(light_color_loc_, 3, &light_color_.x);
      glUniform3fv(light_pos_loc_, 3, &light_pos_.x);
      glUniform2fv(light_range_loc_, 2, &light_range_.x);


      glEnableVertexAttribArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

      glEnableVertexAttribArray(1);
      glBindBuffer(GL_ARRAY_BUFFER, vbo_id_ + 1);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
      

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      glDisableVertexAttribArray(0);
      glDisableVertexAttribArray(1);

      al_flip_display();
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
