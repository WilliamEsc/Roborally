#include "board.hpp"
#include "graph.hpp"
#include "joueurArt.cpp"
#include "automata.cpp"
#include <iostream>

int main()
{
  RR::Board b("board.txt");
  b.save("/tmp/cpy.txt");
  srand(time(NULL));

  RR::Board big("big_board.txt");
  big.save("/tmp/cpy2.txt");

  //RR::Graph g(2, 2, "board.txt");
  //g.afficher();

  Graph g(b);
  Graph gbig(big);
  //gbig.getRobot(3, 8, 2, -1);
  //g2.afficher();
  Location depart_petit(0, 1);
  Location depart_big(0, 8);
  RR::Robot start(RR::Location(0, 1), (RR::Robot::Status)3);
  RR::Location end = {1, 0};
  std::vector<int> res;
  res = g.dijkstra(start, end);
  std::cout << std::endl;
  if (res.size() > 0)
  {
    for (int r : res)
    {
      std::cout << start.move[r] << std::endl;
    }
  }
  else
  {
    std::cout << "Aucune route possible" << std::endl;
  }

  testSpeady(gbig,big);
  testJoueurArt(big);


  return 0;
}
