#include "board.hpp"
#include "graph.hpp"

#include <iostream>

const int MAIN = 9;

bool contain(int contener[MAIN], std::vector<int> content)
{
    int copy[MAIN];
    bool find;
    for (long unsigned int i = 0; i < MAIN; i++)
    {
        /* code */
        copy[i] = contener[i];
    }
    for (long unsigned int i = 0; i < content.size(); i++)
    {
        find = false;
        for (long unsigned int j = 0; j < MAIN; j++)
        {
            if (copy[j] == content[i] && !find)
            { //réarrange la 1ere carte correspondant
                copy[j] = 9;
                find = true;
            }
        }
        if (!find)
        {
            return find;
        }
    }
    return find;
}

bool contain(std::vector<int> contener, std::vector<int> content)
{
    int copy[MAIN];
    bool find;
    for (size_t i = 0; i < MAIN; i++)
    {
        /* code */
        copy[i] = contener[i];
    }

    for (long unsigned int i = 0; i < content.size(); i++)
    {
        find = false;
        for (long unsigned int j = i; j < MAIN; j++)
        {
            if (copy[j] == content[i] && !find)
            { //réarrange la 1ere carte correspondant
                copy[j] = 9;
                find = true;
            }
        }
        if (!find)
        {
            return find;
        }
    }
    return find;
}

void automata(Graph &g, RR::Robot twoB, int carte[MAIN], const Location &end)
{
    std::vector<int> res = g.dijkstra(twoB, end);
    if (res.size() > 0)
    {
        if (!contain(carte, res)) // carte contient la solution de res
        {
            std::vector<int> best(0); //pour choisir la meilleure option
            for (size_t i = 0; i < MAIN; i++)
            {
                Cellule *rob = g.getRobot(twoB.location.line, twoB.location.column, (int)twoB.status, carte[i]);
                if (rob != nullptr)
                {
                    std::vector<int> test = g.dijkstra(rob->robot, end,carte);
                    int copy[9];
                    for (size_t k = 0; k < MAIN; k++)
                    {
                        /* code */
                        if (k != i)
                            copy[k] = carte[k];
                        else
                            copy[k] = 9;
                    }
                    if (test.size() > 0 && contain(copy, test))
                    {
                        if (best.size() == 0 || best.size() > test.size() + 1)
                        {
                            best = test;
                            best.push_back(carte[i]);
                        }
                    }
                }
            }
            for (size_t i = 0; i < best.size(); i++)
            {
                /* code */
                std::cout << twoB.move[best[i]] << std::endl;
            }
            res = best;
        }
        for (long unsigned int i = 0; i < res.size(); i++)
        {
            for (long unsigned int j = i; j < MAIN; j++)
            {
                if (carte[j] == res[i])
                { //réarrange la 1ere carte correspondant
                    int temp = carte[i];
                    carte[i] = carte[j];
                    carte[j] = temp;
                }
            }
        }
    }
}