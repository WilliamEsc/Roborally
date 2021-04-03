#include "board.hpp"
#include "graph.hpp"
#include "joueurArt.hpp"
#include "automata.cpp"
#include <iostream>

int main()
{
  RR::Board b("board.txt");
  b.save("/tmp/cpy.txt");

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

  Graph *g3 = new Graph(b);
  std::cout << std::endl
            << "joueur artificiel : " << std::endl;
  int cartex9[9] = {0, 1, 2, 1, 4, 6, 6, 6, 6};
  end = {1, 0};
  JoueurArt j(g3, start, end, cartex9, b);
  j.afficher();

  int carte[9];
  Location end_big = {0, 2};
  /* RR::Robot crash(RR::Location(8,0), (RR::Robot::Status)2);
  res=gbig.dijkstra(crash, end_big);
  std::cout << std::endl;
  std::cout<<std::endl<<"crash : "<<std::endl;
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
  } */
  while (true)
  {
    std::cout << std::endl<< " DEBUT " << std::endl;
    Cellule *deb = nullptr;
    while (deb == nullptr || deb->robot.status == RR::Robot::Status::DEAD)
    {
      int x = rand() % 24;
      int y = rand() % 11;
      int s = rand() % 4;
      deb = gbig.getRobot(x, y, s, -1);
    }
    Cellule *end = nullptr;
    while (end == nullptr || end->robot.status == RR::Robot::Status::DEAD)
    {
      int x = rand() % 24;
      int y = rand() % 11;
      int s = rand() % 4;
      end = gbig.getRobot(x, y, s, -1);
    }
    RR::Robot speady = deb->robot;
    std::cout << "SPEADY " << speady.location.line << speady.location.column << (int)speady.status << std::endl;
    std::cout << "END " << end->robot.location.line << end->robot.location.column << std::endl;
    std::vector<int> chemin = gbig.dijkstra(speady, end->robot.location);
    if (chemin.size() > 0)
    {
      while (!(speady.location == end->robot.location) && speady.status != RR::Robot::Status::DEAD)
      {
        std::cout << "tirage " << std::endl;
        for (int i = 0; i < 9; i++)
        {
          carte[i] = rand() % 7;
          std::cout << start.move[carte[i]] << " ";
        }

        std::cout << std::endl<< "calcul " << std::endl;
        automata(gbig, speady, carte, end->robot.location);
        for (size_t k = 0; k < 5; k++)
        {
          if (!(speady.location == end->robot.location))
          {
            std::cout << k << " " << start.move[carte[k]] << std::endl;
            big.play(speady, (RR::Robot::Move)carte[k]);
          }
        }
        //std::cout << "pos " << speady.location.line << speady.location.column << (int)speady.status << std::endl;
        //std::cout << "fin calcul " << std::endl;
      }
      if(speady.status == RR::Robot::Status::DEAD){
        std::cout << " DEAD " << std::endl;
      }else{
        std::cout << " FINI " << std::endl;
      }
    }
  }

  delete g3;

  return 0;
}