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
// File: profiler.cc

#include "profiler.h"

#include <allegro5/allegro.h>

MSC_BEGIN

Profiler *Profiler::default_ = new Profiler();

Profiler *Profiler::getDefault()
{
   return default_;
}

Profiler::Profiler()
      : root_(NULL, "Profile"),
        clear_time_(0.0)
{
}

Profiler::~Profiler()
{
}

ProfileNode *Profiler::getRootNode()
{
   return &root_;
}

ProfileNode *Profiler::subnode(const std::string &name)
{
   return root_.subnode(name);
}

void Profiler::clear()
{
   root_.clear();
   clear_time_ = al_get_time();
}

double Profiler::getTimeSinceClear() const
{
   return al_get_time() - clear_time_;
}

MSC_END
