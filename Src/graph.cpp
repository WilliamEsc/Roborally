#include "graph.hpp"

#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_map>

using namespace RR;

Cellule::Cellule(const Robot::Status &s, const Location &l) : robot({l, s}) {}

Graph::Graph(Board b) : tabCell(), dead(nullptr)
{

  //on fait les aretes
  Robot::Move tabMoves[7] = {
      Robot::Move::FORWARD_1,
      Robot::Move::FORWARD_2,
      Robot::Move::FORWARD_3,
      Robot::Move::BACKWARD_1,
      Robot::Move::TURN_LEFT,
      Robot::Move::TURN_RIGHT,
      Robot::Move::U_TURN};

  Robot::Status s[4] = {
      Robot::Status::WEST,
      Robot::Status::NORTH,
      Robot::Status::EAST,
      Robot::Status::SOUTH};

  //initialisation du tableau (les cases)
  std::vector<std::vector<Cellule *>> v1(0);
  std::vector<Cellule *> v2(0);
  Location spot;

  for (auto &it : b.tiles)
  {
    spot.line = it.first.line;
    spot.column = it.first.column;
    while (spot.line >= (int)tabCell.size())
    {
      tabCell.push_back(v1);
    }
    while (spot.column >= (int)tabCell[spot.line].size())
    {
      tabCell[spot.line].push_back(v2); //on met un vector vide pour que la prochaine cellule soient accessible avec les bons coordonnés
    }
    for (Robot::Status st : s)
    {
      tabCell[spot.line][spot.column].push_back(new Cellule(st, spot));
    }
  }

  RR::Robot rob;
  for (long unsigned int i = 0; i < tabCell.size(); i++)
  {
    for (long unsigned int j = 0; j < tabCell[i].size(); j++)
    {
      if (!tabCell[i][j].empty())
      {
        for (int k = 0; k < 4; k++)
        {
          for (int l = 0; l < 7; l++)
          {
            rob = {{(int)i, (int)j}, s[k]};
            b.play(rob, tabMoves[l]);
            switch (rob.status)
            {
            case Robot::Status::DEAD:
              tabCell[i][j][k]->tab.push_back(dead);
              break;
            case Robot::Status::EAST:
              tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][0]);
              break;
            case Robot::Status::NORTH:
              tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][1]);
              break;
            case Robot::Status::WEST:
              tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][2]);
              break;
            case Robot::Status::SOUTH:
              tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][3]);
              break;
            default:
              break;
            }
          }
        }
      }
    }
  }
}

void Graph::afficher() const
{
  for (unsigned int i = 0; i < tabCell.size(); i++)
  {
    for (unsigned int j = 0; j < tabCell[i].size(); j++)
    {
      if (!tabCell[i][j].empty())
      {
        for (unsigned int k = 0; k < 4; k++)
        {
          std::cout << i << " " << j << " " << k << std::endl;
          std::cout << tabCell[i][j][k]->tab.size() << std::endl;
          std::cout << std::endl;
        }
      }
    }
    std::cout << std::endl;
  }

  /*std::cout<<"ex: "<<std::endl;
    for (unsigned int i = 0; i<tabCell[4][0][1]->tab.size(); i++){
      std::cout<<i<<std::endl;
      if (tabCell[4][0][1]->tab[i] == dead) std::cout<<"=="<<std::endl;
    }*/
}

Graph::~Graph()
{
  for (unsigned int i = 0; i < tabCell.size(); i++)
  {
    for (unsigned int j = 0; j < tabCell[i].size(); j++)
    {
      if (!tabCell[i][j].empty())
      {
        for (unsigned int k = 0; k < 4; k++)
        {
          delete tabCell[i][j][k];
        }
      }
    }
  }
}

tasB::tasB() : ind()
{
}
tasB::~tasB()
{
}

int tasB::insert_modify(const Case &c)
{
  std::unordered_map<Robot, int, RobotHash>::const_iterator got = ind.find(c.parent);
  if (got == ind.end()) //le robot n'est pas présent dans le tas
  {
    //insert
    //std::cout << "ins ";
    int i = tb.size();
    tb.push_back(c);
    while (i > 0 && c.poids < tb[(int(i - 1) / 2)].poids)
    {
      //std::cout<<(int (i-1)/2);
      tb[i] = tb[(int(i - 1) / 2)];
      ind[tb[i].parent] = i; //modifier les indices des robots que l'on déplace
      i = (int(i - 1) / 2);
    }
    ind[c.parent] = i;
    return 1;
  }
  else //le robot est présent dans le tas
  {
    //modify
    //std::cout << "mod ";
    int i = ind.at(c.parent);
    if (tb[i].poids > c.poids)
    {
      while (i > 0 && c.poids < tb[(int(i - 1) / 2)].poids)
      {
        tb[i] = tb[(int(i - 1) / 2)];
        ind[tb[i].parent] = i; //modifier les indices des robots que l'on déplace
        i = (int(i - 1) / 2);
      }
      tb[i] = c;
      ind[c.parent] = i;
      return 2;
    }
  }
  return 0;
}

