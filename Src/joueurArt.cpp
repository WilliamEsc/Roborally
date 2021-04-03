#include "graph.hpp"
#include <iostream>
#include <stack>
#include <vector>

//si l'élément e appartient au vector, on renvoie vrai et on le supprime
bool supprCarte(std::vector<int> &vect, int e){
    for(int i=0; i<(int)vect.size(); i++){
            if (vect[i] == e){
                vect[i] = vect[vect.size()-1]; 
                vect.pop_back(); 
                return true;
            }
        }
    return false;
}

//l'élément e est interverti avec l'élément à l'indice carteUsed 
void interverti(int tab[], int e, int carteUsed){
    for(int i=carteUsed; i<9; i++){
            if (tab[i] == e){
                tab[i] = tab[carteUsed];
                tab[carteUsed] = e;
                return;
            }
        }
}

//si l'élément appartient au vector, on renvoi vrai
bool isIn(std::vector<int> vect, int e){
    for(int i=0; i<(int)vect.size(); i++){
            if (vect[i] == e) return true;
        }
    return false;
}

//copie le tableau (de taille 9) dans le vector
void tabToVector(int tab[], std::vector<int> &vect){
    for (int i=0; i<9; i++) vect.push_back(tab[i]);
}

//remplace les éléments du deuxième tableau avec ceux du premier tableau
void ecraseTab(int cartex9[], int cartex9Best[]){
    for (int i=0; i<9; i++) cartex9Best[i] = cartex9[i];
}

//remplit le tableau de cartes de manière aléatoirement en tenant compte de leur rareté dans le jeu original
void tirageCartes(int cartex9[]){
    int cartes[84];
    for (int i = 0; i<6; i++) cartes[i] = 6; //6 U_TURN (6)
    for (int i = 6; i<24; i++) cartes[i] = 4; //18 TURN_LEFT (4)
    for (int i = 24; i<42; i++) cartes[i] = 5; //18 TURN_RIGHT (5)
    for (int i = 42; i<48; i++) cartes[i] = 3; //6 BACKWARD_1 (3)
    for (int i = 48; i<66; i++) cartes[i] = 0; //18 FORWARD_1 (0)
    for (int i = 66; i<78; i++) cartes[i] = 1; //12 FORWARD_2 (1)
    for (int i = 78; i<84; i++) cartes[i] = 2; //6 FORWARD_3 (2)

    for(int i=0; i<9; i++){
        int n = rand()%(84-i); 
        cartex9[i] = cartes[n];
        cartes[n] = cartes[84-1-i]; //cartes[i] prend la dernière case
    }
}

//retourne soit le nombre de cartes (mouvements) nécessaires pour arriver à destination,
//soit -1 si il n'y a pas de chemins possibles
int joueurArt(Graph *g, Robot rob, RR::Location end, int cartex9[], RR::Board b){
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
    RR::Robot robTest;
    std::vector<int> vectCartes; //pour remplir et vider la pile (pour savoir auxquelles cartes on a accès)
    tabToVector(cartex9, vectCartes);

    int mouv;
    int newMove=-1;
    bool ok = false;
    int tailleChem;
    
    int carteUsedTemp = 0;
    int bloque = false;
    bool bloqueBloque = false;
    int arrive = false;

    int cartex9Best[9]; //LE chemin le plus court
    for (int i=0; i<9; i++) cartex9Best[i] = -1;
    int carteUsed = 0; //nombre de carte utilisées dans LE chemin le plus court
    bool arriveTemp = false;

    //le robot d'un niveau de la première pile est associé au vector de mouvements possibles du meme niveau de la deuxième pile
        //du coup les 2 piles ont forcément la même taille
    std::stack<RR::Robot> pileRobot;
    std::stack<std::vector<int>> pileActionsPossibles;

    if(ret.size() == 0) return 0;
    
    while(!bloqueBloque){
        mouv = ret.back(); //prochain mouvement à faire selon le chemin le plus court actuel
        ret.pop_back(); 
        ok = false;

        //on cherche si on a la carte pour le mouvement à faire
        ok = isIn(vectCartes, mouv);
        
        if (ok == true ){
            //on a la carte, donc on l'utilise
            interverti(cartex9, mouv, carteUsedTemp); //pour bouger move dans le tableau
            carteUsedTemp++;
            pileRobot.push(rob); //on retient la position du robot avant le mouvement
            supprCarte(vectCartes, mouv); //on va tester mouv, donc on reviendra pas dessus plus tard
            pileActionsPossibles.push(vectCartes); //on retient les actions qui pourront etre testées pour quand on reviendra à ce mouvement
            b.play(rob, tabMoves[mouv]);
        }
        else {
            //si on a pas la carte, il va falloir modifier notre chemin le plus court
            tailleChem = -1; 
            newMove = -1; 
            for(int i=0; i<(int)vectCartes.size(); i++){ //on parcourt tout ce que les cartes qui nous restent nous permettent de faire
                if (rob.status != RR::Robot::Status::DEAD){
                    robTest = rob;
                    b.play(robTest, tabMoves[vectCartes[i]]);
                    if(robTest.status != RR::Robot::Status::DEAD){
                        test = g->dijkstra(robTest, end);
                        if (tailleChem == -1 || (int)test.size() < tailleChem ) {
                            ret.clear();
                            ret = test; 
                            newMove = vectCartes[i]; 
                            tailleChem = ret.size();
                        }
                    }
                }
            }
            if (tailleChem == -1 || newMove == -1) bloque = true; //on est bloqué 
            else {
                //on a trouvé un nouveau chemin
                interverti(cartex9, newMove, carteUsedTemp); //pour bouger newMove dans le tableau
                carteUsedTemp++;
                pileRobot.push(rob);
                supprCarte(vectCartes, newMove);
                pileActionsPossibles.push(vectCartes);
                b.play(rob, tabMoves[newMove]); //on joue newMove (le nouveau chemin commence après newMove)
            }
        }
        if (rob.location.column == end.column && rob.location.line == end.line) {
            //on est arrivé à la case de destination
            arriveTemp = true;
            if (carteUsedTemp < carteUsed || arrive == false){
                //le chemin est plus court que celui qu'on avait avant, donc on le retient
                arrive = true; 
                carteUsed = carteUsedTemp;
                ecraseTab(cartex9, cartex9Best);
            }
        }
        if( bloque || carteUsedTemp>=5 || arriveTemp) {
            //différents cas ou on ne va pas chercher à aller plus loin dans le chemin actuel
                //on retourne sur nos pas pour explorer d'autres possiblités
            if (!pileRobot.empty()){
                if(cartex9Best[0] == -1) {
                    //cas ou on n'a pas encore de chemin, donc on prend ce qu'il y a 
                    ecraseTab(cartex9, cartex9Best);
                    carteUsed = carteUsedTemp;
                }
                //on récupère le robot précédent (avant la denière action) et le vecteur de cartes qui lui correspond
                rob = pileRobot.top();
                pileRobot.pop();
                vectCartes = pileActionsPossibles.top();
                pileActionsPossibles.pop();
                carteUsedTemp--; //on revient en arrière, donc on utilise plus ula dernière carte qu'on a utilisé
                bloque = false;
                ret = g->dijkstra(rob, end); 
                arriveTemp = false;
            }
            else bloqueBloque = true;
        }
    }
    ecraseTab(cartex9Best, cartex9);
    if (arrive) return carteUsed;
    return -1;
}


