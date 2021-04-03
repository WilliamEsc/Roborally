#include "board.hpp"
#include "graph.hpp"

#include <iostream>

const int MAIN = 9;

int contain(int contener[MAIN], std::vector<int> content, int nbBon)
{
    int nbfind = nbBon;
    for (long unsigned int i = 0; i < content.size(); i++)
    {
        bool find = false;
        for (long unsigned int j = i + nbBon; j < MAIN; j++)
        {
            if (contener[j] == content[i])
            { //réarrange la 1ere carte correspondant
                int tmp = contener[nbfind];
                contener[nbfind] = contener[j];
                contener[j] = tmp;
                nbfind++;
                find = true;
                break;
            }
        }
        if (!find || nbfind == 5)
        {
            return nbfind;
        }
    }
    return nbfind;
}

void affiche(std::vector<int> content, RR::Robot twoB)
{
    std::cout << "pos " << twoB.location.line << twoB.location.column << (int)twoB.status << std::endl;
    for (size_t i = 0; i < content.size(); i++)
    {
        std::cout << twoB.move[content[i]] << std::endl;
    }
}

void automata(Graph &g, RR::Robot twoB, int carte[MAIN], const Location &end)
{
    //std::cout << "pos " << twoB.location.line << twoB.location.column << (int)twoB.status << std::endl;
    std::vector<int> res = g.dijkstraBonSens(twoB, end);
    if (res.size() > 0)
    {
        int nbBon = contain(carte, res, 0);
        //std::cout << "nbBon :" << nbBon << std::endl;
        if ( nbBon < 5 && (nbBon != (int) res.size()) ) // carte contient la solution de res
        {
            for (int i = 0; i < nbBon; i++)
            {
                twoB = g.getRobot(twoB.location.line, twoB.location.column, (int)twoB.status, carte[i])->robot;
                //std::cout << "debut " << i << " " << twoB.move[carte[i]] << std::endl;
            }

            for (size_t n = nbBon; n < 5; n++)
            {
                int bestnb = -1;
                int bestI = -1;
                std::vector<int> bestVec;
                for (int i = nbBon; i < MAIN; i++)
                {
                    int save = carte[i];
                    carte[i] = carte[nbBon];
                    carte[nbBon] = save;
                    //std::cout << "pos " << i << " " << nbBon << " " << twoB.location.line << twoB.location.column << (int)twoB.status << twoB.move[save] << std::endl;
                    nbBon++;
                    Cellule *rob = g.getRobot(twoB.location.line, twoB.location.column, (int)twoB.status, save);
                    if (rob != nullptr)
                    {
                        std::vector<int> test = g.dijkstraBonSens(rob->robot, end);

                        int tmp = contain(carte, test, nbBon);

                        if (tmp >= 4)
                        {
                            //std::cout << "2 " << twoB.move[save] << std::endl;
                            //affiche(test, rob->robot);
                            //nbBon = 5;
                            return;
                        }
                        else if (tmp > bestnb)
                        {
                            //std::cout << "best :" << tmp << " " << twoB.move[save] << std::endl;
                            bestnb = tmp;
                            bestI = save;
                            bestVec = test;
                        }
                    }
                    nbBon--;
                }
                //std::cout << "end :" << n << std::endl;
                for (size_t i = nbBon; i < 9; i++)
                {
                    if (carte[i] == bestI)
                    {
                        int save = carte[i];
                        carte[i] = carte[nbBon];
                        carte[nbBon] = save;
                        nbBon++;
                    }
                }
                nbBon = contain(carte, bestVec, nbBon);
            }
        }
    } 
}