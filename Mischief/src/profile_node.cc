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
// File: profile_node.cc

#include "profile_node.h"

#include <allegro5/allegro.h>

MSC_BEGIN

const std::string &ProfileNode::getName() const
{
   return name_;
}

ProfileNode *ProfileNode::getParentNode()
{
   return parent_;
}

ProfileNode *ProfileNode::subnode(const std::string &name)
{
   ProfileNode *node(NULL);

   for (std::vector<ProfileNode*>::iterator it = children_.begin(); it != children_.end(); ++it)
      if ((*it)->getName() == name)
         node = *it;

   if (!node)
   {
      node = new ProfileNode(this, name);
      children_.push_back(node);
   }

   return node;
}

void ProfileNode::clear()
{
   time_accum_ = 0.0;
   start_time_ = -1.0;

   for (std::vector<ProfileNode*>::iterator it = children_.begin(); it != children_.end(); ++it)
      (*it)->clear();
}

void ProfileNode::start()
{
   if (start_time_ < 0)
      start_time_ = al_get_time();
}

void ProfileNode::stop()
{
   if (start_time_ >= 0)
      time_accum_ += al_get_time() - start_time_;
}

double ProfileNode::getTime() const
{
   return time_accum_ + (start_time_ < 0 ? 0 : al_get_time() - start_time_);
}

ProfileNode::ProfileNode(ProfileNode *parent, const std::string &name)
      : name_(name),
        parent_(parent),
        time_accum_(0.0),
        start_time_(-1.0)
{
}

ProfileNode::~ProfileNode()
{
   for (std::vector<ProfileNode*>::iterator it = children_.begin(); it != children_.end(); ++it)
      delete *it;
}

MSC_END
