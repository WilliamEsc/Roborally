#include "joueurArt.hpp"
#include "graph.hpp"
#include <iostream>

//si l'element e appartient au tableau (de taille 9), on renvoie vrai
//bool suppr pour dire si on veut que l'element soit supprimer du tableau ou non
    //sinon renvoie faux
bool isIn2(int tab[], int e, bool suppr){
    for(int i=0; i<9; i++){
            if (tab[i] == e){
                if (suppr == true) tab[i] = -1; //on pourra plus utiliser ce mouvement
                return true;
            }
        }
    return false;
}

//si l'élément appartient au tableau (de taille 9), on renvoi vrai
//bool rempl: pour dire si on veut que l'élément cherché soit interverti avec l'élement à l'indice carteUsed
bool isIn(int tab[], int e, int &carteUsed, bool rempl){
    for(int i=carteUsed; i<9; i++){
            if (tab[i] == e){
                if (rempl == true){
                    //on intervertit les cases
                    tab[i] = tab[carteUsed];
                    tab[carteUsed] = e;
                    carteUsed++;
                } 
                return true;
            }
        }
    return false;
}

JoueurArt::JoueurArt(Graph *g, Robot rob, RR::Location end, int cartex9[], RR::Board b){
    Robot::Move tabMoves[7] = {
    Robot::Move::FORWARD_1,
    Robot::Move::FORWARD_2,
    Robot::Move::FORWARD_3,
    Robot::Move::BACKWARD_1,
    Robot::Move::TURN_LEFT,
    Robot::Move::TURN_RIGHT,
    Robot::Move::U_TURN };

    std::vector<int> ret = g->dijkstra(rob, end);
    std::vector<int> test;
    int mouv;
    int newMove=-1;
    bool ok = false;
    int tailleChem;
    
    carteUsed = 0;
    bloque = false;
    arrive = false;

    while(ret.size() > 0 && carteUsed < 5 && !bloque){
        mouv = ret.back(); //prochain mouvement à faire selon le chemin le plus court actuel
        ret.pop_back(); 
        ok = false;

        //on cherche si on a la carte pour le mouvement à faire
        ok = isIn(cartex9, mouv, carteUsed, true);
        
        if (ok == true ){
            //on a la carte, donc on l'utilise
            b.play(rob, tabMoves[mouv]);
        }
        else {
            //si on a pas la carte, il va falloir modifier notre chemin le plus court
            tailleChem = -1; //taille du chemin actuel, on le met à -1 pour obligatoirement le changer
            newMove = -1;
            for(int i=0; i<7; i++){
                if (i != mouv && rob.status != RR::Robot::Status::DEAD && isIn(cartex9, i, carteUsed, false)){
                    Robot robTest(RR::Location((int)rob.location.line,
                                (int) rob.location.column),
                                (RR::Robot::Status) rob.status);
                    b.play(robTest, tabMoves[i]);
                    if(robTest.status != RR::Robot::Status::DEAD){
                        test = g->dijkstra(robTest, end);
                        if (tailleChem == -1 || (int)test.size() < tailleChem ) {
                            ret.clear();
                            ret = test; 
                            newMove = i; 
                            tailleChem = ret.size();
                        }
                    }
                }
            }
            if (tailleChem == -1 || newMove == -1) bloque = true; //on est bloqué 
            else {
                ok = isIn(cartex9, newMove, carteUsed, true); //pour le supprimer des mouvements possibles
                b.play(rob, tabMoves[newMove]);
            }
        }
    }
    if (rob.location.column == end.column && rob.location.line == end.line) arrive = true;
    for (int i=0; i<9; i++) carteAJouer[i] = cartex9[i];
    
}

int JoueurArt::getCarteUsed(){
    return carteUsed;
}

void JoueurArt::afficher(){
    if(arrive) std::cout<<"arrive"<<std::endl;
    else std::cout<<"pas arrive"<<std::endl;

    std::cout<<"nombre de cartes utilisees: "<<carteUsed<<std::endl;

    RR::Robot start(RR::Location(0, 0), (RR::Robot::Status)0); //pour avoir accès à la correspondance des mouvements, peut importe les coordonnées
    for (int i=0; i<carteUsed; i++) std::cout<<start.move[carteAJouer[i]]<<std::endl;
}

