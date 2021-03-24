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

  Cellule(Robot::Status s, Location l);

  //~Cellule();
};

class Case
{
public:
  RR::Robot parent;
  int poids;

  Case(Robot r, int p);
  Case();
};

class Graph
{
public:
  /* construction d'une liste vide */
  Graph(unsigned int x_, unsigned int y_, std::string b);
  Graph(unsigned int x_, unsigned int y_, Board b);

  std::vector<Case> dijkstra(Robot start, Location arrive);

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
  unsigned int x, y;
  std::string str_board;
};

class tasB
{
public:
  std::vector<Case> tb; // tableau du tas binaire, avec le robot et sa priorité
  std::unordered_map<Robot, int, RobotHash> ind;//permet de retrouver l'indice de chaque robot

  tasB();

  int insert_modify(Case c);

  Case pop();

  int get_poids(Robot r);

  bool empty();
};

#endif