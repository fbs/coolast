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