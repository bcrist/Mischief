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
// File: configuration.cc

#include "configuration.h"

#include <sstream>

#include "logger.h"

MSC_BEGIN

Configuration Configuration::load(const std::string &filename)
{
   Configuration cfg;

   sqlite3 *db;
   if (sqlite3_open(filename.c_str(),  &db) == SQLITE_OK)
   {
      cfg.display = DisplayConfiguration::load(db);

   }
   else // sqlite3_open() != SQLITE_OK
   {
      log("Failed to open database", db, NULL);
      sqlite3_close(db);
   }

   return cfg;
}

void Configuration::save(const Configuration &cfg, const std::string &filename)
{
   sqlite3 *db;
   if (sqlite3_open(filename.c_str(),  &db) == SQLITE_OK)
   {
      DisplayConfiguration::save(cfg.display, db);

   }
   else // sqlite3_open() != SQLITE_OK
   {
      log("Failed to open database", db, NULL);
      sqlite3_close(db);
   }
}

void Configuration::log(const char *msg)
{
   Logger::appendLine("Configuration: ", msg);
}

void Configuration::log(const char *msg, sqlite3 *db)
{
   log(msg, db, NULL);
}

void Configuration::log(const char *msg, sqlite3 *db, const char *sql)
{
   std::ostringstream oss;

   oss << "Configuration: " << msg << ": (" << sqlite3_errcode(db) << ") ";
   oss << sqlite3_errmsg(db) << std::endl;

   if (sql)
      oss << sql << std::endl;

   Logger::appendLine(oss.str().c_str());
}

MSC_END
