# coolast

An extension to the [compiler-class](https://class.coursera.org/compilers) software that
makes it possible to get a graphical representation of the AST. 

## Dependencies

- graphviz
- compiler-class VM image / toolchain

## Usage (VM specific)

    $ sudo aptitude update && sudo aptitude install graphviz git-core -y
    $ cd ~
    $ mkdir coolast
    $ cd coolast
    $ make -f /usr/class/cs143/assignments/PA4/Makefile > /dev/null
    $ git clone git://github.com/fbs/coolast.git
    $ cp coolast/* .
    $ nano Makefile
        add dotdump.cc to the CSRC list.
    $ nano semant-phase.cc
      	   Add the following to main:
 	   ofstream dotfile;
	   dotfile.open("ast.dot");
	   ast_root->dump_to_dot(dotfile, 0);
    $ make semant
    $ ./mysemant good.cl
    $ dot -Tpng ast.dot > ast.png
    $ midori ast.png

    
	

