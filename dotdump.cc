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

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "cool.h"
#include "tree.h"
#include "cool-tree.h"
#include "utilities.h"

#include "dotdump.h"

#define DEBUG

#ifdef DEBUG
#define PRINT_DEBUG(msg)				\
  cerr << msg << endl;
#else
#define PRINT_DEBUG(msg) ;
#endif

using namespace std;

/* Generate a unique node id */
node_id_t
generate_node_id(void) {
  static node_id_t n = 1;
  return n++;
}

dot_attribute::dot_attribute() {
  ;
}

void
dot_attribute::set_label(const char * str) {
  _label = str;
}

void
dot_attribute::set_label(Symbol s) {
  ostringstream oss;
  oss << s;
  _label = oss.str();
}

void
dot_attribute::append_label(const char * str) {
  _label += str;
}

void
dot_attribute::append_label(Symbol s) {
  ostringstream oss;
  oss << s;
  _label += oss.str();
}

const char *
dot_attribute::to_string() {
  if (! (_label.empty())) {
    _str = " [label=\"";
    _str += _label;
    _str += "\"]";
    return _str.c_str();
  }
  return "";
}

dot_node::dot_node(node_id_t id) {
  _nodeid = id;
}

node_id_t
dot_node::get_id() {
  return _nodeid;
}

dot_edge::dot_edge(node_id_t parent, node_id_t child) {
  _parent = parent;
  _child = child;
}

dot_edge::dot_edge(dot_node parent, dot_node child) {
  _parent = parent.get_id();
  _child = child.get_id();
}

dot_edge::dot_edge(node_id_t parent, dot_node child) {
  _parent = parent;
  _child = child.get_id();
}

node_id_t
dot_edge::get_parent() {
  return _parent;
}

node_id_t
dot_edge::get_child() {
  return _child;
}

ostream&
operator<<(ostream& out, dot_node node)
{
  out << "\t" << node.get_id() << node.to_string() << ';' << endl;
  return out;
}

ostream&
operator<<(ostream& out, dot_edge edge)
{
  out << "\t" << edge.get_parent() << "->" << edge.get_child();
  out << edge.to_string() << ';' << endl;
  return out;
}

// Symbol
// formal_class::get_name() {
//   return name;
// }

// Symbol
// formal_class::get_type_decl() {
//   return type_decl;
// }

void
Expression_class::dump_type_dot(ofstream& stream, dot_node * parent)
{
  if (type) {
    parent->append_label(":");
    parent->append_label(type);
  }
}

/* Starting point of the tree iteration
 Format specifies the way the tree is build
 -1: each class in its own diagram;
 0: all classes in one diagram;
*/
void
program_class::dump_to_dot(std::ofstream& stream, int format)
{
  dot_node node(0);
  if (format == -1)
    {
      for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
	stream << "digraph AST" << i << " {\n";
	stream << "\tnode [shape=record];\n";
	stream << "\t0 [shape=diamond,color=red,label=root];\n";
	classes->nth(i)->dump_to_dot(stream, &node);
	stream << "\n}\n";
      }
    }
  else
    {
      stream << "digraph AST {" << endl;
      stream << "\tnode [shape=record];\n";
      stream << "\t0 [shape=diamond,color=red,label=root];\n";
      for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
	classes->nth(i)->dump_to_dot(stream, &node);
      }
      stream << "\n}\n\n";
    }
}

/*
  Make a node with `Classname:Parent` as format.
*/
void
class__class::dump_to_dot(ofstream& stream, dot_node * p)
{
  dot_node node (generate_node_id());
  dot_edge edge (*p, node);

  PRINT_DEBUG("dumping class");
  
  node.set_label(name);
  node.append_label(":");
  node.append_label(parent);
  stream << node;
  stream << edge;
for(int i = features->first(); features->more(i); i = features->next(i))
    features->nth(i)->dump_to_dot(stream, &node);
}

/*
  method -> list_of_formals
 */
