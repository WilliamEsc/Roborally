#include "board.hpp"
#include "graph.hpp"
#include <iostream>

int main()
{
  RR::Board b("board.txt");
  b.save("/tmp/cpy.txt");

  //RR::Graph g(2, 2, "board.txt");
  //g.afficher();

  Graph g2(b);
  //g2.afficher();
  RR::Robot start(RR::Location(0, 1), (RR::Robot::Status)3);
  RR::Location end = {1, 0};
  std::vector<int> res;
  g2.dijkstra(start, end, res);
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
  end = {1, 0};
  g2.dijkstra(start, end, res);
  std::cout << std::endl;
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
  return 0;
}