void afficherChemin(int cartex9[], int carteUsed){
    RR::Robot start(RR::Location(0, 0), (RR::Robot::Status)0); //pour avoir accès à la correspondance des mouvements, peut importe les coordonnées

    std::cout << std::endl << "IA (à l'endroit): " << std::endl;
    std::cout << "jeu de cartes : (";
    for (int i=0; i<9; i++) std::cout<<start.move[cartex9[i]]<<" ";
    std::cout<<")"<<std::endl;
    if(carteUsed >= 0) std::cout << "arrivé avec " << carteUsed << " cartes  "<< std::endl;
    else {
        std::cout << "pas possible d'arriver avec les cartes fournies, on peut se rapprocher avec:" << std::endl;
        carteUsed = 5; //on regarde les 5 premières pour voir ou on peut avancer
    }
    

    for (int i=0; i<carteUsed; i++) std::cout << start.move[cartex9[i]] << std::endl;
    std::cout<<std::endl;
}

void afficherChemin(std::vector<int> res){
    std::cout << "dikjstra (à l'envers)" << std::endl;
    if(res.size() > 0) std::cout << "arrivé avec "<<res.size() << " cartes  " << std::endl;
    else {
        //res.size() == 0
        std::cout << "pas de mouvements nécessaires" << std::endl;
        return;
    }

    RR::Robot start(RR::Location(0, 0), (RR::Robot::Status)0); //pour avoir accès à la correspondance des mouvements, peut importe les coordonnées
    for (int r : res) std::cout << start.move[r] << std::endl;
        
    //for (int i=0; i<(int)res.size(); i++) std::cout<<start.move[res[i]]<<std::endl;
}

void testJoueurArt(RR::Board b){
    Graph* g3 = new Graph(b);
    Graph g2(b);
    RR::Location end;
    std::vector<int> res;
    int carteUsed; //nombre de cartes (de mouvements) utilisés

    std::cout << std::endl << "//////////////////////////" << std::endl;
    RR::Robot start(RR::Location(0, 1), (RR::Robot::Status)3);
    int cartex9[] = {0,0,2,3,4,6,6,6,6};
    
    std::cout << "test chemin (0, 1) -> (0, 1)" << std::endl;
    end = {0, 1};
    res = g2.dijkstra(start, end);
    afficherChemin(res);
    carteUsed = joueurArt(g3, start, end, cartex9, b);
    afficherChemin(cartex9, carteUsed);

    /*std::cout << "test chemin (0, 1) -> (1, 0)" << std::endl;
    end = {1, 0};
    res = g2.dijkstra(start, end);
    afficherChemin(res);
    carteUsed = joueurArt(g3, start, end, cartex9, b);
    afficherChemin(cartex9, carteUsed);*/

    std::cout << "test chemin (0, 1) -> (4, 5)" << std::endl;
    end = {4, 5};
    res = g2.dijkstra(start, end);
    afficherChemin(res);
    carteUsed = joueurArt(g3, start, end, cartex9, b);
    afficherChemin(cartex9, carteUsed);
   
    std::cout << std::endl << "//////////////////////////" << std::endl;
    std::cout << "avec cartes au hasard :" << std::endl;
    end = {1, 0};
    tirageCartes(cartex9); //pour avoir 9 cartes au hasard
    carteUsed = joueurArt(g3, start, end, cartex9, b);
    afficherChemin(cartex9, carteUsed);
    delete g3;
    std::cout << "//////////////////////////" << std::endl;
}
