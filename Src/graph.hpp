#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "board.hpp"
#include <unordered_map>
using namespace RR;

class Cellule
{
public:
  std::vector<Cellule *> tab;

  Robot robot;

  Cellule(const Robot::Status& s,const Location& l);

  //~Cellule();
};

class Case
{
public:
  RR::Robot parent;
  int poids;

  Case(const Robot& r,const int& p);
  Case();
};

class Graph
{
public:
  /* construction d'une liste vide */
  Graph( Board b);

  std::vector<int> dijkstra(const Robot& start,const Location& arrive);

  /* construction par copie */
  Graph(const Graph &autre);

  /* affectation */
  Graph &operator=(const Graph &autre);

  /* destruction */
  ~Graph();

  /* consultation de la tete */
  const Cellule *tete() const;
  Cellule *tete();

  /* insert l'élément dans la liste */
  void insert(int v);

  /* taille de la liste */
  int taille() const;

  /* recherche dans la liste */
  Cellule *recherche(int v);

  /* affichage */
  void afficher() const;

private:
  std::vector<std::vector<std::vector<Cellule *>>> tabCell;
  Cellule *dead;
};

class tasB
{
public:
  std::vector<Case> tb; // tableau du tas binaire, avec le robot et sa priorité
  std::unordered_map<Robot, int, RobotHash> ind;//permet de retrouver l'indice de chaque robot

  tasB();
  ~tasB();

  int insert_modify(const Case& c);

  Case pop();


  bool empty();
};

#endif