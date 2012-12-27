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
// File: logger.h

#ifndef MISCHIEF_LOGGER_H_
#define MISCHIEF_LOGGER_H_
#include "mischief_std.h"

#include <allegro5/allegro_native_dialog.h>

MSC_BEGIN

class Logger
{
public:
   static Logger *getDefault();
   static void setDefault(Logger *log);

   static void append(const char *txt);
   static void append(const char *txt1, const char *txt2);
   static void append(const char *txt1, const char *txt2, const char *txt3);
   static void append(const char *txt1, const char *txt2, const char *txt3, const char *txt4);

   static void appendLine();
   static void appendLine(const char *txt);
   static void appendLine(const char *txt1, const char *txt2);
   static void appendLine(const char *txt1, const char *txt2, const char *txt3);
   static void appendLine(const char *txt1, const char *txt2, const char *txt3, const char *txt4);

   Logger();
   virtual ~Logger();

   virtual void log(const char *txt);
   virtual void clean();

private:
   static Logger default_log_;
   static Logger *default_log__;
   ALLEGRO_TEXTLOG *al_log_;
};

MSC_END

#endif
