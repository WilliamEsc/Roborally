#include "graph.hpp"

#include <fstream>
#include <iostream>
#include <queue>

Cellule::Cellule(Robot::Status s, Location l) : robot({l, s}) {}

Graph::Graph(unsigned int x_, unsigned int y_, std::string b) : x(x_), y(y_), str_board(b)
{
  dead = nullptr;
  //initialisation du tableau (les cases)
  Board board(str_board);
  int numL = 0;
  int numC = 0;
  std::vector<std::vector<Cellule *>> v1;
  std::vector<Cellule *> v2;
  v2.clear();
  Location spot;
  std::ifstream file("board.txt");

  //on parcours le fichier du plateau pour créer le tableau avec les bonnes dimensions
  while (!file.eof())
  {
    int type;
    file >> spot.line >> spot.column >> type;
    if (spot.line != numL)
    {
      numL = spot.line;
      numC = 0; //num de colonne
      tabCell.push_back(v1);
      v1.clear();
    }
    while (numC < spot.column)
    {
      v1.push_back(v2); //on met un vector vide pour que la prochaine cellule soient accessible avec les bons coordonnés
      numC++;
    }
    Location l(spot.line, spot.column);
    v2.push_back(new Cellule(Robot::Status::EAST, l));
    v2.push_back(new Cellule(Robot::Status::NORTH, l));
    v2.push_back(new Cellule(Robot::Status::WEST, l));
    v2.push_back(new Cellule(Robot::Status::SOUTH, l));
    v1.push_back(v2);
    v2.clear();
    numC++;
  }
  tabCell.push_back(v1);
  file.close();

  //on fait les aretes
  Robot::Move tabMoves[7] = {Robot::Move::FORWARD_1,
                             Robot::Move::FORWARD_2,
                             Robot::Move::FORWARD_3,
                             Robot::Move::BACKWARD_1,
                             Robot::Move::TURN_LEFT,
                             Robot::Move::TURN_RIGHT,
                             Robot::Move::U_TURN};
  RR::Robot rob;
  for (int i = 0; i < tabCell.size(); i++)
  {
    for (int j = 0; j < tabCell[i].size(); j++)
    {
      if (!tabCell[i][j].empty())
      {
        for (int k = 0; k < 4; k++)
        {
          Robot::Status s;
          switch (k)
          {
          case 0:
            s = Robot::Status::EAST;
            break;
          case 1:
            s = Robot::Status::NORTH;
            break;
          case 2:
            s = Robot::Status::WEST;
            break;
          case 3:
            s = Robot::Status::SOUTH;
            break;
          default:
            break;
          }

          for (int l = 0; l < 7; l++)
          {
            rob = {{i, j}, s};
            board.play(rob, tabMoves[l]);
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

Graph::Graph(unsigned int x_, unsigned int y_, Board b) : x(x_), y(y_), tabCell(0)
{
  dead = nullptr;

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
      Robot::Status::EAST,
      Robot::Status::NORTH,
      Robot::Status::WEST,
      Robot::Status::SOUTH};

  //initialisation du tableau (les cases)
  std::vector<std::vector<Cellule *>> v1;
  std::vector<Cellule *> v2;
  v1.clear();
  v2.clear();
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

tasB::tasB() : tb(0) {}

int tasB::insert_modify(Case c)
{
  std::unordered_map<Robot, int>::const_iterator got = ind.find(c.parent);
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
      ind[tb[2 * i + i].parent] = i; //modifier les indices des robots que l'on déplace
      i = 2 * i + 1;
    }
  }
  ind[e.parent] = i;
  tb[i] = e;
  ind.erase(ret.parent);
  return ret;
}

int tasB::get_poids(Robot r)
{
  std::unordered_map<Robot, int>::const_iterator got = ind.find(r);
  if (got == ind.end())
  {
    //not found
    return -1;
  }
  else
  {
    //found
    return tb[ind.at(r)].poids;
  }
}

bool tasB::empty()
{
  return tb.empty();
}

Case::Case(Robot r, int p) : parent(r), poids(p) {}
Case::Case() : parent(), poids(-1) {}

std::vector<Case> Graph::dijkstra(Robot start, Location arrive)
{
  //std::cout << "start ";
  std::unordered_map<Robot, Case, RobotHash> road; //Robot -> parent,poids
  Case r(start, 0);
  road[start] = r;
  tasB s;
  s.insert_modify(r);
  while (!s.empty())
  {
    //std::cout << "1 ";
    Case temp = s.pop();
    for (int x = 0; x < 7; x++)
    {
      Cellule *r = tabCell[temp.parent.location.line][temp.parent.location.column][(int)temp.parent.status]->tab[x];
      if (r != nullptr && r->robot.status != Robot::Status::DEAD)
      {
        int dw;
        std::unordered_map<Robot, Case>::const_iterator got = road.find(r->robot);
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

  for (auto &r : road)
  {
    std::cout << r.first.location.column << r.first.location.line << r.second.poids << std::endl;
  }
  
  //choix du robot en position final avec le poids le plus faible
  Robot final = {arrive, (Robot::Status)0};
  for (int i = 1; i < 4; i++)
  {
    Robot test = {arrive, (Robot::Status)i};
    if (road[final].poids >= road[test].poids)
    {
      final = test;
    }
  }

  //push des parents de chaque position du chemin entre le robot final et le robot d'arrivée
  std::vector<Case> ret;
  Case chemin = road[final];
  while (!(chemin.parent == start))
  {
    ret.push_back(chemin);
    chemin = road[chemin.parent];
  }
  ret.push_back(chemin);
  return ret;
}