Case tasB::pop()
{
  Case ret = tb[0];
  int s = tb.size() - 1;
  if (s == 0)
  {
    tb.pop_back();
    ind.erase(ret.parent);
    return ret;
  }
  Case e = tb[s];
  tb.pop_back();
  int i = 0;
  while (((2 * i + 1 < s && e.poids > tb[2 * i + 1].poids) || (2 * i + 2 < s && e.poids > tb[2 * i + 2].poids)))
  {
    if (2 * i + 2 < s && tb[2 * i + 1].poids > tb[2 * i + 2].poids)
    {
      tb[i] = tb[2 * i + 2];
      ind[tb[2 * i + 2].parent] = i; //modifier les indices des robots que l'on déplace
      i = 2 * i + 2;
    }
    else
    {
      tb[i] = tb[2 * i + 1];
      ind[tb[2 * i + 1].parent] = i; //modifier les indices des robots que l'on déplace
      i = 2 * i + 1;
    }
  }
  ind[e.parent] = i;
  tb[i] = e;
  ind.erase(ret.parent);
  return ret;
}

bool tasB::empty()
{
  return tb.empty();
}

Case::Case(const Robot &r, const int &p) : parent(r), poids(p) {}
Case::Case() : parent(), poids(-1) {}

void Graph::dijkstra(const Robot &start, const Location &arrive, std::vector<int> &res)
{
  //std::cout << "start ";
  std::unordered_map<Robot, Case, RobotHash> road; //Robot -> parent,poids
  Case st(start, 0);
  road[start] = st;
  tasB s;
  s.insert_modify(st);
  while (!s.empty())
  {
    //std::cout << "1 ";
    Case temp = s.pop(); //Attention ici temp est issue du tas donc parent est le robot en lui meme avec son poids
    for (int x = 0; x < 7; x++)
    {
      Cellule *r = tabCell[temp.parent.location.line][temp.parent.location.column][(int)temp.parent.status]->tab[x];
      if (r != nullptr && r->robot.status != Robot::Status::DEAD)
      {
        int dw;
        std::unordered_map<Robot, Case, RobotHash>::const_iterator got = road.find(r->robot);
        if (got == road.end())
        {
          //not found
          dw = -1;
        }
        else
        {
          //found
          dw = road.at(r->robot).poids;
        }
        if ((dw == -1) || (dw > temp.poids + 1))
        {
          //std::cout << "in-" << temp.poids;
          Case maj(temp.parent, temp.poids + 1);
          road[r->robot] = maj;
          s.insert_modify({r->robot, temp.poids + 1});
        }
      }
    }
    //std::cout << std::endl;
  }

  //choix du robot en position final avec le poids le plus faible
  Robot final = {arrive, (Robot::Status)0};
  for (int i = 1; i < 4; i++)
  {
    std::unordered_map<Robot, Case, RobotHash>::const_iterator got = road.find(final);
    if (got != road.end())
    {
      Robot test = {arrive, (Robot::Status)i};
      if (road[final].poids >= road[test].poids)
      {
        final = test;
      }
    }
  }

  //push des parents de chaque position du chemin entre le robot final et le robot d'arrivée
  std::unordered_map<Robot, Case, RobotHash>::const_iterator got = road.find(final);
  if (got != road.end())
  {
        //found
    Case chemin = road[final];
    Cellule *test = tabCell[chemin.parent.location.line][chemin.parent.location.column][(int)chemin.parent.status];
    while (!(chemin.parent == start))
    {
      for (int i = 0; i < 7; i++)
      {
        if (test->tab[i] != nullptr)
        {
          if (test->tab[i]->robot == final)
          {
            res.push_back(i);
            final = chemin.parent;
            chemin = road[final];
            test = tabCell[chemin.parent.location.line][chemin.parent.location.column][(int)chemin.parent.status];
            break;
          }
        }
      }
    }
    for (int i = 0; i < 7; i++)
    {
      if (test->tab[i] != nullptr && test->tab[i]->robot == final)
      {
        res.push_back(i);
        test = tabCell[final.location.line][final.location.column][(int) final.status];
        final = chemin.parent;
        chemin = road[chemin.parent];
        break;
      }
    }
  }

}