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
        if (nbBon < 5 && (nbBon != (int)res.size())) // carte contient la solution de res
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
                    if (rob != nullptr && rob->robot.status!=RR::Robot::Status::DEAD)
                    {
                        std::vector<int> test = g.dijkstraBonSens(rob->robot, end);

                        int tmp = contain(carte, test, nbBon);


                        if (tmp >= 5 && nbBon!=5)
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
                        twoB = g.getRobot(twoB.location.line, twoB.location.column, (int)twoB.status, save)->robot;
                        nbBon++;
                        break;
                    }
                }
                int nb=contain(carte, bestVec, nbBon);
                for (int i = 0; i < nb-nbBon; i++)
                {
                    twoB = g.getRobot(twoB.location.line, twoB.location.column, (int)twoB.status, bestVec[i])->robot;

                }
                nbBon = nb;
            }
        }
    }
}

void testSpeady(Graph& g, Board& b)
{
    int carte[9];
    int nbDead=0;
    int nbFini=0;
    int nbTours=0;
    int nbTrajet=0;
    while (nbTrajet<100)
    {
        //std::cout << std::endl<< " DEBUT " << std::endl;
        Cellule *deb = nullptr;
         while (deb == nullptr || deb->robot.status == RR::Robot::Status::DEAD)
        {
            int x = rand() % 24;
            int y = rand() % 11;
            int s = rand() % 4;
            deb = g.getRobot(x, y, s, -1);
        }
         Cellule *end = nullptr;
         while (end == nullptr || end->robot.status == RR::Robot::Status::DEAD)
        {
            int x = rand() % 24;
            int y = rand() % 11;
            int s = rand() % 4;
            end = g.getRobot(x, y, s, -1);
        }
        RR::Robot speady = deb->robot;
        //std::cout << "SPEADY " << speady.location.line<<" " << speady.location.column<<" " << (int)speady.status << std::endl;
        //std::cout << "END " << end->robot.location.line<<" " << end->robot.location.column << std::endl;
        std::vector<int> chemin = g.dijkstra(speady, end->robot.location);
        if (chemin.size()>0)
        {
            while (!(speady.location == end->robot.location) && speady.status != RR::Robot::Status::DEAD)
            {
                //std::cout << "tirage " << std::endl;
                for (int i = 0; i < 9; i++)
                {
                    carte[i] = rand() % 7;
                    //std::cout << speady.move[carte[i]] << " ";
                }

                //std::cout << std::endl<< "calcul " << std::endl;
                automata(g, speady, carte, end->robot.location);
                for (size_t k = 0; k < 5; k++)
                {
                    if (!(speady.location == end->robot.location))
                    {
                        //std::cout << k << " " << speady.move[carte[k]] << std::endl;
                        b.play(speady, (RR::Robot::Move)carte[k]);
                    }
                }
                nbTours++;
                //std::cout << "pos " << speady.location.line << speady.location.column << (int)speady.status << std::endl;
                //std::cout << "fin calcul " << std::endl;
            }
            if (speady.status == RR::Robot::Status::DEAD)
            {
                //std::cout << " DEAD " << std::endl;
                nbDead++;
            }
            else
            {
                //std::cout << " FINI " << std::endl;
                nbFini++;
            }
            nbTrajet++;
        }
    }
    std::cout<<nbFini << " vs " << nbDead<<std::endl;
}