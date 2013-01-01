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
// File: display_configuration.cc

#include "display_configuration.h"

#include <sstream>
#include <allegro5/allegro.h>

#include "Logger.h"

MSC_BEGIN

const DisplayConfiguration DisplayConfiguration::default_configuration = DisplayConfiguration();

DisplayConfiguration DisplayConfiguration::load(sqlite3 *db)
{
   DisplayConfiguration dc(default_configuration);

   

   
         sql = "SELECT mode, adapter, refresh_rate, "
               "multisamples, width, height FROM m_conf_display "
               "ORDER BY _rowid_ DESC LIMIT 1";

         if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
         {
            int result = sqlite3_step(stmt);
            if (result == SQLITE_ROW)
            {
               switch (sqlite3_column_int(stmt, 0))
               {
                  case (int)kDISPLAY_FULLSCREEN_WINDOWED:
                     dc.display_mode = kDISPLAY_FULLSCREEN;
                     break;

                  case (int)kDISPLAY_FULLSCREEN:
                     dc.display_mode = kDISPLAY_FULLSCREEN;
                     break;

                  default:
                     dc.display_mode = kDISPLAY_WINDOWED;
               }

               dc.display_adapter = sqlite3_column_int(stmt, 1);
               dc.refresh_rate = sqlite3_column_int(stmt, 2);
               dc.multisamples = sqlite3_column_int(stmt, 3);
               dc.width = sqlite3_column_int(stmt, 4);
               dc.height = sqlite3_column_int(stmt, 5);
      
               log("Successfully loaded configuration from database.");
            }
            else if (result == SQLITE_DONE)
               log("No configuration found in database; using defaults.");
            else
               log("Failed to load configuration from database", db);

            if (sqlite3_finalize(stmt) != SQLITE_OK)
               log("Error finalizing statement", db);
         }
         else  // sqlite3_prepare_v2() != SQLITE_OK
            log("Could not prepare statement", db, sql);

      }
      else if (result != SQLITE_DONE)
         log("Error executing statement", db, sql);
   }
   else
      log("Could not prepare statement", db, sql);

   return dc;
}

void DisplayConfiguration::save(const DisplayConfiguration &dc, sqlite3 *db)
{
   const char *sql = "CREATE TABLE IF NOT EXISTS m_conf_display ("
                     "mode INTEGER,"
                     "adapter INTEGER,"
                     "refresh_rate INTEGER,"
                     "multisamples INTEGER,"
                     "width INTEGER,"
                     "height INTEGER)";

   if (sqlite3_exec(db, sql, NULL, NULL, NULL) == SQLITE_OK)
   {
      sqlite3_stmt *del_stmt;
      const char *delsql = "DELETE FROM m_conf_display";
      if (sqlite3_prepare_v2(db, delsql, -1, &del_stmt, NULL) == SQLITE_OK)
      {
         sqlite3_stmt *ins_stmt;
         sql = "INSERT INTO m_conf_display "
               "(mode, adapter, refresh_rate, multisamples, width, height)"
               " VALUES (?, ?, ?, ?, ?, ?)";
         if (sqlite3_prepare_v2(db, sql, -1, &ins_stmt, NULL) == SQLITE_OK)
         {
            if (sqlite3_bind_int(ins_stmt, 1, dc.display_mode) != SQLITE_OK ||
                sqlite3_bind_int(ins_stmt, 2, dc.display_adapter) != SQLITE_OK ||
                sqlite3_bind_int(ins_stmt, 3, dc.refresh_rate) != SQLITE_OK ||
                sqlite3_bind_int(ins_stmt, 4, dc.multisamples) != SQLITE_OK ||
                sqlite3_bind_int(ins_stmt, 5, dc.width) != SQLITE_OK ||
                sqlite3_bind_int(ins_stmt, 6, dc.height) != SQLITE_OK)
            {
               log("Error binding statement parameters", db, sql);
            }
            else
            {
               bool success(true);

               if (sqlite3_step(del_stmt) != SQLITE_DONE)
               {
                  log("Error executing statement", db, delsql);
                  success = false;
               }
               else if (sqlite3_step(ins_stmt) != SQLITE_DONE)
               {
                  log("Error executing statement", db, sql);
                  success = false;
               }

               if (success)
                  log("Successfully saved configuration to database.");
               else
               {
                  if (sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL) != SQLITE_OK)
                     log("Error rolling back", db, "ROLLBACK");
               }
            }

            if (sqlite3_finalize(ins_stmt) != SQLITE_OK)
               log("Error finalizing statement", db, sql);
         }
         else
            log("Error preparing statement", db, sql);

         if (sqlite3_finalize(del_stmt) != SQLITE_OK)
            log("Error finalizing statement", db, delsql);
      }
      else  // del_stmt
         log("Error preparing statement", db, delsql);
   }
   else  // sqlite3_exec() != SQLITE_OK
      log("Failed to create table m_conf_display", db, sql);
}

DisplayConfiguration::DisplayConfiguration()
      : display_mode(kDISPLAY_WINDOWED),
        display_adapter(ALLEGRO_DEFAULT_DISPLAY_ADAPTER),
        refresh_rate(-1),
        multisamples(1),
        width(640),
        height(480)
{
}

DisplayConfiguration::DisplayConfiguration(DisplayMode mode, int adapter, int refresh_rate, int multisamples, int width, int height)
      : display_mode(mode),
        display_adapter(adapter),
        refresh_rate(refresh_rate),
        multisamples(multisamples),
        width(width),
        height(height)
{
}

bool DisplayConfiguration::tableExists(sqlite3 *db)
{
   bool retval = 
   const char *sql = "SELECT 1 FROM sqlite_master WHERE type='table' AND name='m_conf_display'";

   sqlite3_stmt *stmt;
   if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK)
   {
      int result = sqlite3_step(stmt);
      if (result == SQLITE_ROW)
      {
         if (sqlite3_finalize(stmt) != SQLITE_OK)
            log("Error finalizing statement", db, sql);
      }
   }

   return retval;
}

void DisplayConfiguration::log(const char *msg)
{
   Logger::appendLine("Configuration: ", msg);
}

void DisplayConfiguration::log(const char *msg, sqlite3 *db)
{
   log(msg, db, NULL);
}

void DisplayConfiguration::log(const char *msg, sqlite3 *db, const char *sql)
{
   std::ostringstream oss;

   oss << "DisplayConfiguration: " << msg << ": (" << sqlite3_errcode(db) << ") ";
   oss << sqlite3_errmsg(db) << std::endl;

   if (sql)
      oss << sql << std::endl;

   Logger::appendLine(oss.str().c_str());
}

MSC_END
