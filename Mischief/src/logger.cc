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
// File: logger.cc

#include "logger.h"

MSC_BEGIN

Logger Logger::default_log_ = Logger();
Logger *Logger::default_log__ = &Logger::default_log_;

Logger *Logger::getDefault()
{
   return default_log__;
}

void Logger::setDefault(Logger *log)
{
   default_log__ = log;
}

void Logger::append(const char *txt)
{
   default_log__->log(txt);
}

void Logger::append(const char *txt1, const char *txt2)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
}

void Logger::append(const char *txt1, const char *txt2, const char *txt3)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
   default_log__->log(txt3);
}

void Logger::append(const char *txt1, const char *txt2, const char *txt3, const char *txt4)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
   default_log__->log(txt3);
   default_log__->log(txt4);
}

void Logger::appendLine()
{
   default_log__->log("\n");
}

void Logger::appendLine(const char *txt)
{
   default_log__->log(txt);
   default_log__->log("\n");
}

void Logger::appendLine(const char *txt1, const char *txt2)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
   default_log__->log("\n");
}

void Logger::appendLine(const char *txt1, const char *txt2, const char *txt3)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
   default_log__->log(txt3);
   default_log__->log("\n");
}

void Logger::appendLine(const char *txt1, const char *txt2, const char *txt3, const char *txt4)
{
   default_log__->log(txt1);
   default_log__->log(txt2);
   default_log__->log(txt3);
   default_log__->log(txt4);
   default_log__->log("\n");
}

Logger::Logger()
      : al_log_(NULL)
{
}

Logger::~Logger()
{
   if (al_log_)
      clean();
}

void Logger::log(const char *txt)
{
   if (!al_log_)
      al_log_ = al_open_native_text_log("Log Messages", ALLEGRO_TEXTLOG_NO_CLOSE | ALLEGRO_TEXTLOG_MONOSPACE);

   al_append_native_text_log(al_log_, txt);
}

void Logger::clean()
{
   if (al_log_)
   {
      al_close_native_text_log(al_log_);
      al_log_ = NULL;
   }
}

MSC_END
