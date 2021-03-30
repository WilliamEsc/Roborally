#include "board.hpp"
#include "graph.hpp"
#include "joueurArt.hpp"
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
  res=g2.dijkstra(start, end);
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
  end = {4, 0};
  res= g2.dijkstra(start, end);
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

  Graph* g3 = new Graph(b);
  std::cout<<std::endl<<"joueur artificiel : "<<std::endl;
  int cartex9[] = {1, 1, 2, 3, 4, 5, 5, 6, 1};
  end = {1, 0};
  JoueurArt j(g3, start, end, cartex9, b);
  j.afficher();
  delete g3;

  return 0;
}