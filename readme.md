Escriva William
Gomez Malaurie
# Projet Roborally

Ce projet vise à créer un joueur automatique pour le jeu [Roborally](https://fr.wikipedia.org/wiki/RoboRally). 
Le sujet est disponible en pdf [sur la page du cours](https://liris.cnrs.fr/vincent.nivoliers/lifap6/Supports/Projet/roborally.pdf).


## Organisation des dossiers fichiers
* board.hpp et board.cpp

	Code contenant les classes Robot et Board. 

* graph.hpp et graph.cpp -> Graph (le constructeur) 

	Contient le code qui concerne la structure de données permettant de stocker le graphe. Ce conctructeur initialise un vecteur à 3 dimensions (ligne, colonne, orientation) dont chaque "case" pointe sur un tableau de taille 7 (pour les 7 actions possibles).

* graphs.hpp et graph.cpp -> dijkstraBonSens (fonction) 

	Contient le code qui concerne la recherche du plus court chemin dans le graphe est contenu. Cette fonction retourne un vecteur contenant des eniers correspondants aux actions à effectuer.
	La fonction dijkstra (dans les mêmes fichiers) fait la même chose, mais retourne le vecteur à l'envers (c'est à dire, les actions à effectuer en premiers sont à la fin du vecteur).

* automata.cpp -> automata (fonction) et joueurArt.cpp -> joueurArt (fonction)

	Contiennent respectivement le code du premier et du deuxième joueur artificiel. Nous en avons fait deux afin de pouvoir les mettre en compétition. Ces fonctions prennent notamment un tableau de 9 cartes en paramètre. Celui-ci sera réarrangé afin que les actions choisies figurent dans l'ordre au début du tableau.

* automata.cpp -> testAutomata (fonction) et joueurArt.cpp -> testJoueurArt (fonction)

	Contiennent un code similaire qui permet de tester les fonctions automata et joueurArt. Lancent respectivement 100 et 5 recherches de chemins dont la case de début et celle d'arrivée sont aléatoires. Le nombre de chemins qui ont pu être résolus vs le nombre de chemins qui ont résultés en un décès sont envoyés sur la sortie standart pour afichage.

* joueurArt.cpp -> tirageCartes (fonction)

	Fonction qui prend en paramètre un tableau, et le remplit avec un tirage de cartes aléatoire qui tient compte de la rareté des actions (prend les proportions des cartes du jeu original).

* duel.cpp 

	Permet la mise en compétitions des robots (automata et joueurArt). 100 manches sont lancées. Le nombre moyen de tours par manche et le temps d'éxécution moyen pour arriver à l'objectif y sont calculés et envoyés dans la sortie standart pour affichage.

* board.txt et big_board.txt

	Petit et grand plateau


## Compilation / Exécution
* make

	Compilation du projet complet

* Src/test_board

	Execution de testBoard.cpp (appelle dijkstra et les fonction de test de automata et joueurArt)

* Src/duel

	Execution du duel (compétition de robots, issu de duel.cpp)
	
* make clean

	Suppression des éxécutables et fichiers objets


