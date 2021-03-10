#ifndef GRAPH_HPP_
#define GRAPH_HPP_

#include "board.hpp"
using namespace RR;

class Cellule {
  public:
    std::vector<Cellule*> tab;

    Robot::Status stat;
    Location loc;

    Cellule(Robot::Status s, Location l);

    //~Cellule();
  } ;

  class Graph
  {
    public :

      /* construction d'une liste vide */
      Graph(unsigned int x_, unsigned int y_, std::string b) ;
      Graph(unsigned int x_, unsigned int y_, Board b) ;

      /* construction par copie */
      Graph(const Graph& autre) ;

      /* affectation */
      Graph& operator=(const Graph& autre) ;

      /* destruction */
      ~Graph() ;

      /* consultation de la tete */
      const Cellule* tete() const ;
      Cellule* tete() ;

      /* insert l'élément dans la liste */
      void insert(int v);

      /* taille de la liste */
      int taille() const ;

      /* recherche dans la liste */
      Cellule* recherche(int v) ;

      /* affichage */
      void afficher() const ;

    private :

      std::vector<std::vector<std::vector<Cellule*>>> tabCell;
      Cellule* dead;
      unsigned int x, y;
      std::string str_board;


  } ;

#endif