void
method_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node method_node (generate_node_id());
  dot_node formal_node (generate_node_id());
  dot_edge pm_edge (*parent, method_node);
  dot_edge mf_edge (method_node, formal_node);

  method_node.set_label("method\\n");
  method_node.append_label(name);
  stream << method_node;
  stream << pm_edge;

  /*
    To prevent a node from getting to wide limit to 5 formals per node.
    If there are more than 5 normals create a second formal node and
    let the first formal point to it.
      i.e. method -> formals_1-5 -> formals_6-10
  */

  if (formals->len()) {
    for(int i = formals->first(); formals->more(i); i = formals->next(i)) {
      if (i > 5) {
	break;
      }
      if (i != 0) {
	formal_node.append_label(" | ");
      }
      formals->nth(i)->dump_to_dot(stream, &formal_node);
    }
  }
  else {
    formal_node.set_label("void");
  }

  stream << formal_node;
  stream << mf_edge;

  expr->dump_to_dot(stream, &method_node);
}

void
attr_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent, node);
  node.set_label(name);
  node.append_label("\\n");
  node.append_label(type_decl);
  stream << node;
  stream << edge;
  init->dump_to_dot(stream, &node);
}

  /* Since we don't want our ast to be filled with formal nodes with
     pretty print 5 formals in the parent node, name on one line and the type on the
     line below it.

     Label will have the following format: "name\ntype | name\ntype .. "
  */
void
formal_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  parent->append_label(name);
  parent->append_label("\\n");
  parent->append_label(type_decl);
}

void
branch_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ;
}

void
assign_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node assign (generate_node_id());
  dot_node left (generate_node_id());
  dot_edge pa_edge (*parent, assign);
  dot_edge al_edge (assign, left);
  
  assign.set_label("assign");
  left.set_label("value: ");
  left.append_label(name);

  stream << assign;
  stream << left;
  stream << pa_edge;
  stream << al_edge;
  
  expr->dump_to_dot(stream, &assign);
}

void
static_dispatch_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ;
}

void
dispatch_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ;
}

void
cond_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("if");
  stream << node << edge;

  pred->dump_to_dot(stream, &node);
  then_exp->dump_to_dot(stream, &node);
  else_exp->dump_to_dot(stream, &node);
}

void
loop_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("while");
  stream << node << edge;

  pred->dump_to_dot(stream, &node);
  body->dump_to_dot(stream, &node);
}

void
typcase_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ;
}

void
block_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent, node);

  PRINT_DEBUG("expr block");
  
  node.set_label("expr block");

  stream << node;
  stream << edge;

  for(int i = body->first(); body->more(i); i = body->next(i)) {
    body->nth(i)->dump_to_dot(stream, &node);
  }
}

void
let_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ;
}

void
plus_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("-");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
sub_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("+");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
mul_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("*");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
divide_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("/");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
neg_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("neg ");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
}

void
lt_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("< ");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
eq_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("= ");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node); 
}

void
leq_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("<= ");
  stream << node << edge;

  e1->dump_to_dot(stream, &node);
  e2->dump_to_dot(stream, &node);
}

void
comp_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("not ");
  stream << node << edge;
  e1->dump_to_dot(stream, &node);
}

void
int_const_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("int ");
  node.append_label(token);

  stream << node << edge;
}

void bool_const_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  if (val)
      node.set_label("bool 1");
  else
      node.set_label("bool 0");

  stream << node << edge;
}

void string_const_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  ostringstream oss;
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  print_escaped_string(oss, token->get_string());
  node.set_label("string ");
  node.append_label(oss.str().c_str());

  stream << node << edge;
}

void new__class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);

  node.set_label("new ");
  node.append_label(type_name);
  dump_type_dot(stream, &node);

  stream << node;
  stream << edge;
}

void
isvoid_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);
  node.set_label("isvoid ");
  dump_type_dot(stream, &node);

  stream << node;
  stream << edge;

  e1->dump_to_dot(stream, &node);
  
}

void no_expr_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);
  node.set_label("no_expr");
  stream << node;
  stream << edge;
}

void
object_class::dump_to_dot(ofstream& stream, dot_node * parent)
{
  dot_node node (generate_node_id());
  dot_edge edge (*parent,node);
  node.set_label("object\\n");
  node.append_label(name);
  stream << node;
  stream << edge;
}

