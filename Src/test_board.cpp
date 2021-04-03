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

  testJoueurArt(b);


  int carte[9]={0,1,2,1,4,6,6,6,6};
  automata(g2,start,carte,end);
  std::cout <<"best "<< std::endl;

  for(long unsigned int i=0;i<res.size();i++){
    std::cout<<start.move[res[i]]<<std::endl;
  }
  std::cout <<"carte "<< std::endl;
  for (size_t i = 0; i < 9; i++)
  {
    std::cout<<start.move[carte[i]]<<std::endl;
  }


  return 0;
}