#ifndef JOUEURART_HPP_
#define JOUEURART_HPP_

#include "board.hpp"
#include "graph.hpp"
#include <vector>

class JoueurArt{
    public:
    JoueurArt(Graph *g, Robot rob, RR::Location end, int cartex9[], RR::Board b);
    void afficher();
    
    int carteUsed;
    bool arrive;
    bool bloque;
    int carteAJouer[9];
    
    
};

#endif
