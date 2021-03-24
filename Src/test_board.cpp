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
  std::vector<Case> ret=g2.dijkstra(start,end);
  std::cout<<std::endl;
  for(auto& r: ret){
    std::cout<<r.parent.location.column<<r.parent.location.line<<(int) r.parent.status<<"p"<<r.poids<<std::endl;
  }
  std::cout<<start.location.column<<(int) start.status;
  return 0 ;
}