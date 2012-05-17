#include "cool.h"
#include "tree.h"
#include "cool-tree.h"
#include "utilities.h"

#define PROGRAM_NODE "program"

static int classname = 0;
static int featurename = 1000;
static int formalname = 2000;
static int branchname = 3000;
static int exprname = 4000;

int exprcaller;

void Expression_class::dump_to_dot(std::ofstream& f, int n) {

  f << exprcaller << "->" << exprname << ";" << std::endl; 
// if (type) {
  f << exprname << " [label=\" notype \"];" << std::endl;
  // }
  // else
  //   {
  //     f << exprname << " [label=\"type: " << type << "\"];" << std::endl;
  //   }
  exprname++;
}

void program_class::dump_to_dot(std::ofstream& f, int n) {
  f << "digraph AST {" << std::endl;
  for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
     classes->nth(i)->dump_to_dot(f, i);
     classname++;
  }
  f << "}" << std::endl;
}

/* n is the class index */
void class__class::dump_to_dot(std::ofstream& f, int n) {
  // program->classIndex
  f << "\t" << PROGRAM_NODE << "->" << classname << ";" << std::endl;
  f << "\t" << classname << " [label= \"" << name << "\"];" << std::endl;

  for(int i = features->first(); features->more(i); i = features->next(i)) {
    features->nth(i)->dump_to_dot(f, i);
    featurename++;
  }
}

void method_class::dump_to_dot(std::ofstream& f, int n) {
  f << "\t" << classname << "->" << featurename << ";" << std::endl;
  f << "\t" << featurename << " [label= \" method: " << name << ":" << return_type << "\"];" << std::endl;
  f << "\t" << featurename << "->" << formalname << ";" << std::endl;
  f << "\t" << formalname << " [label= \" params: " ;
  for(int i = formals->first(); formals->more(i); i = formals->next(i)) {
    formals->nth(i)->dump_to_dot(f, i);
  }
  f << "\"];" << std::endl;
  formalname++;
  exprcaller = featurename;
  expr->dump_to_dot(f,-1);
}

void attr_class::dump_to_dot(std::ofstream& f, int n) {
  f << "\t" << classname << "->" << featurename << ";" << std::endl;
  f << "\t" << featurename << " [label= \" attr: " << name << ":" << type_decl << "\"];" << std::endl;
  exprcaller = featurename;
  init->dump_to_dot(f,-1);
}

void formal_class::dump_to_dot(std::ofstream& f, int n) {
  f << name << ":" << type_decl << ", ";
}

void block_class::dump_to_dot(std::ofstream& f, int n) {
  int tmp = exprcaller;
  f << "\t" << exprcaller << "->" << exprname << ":" << std::endl;
  f << "\t" << exprcaller << " [label= \" block\"];" << std::endl;
  exprcaller = exprname;
  exprname++;
  for(int i = body->first(); body->more(i); i = body->next(i))
     body->nth(i)->dump_to_dot(f, -1);
  exprcaller = tmp;
}

void branch_class::dump_to_dot(std::ofstream& f, int n) {
  ;
}

void assign_class::dump_to_dot(std::ofstream& f, int n) {
  int tmp = exprcaller;
  f << "\t" << exprcaller << "->" << exprname << ";" << std::endl;
  f << "\t" << exprname << " [label= \" assign: " << name << "\"];" << std::endl;
  exprcaller = exprname;
  expr->dump_to_dot(f,n);
  exprcaller = tmp;
  exprname++;
}

void no_expr_class::dump_to_dot(std::ofstream& f, int n) {
  f << "\t" << exprcaller << "->" << exprname << ";" << std::endl;
  f << "\t" << exprname << " [label= \" no_expr\"];" << std::endl;
}
