#include "board.hpp"
#include "graph.hpp"

#include <iostream>

int main() {
  RR::Board b("board.txt") ;
  b.save("/tmp/cpy.txt") ;

  //RR::Graph g(2, 2, "board.txt");
  //g.afficher();

  Graph g2(b);
  g2.afficher();
  RR::Robot start(RR::Location((int)0,(int) 1),(RR::Robot::Status) 3);
  RR::Location end={1, 0};
  std::vector<int> ret=g2.dijkstra(start,end);
  std::cout<<std::endl;
  for(auto& r: ret){
    std::cout<<start.move[r]<<std::endl;
  }
  return 0 ;
}