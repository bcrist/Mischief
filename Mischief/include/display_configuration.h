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
// File: display_configuration.h

#ifndef MISCHIEF_DISPLAY_CONFIGURATION_H_
#define MISCHIEF_DISPLAY_CONFIGURATION_H_
#include "mischief_std.h"

#include "sqlite3.h"

MSC_BEGIN

struct DisplayConfiguration
{
   static DisplayConfiguration load(sqlite3 *db);
   static void save(const DisplayConfiguration &dc, sqlite3 *db);
   static const DisplayConfiguration default_configuration;

   enum DisplayMode
   {
      kDISPLAY_WINDOWED = 0,
      kDISPLAY_FULLSCREEN_WINDOWED = 1,
      kDISPLAY_FULLSCREEN = 2
   };

   DisplayConfiguration();
   DisplayConfiguration(DisplayMode mode, int adapter, int refresh_rate, int multisamples, int width, int height);

   DisplayMode display_mode;
   int display_adapter;
   int refresh_rate;
   int multisamples;
   int width;
   int height;

private:
   static void log(const char *msg);
   static void log(const char *msg, sqlite3 *db);
   static void log(const char *msg, sqlite3 *db, const char *sql);
};

MSC_END

#endif
