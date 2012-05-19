/*
Copyright (c) 2012 bas smit (fbs)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
Copyright (c) 1995,1996 The Regents of the University of California.
All rights reserved.

Permission to use, copy, modify, and distribute this software for any
purpose, without fee, and without written agreement is hereby granted,
provided that the above copyright notice and the following two
paragraphs appear in all copies of this software.

IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
*/

#ifndef _dot_dump_h_
#define _dot_dump_h_

#include <iostream>
#include <sstream>
#include <string>

class dot_node;
class dot_edge;
class dot_attribute;

typedef int node_id_t;

node_id_t generate_node_id(void);

class dot_attribute {
 private:
  std::string	_label;
  std::string        _str;
 public:
  dot_attribute();
  void set_label(const char *);
  void set_label(Symbol);
  void append_label(const char *);
  void append_label(Symbol);
  const char * to_string(); 
};

class dot_node: public dot_attribute {
private:
  node_id_t _nodeid;
public:
  dot_node(node_id_t);
  node_id_t get_id();
  
  friend std::ostream& operator<<(std::ostream&, dot_node);
};

class dot_edge: public dot_attribute {
 private:
  node_id_t _parent;
  node_id_t _child;
 public:
  dot_edge(node_id_t, node_id_t);
  dot_edge(dot_node, dot_node);
  dot_edge(node_id_t, dot_node);
  node_id_t get_parent();
  node_id_t get_child();
  void set_parent(node_id_t);

  friend std::ostream& operator<<(std::ostream&, dot_edge);
};

#endif
