#include "board.hpp"

#include <iostream>

int main() {
  RR::Board b("board.txt") ;
  b.save("/tmp/cpy.txt") ;

  //RR::Graph g(2, 2, "board.txt");
  //g.afficher();

  RR::Graph g2(2, 2, b);
  g2.afficher();

  return 0 ;
}
