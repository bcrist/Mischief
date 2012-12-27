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
// File: profile_node.h

#ifndef MISCHIEF_PROFILE_NODE_H_
#define MISCHIEF_PROFILE_NODE_H_
#include "mischief_std.h"

#include <string>
#include <vector>

MSC_BEGIN

class Profiler;

class ProfileNode
{
   friend class Profiler;

public:
   const std::string &getName() const;

   ProfileNode *getParentNode();
   ProfileNode *subnode(const std::string &name);

   void clear();
   void start();
   void stop();
   double getTime() const;

private:
   ProfileNode(ProfileNode *parent, const std::string &name);
   ~ProfileNode();


   std::string name_;

   ProfileNode *parent_;
   std::vector<ProfileNode*> children_;

   double time_accum_;
   double start_time_;


   DISALLOW_COPY_AND_ASSIGN(ProfileNode);
};

MSC_END

#endif
