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
// File: profiler.h

#ifndef MISCHIEF_PROFILER_H_
#define MISCHIEF_PROFILER_H_
#include "mischief_std.h"

#include "profile_node.h"

MSC_BEGIN

class Profiler
{
public:
   static Profiler *getDefault();

   Profiler();
   ~Profiler();

   ProfileNode *getRootNode();
   ProfileNode *subnode(const std::string &name);

   void clear();
   double getTimeSinceClear() const;

private:
   static Profiler *default_;

   ProfileNode root_;
   double clear_time_;

   DISALLOW_COPY_AND_ASSIGN(Profiler);
};

MSC_END

#endif
