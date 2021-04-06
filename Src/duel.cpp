#include "board.hpp"
#include "graph.hpp"
#include "joueurArt.cpp"
#include "automata.cpp"
#include <iostream>

int main()
{
    Board b("big_board.txt");
    Graph g(b);
    Robot depart = {{0, 2}, RR::Robot::Status::SOUTH};
    Robot target = {{22, 10}, RR::Robot::Status::NORTH};

    srand((unsigned)time(0));
    std::cout << "DEBUT DU TEST" << std::endl;
    std::cout << "ROBOT = " << depart.location.line<< depart.location.column <<(int) depart.status<< std::endl;
    std::cout << "TARGET = " << target.location.line << target.location.column << std::endl;

    int total1 = 0;
    int total2 = 0;
    int victoire1 = 0;
    int victoire2 = 0;
    int draw = 0;

    for (int i = 0; i < 100; i++)
    {
        RR::Robot player1 = depart;
        RR::Robot player2 = depart;
        int tours1 = 0;
        int tours2 = 0;
        while ((!(player1.location == target.location) && player1.status != Robot::Status::DEAD) ||
                (!(player2.location == target.location) && player2.status != Robot::Status::DEAD) )
        {
            if(!(player1.location == target.location)){
                tours1++;
            }
            if(!(player2.location == target.location)){
                tours2++;
            }
            // distribution des cartes
            int cards1[9];
            int cards2[9];
            for (int i = 0; i < 9; i++){
                cards1[i] = rand() % 7;
                cards2[i] = cards1[i];
            }
            // joueur artificiel
            if(player1.status != Robot::Status::DEAD)
                automata(g, player1, cards1, target.location);
            if(player2.status != Robot::Status::DEAD)
                joueurArt(&g, player1,target.location, cards2,b);
            //best_play(b, g, source, target.location, cards);

            for (int i = 0; i < 5; ++i)
            {
                if (!(player1.location == target.location)&& player1.status != Robot::Status::DEAD)
                    b.play(player1,(RR::Robot::Move) cards1[i]);
                if (!(player2.location == target.location)&& player2.status != Robot::Status::DEAD)
                    b.play(player2,(RR::Robot::Move) cards2[i]);
            }
        }
        std::cout << "fin de la manche player1 n*" << i << "nb_tours = " << tours1 << std::endl;
        std::cout << "fin de la manche player2 n*" << i << "nb_tours = " << tours2 << std::endl;
        if(tours1 < tours2){
            victoire1++;
        } else if(tours2 < tours1){
            victoire2++;
        }else{
            draw++;
        }
        total1+=tours1;
        total2+=tours2;
    }
    std::cout << "FIN DU TEST" << std::endl;
    double nb_tours_moyen1 = (double) total1 / 100;
    double nb_tours_moyen2 = (double) total2 / 100;
    std::cout << "Player1 " << victoire1 <<" " << draw <<" "<< victoire2 << "Player2" << std::endl;
    std::cout << "l'algo1 fait " << nb_tours_moyen1 << "tours par manche en moyenne" << std::endl;
    std::cout << "l'algo2 fait " << nb_tours_moyen2 << "tours par manche en moyenne" << std::endl;
      return 0;
}