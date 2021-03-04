#include "board.hpp"

#include <fstream>
#include <iostream>
#include <bitset>

namespace RR
{

  /**** Enumerations ****/

  enum struct Direction
  {
    EAST,
    NORTH,
    WEST,
    SOUTH,
    NONE
  };

  enum struct Rotation
  {
    RIGHT,
    LEFT,
    NONE
  };

  /* strongly typed enum manipulation */
  /* https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int */
  template <typename T>
  constexpr auto operator+(T e) noexcept
      -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>>
  {
    return static_cast<std::underlying_type_t<T>>(e);
  }

  /**** Location ****/

  Location::Location(int l, int c) : line(l), column(c) {}

  bool Location::operator==(const Location &rhs) const
  {
    return (line == rhs.line) && (column == rhs.column);
  }

  std::size_t LocationHash::operator()(const Location &l) const
  {
    if (sizeof(long long) == 2 * sizeof(int))
    {
      //this supposedly always happens
      long long concat = l.line;
      concat <<= 8 * sizeof(int);
      concat += l.column;
      return std::hash<long long>()(concat);
    }
    else
    {
      //just in case
      std::bitset<16 * sizeof(int)> concat;
      concat |= l.line;
      concat <<= 8 * sizeof(int);
      concat |= l.column;
      return std::hash<std::bitset<16 * sizeof(int)>>()(concat);
    }
  }

  /**** Tile manipulation ****/

  /* Check whether a tile is a fast belt */
  static bool tile_is_fast(Board::TileType type)
  {
    return type > Board::TileType::JOIN_EW_TO_S &&
           type < Board::TileType::ROTATE_RIGHT;
  }

  /* Check the direction in which the tiles moves the robot if any */
  static Direction tile_direction(Board::TileType type)
  {
    /* rotating tile and regular tiles have no direction */
    if (type > Board::TileType::FAST_JOIN_EW_TO_S)
      return Direction::NONE;

    /* fast belts are ordered the same way as regular belts */
    int base_type = +type % 24;

    /* non rotating belts, enum conversion is straightforward */
    if (base_type < +Board::TileType::TURN_SE)
      return (Direction) + base_type;

    /* conversion table for turns and perpendicular joins */
    static const Direction directions[8] =
        {
            Direction::EAST,
            Direction::WEST,
            Direction::NORTH,
            Direction::NORTH,
            Direction::WEST,
            Direction::EAST,
            Direction::SOUTH,
            Direction::SOUTH};

    /* turns and perpendicular joins */
    if (base_type < +Board::TileType::JOIN_NS_TO_E)
      return directions[(base_type - +Board::TileType::TURN_SE) % 8];

    /* opposite joins, enum conversion is straightforward */
    return (Direction)(base_type - +Board::TileType::JOIN_NS_TO_E);
  }

  static Rotation tile_rotation(Board::TileType type, Direction direction 
  /* some tiles depend on the incoming direction */
  )
  {
    /* static tiles */
    if (type == Board::TileType::NOTHING)
      return Rotation::NONE;

    /* simple rotating tiles */
    if (type > Board::TileType::FAST_JOIN_EW_TO_S)
      return (Rotation)((+type - +Board::TileType::ROTATE_RIGHT) % 2);

    /* fast belts are ordered the same way as regular belts */
    int base_type = +type % 24;

    /* non rotating belts */
    if (base_type < +Board::TileType::TURN_SE)
      return Rotation::NONE;

    /* simple rotating belts */
    if (base_type < +Board::TileType::JOIN_S_TO_E)
      return (Rotation)((base_type - +Board::TileType::TURN_SE) % 2);

    /* joins depend on the incoming direction */
    if (direction == Direction::NONE)
      return Rotation::NONE;

    /* constraint table for perpendicular joins */
    static const Direction perp_directions[8] =
        {
            Direction::SOUTH,
            Direction::SOUTH,
            Direction::EAST,
            Direction::WEST,
            Direction::NORTH,
            Direction::NORTH,
            Direction::WEST,
            Direction::EAST};

    /* perpendicular joins */
    if (base_type < +Board::TileType::JOIN_NS_TO_E)
    {
      if (direction == perp_directions[base_type - +Board::TileType::JOIN_S_TO_E])
      {
        return (Rotation)(base_type % 2);
      }
      else
      {
        return Rotation::NONE;
      }
    }

    /* opposite joins */
    if ((base_type - +Board::TileType::JOIN_NS_TO_E) % 2 != +direction % 2)
    {
      /* this will be hard to figure in a month */
      /* (five years later) indeed, I'll trust myself on this */
      return (Rotation)((
                            +direction / 2 +
                            +direction % 2 +
                            (base_type - +Board::TileType::JOIN_NS_TO_E) / 2) %
                        2);
    }
    return Rotation::NONE;
  }

  /**** Robot ****/

  /** Moving **/

  /* move the robot in a direction */
  static void robot_push(Robot &robot, Direction direction)
  {
    /* a dead robot does not move */
    if (robot.status == Robot::Status::DEAD)
      return;

    /* no direction, no chocolate */
    if (direction == Direction::NONE)
      return;

    /* good luck with that */
    /* (five years later) yeah sure */
    robot.location.line += (+direction % 2) * (+direction - 2);
    robot.location.column += ((1 + +direction) % 2) * (1 - +direction);
  }

  /* rotate robot in a direction */
  static void robot_rotate(Robot &robot, Rotation rotation)
  {
    /* a dead robot does not rotate */
    if (robot.status == Robot::Status::DEAD)
      return;

    /* no rotation, no chocolate */
    if (rotation == Rotation::NONE)
      return;

    /* enum conversion is straightforward */
    if (rotation == Rotation::LEFT)
    {
      robot.status = (Robot::Status)((+robot.status + 1) % 4);
    }
    else
    {
      robot.status = (Robot::Status)((+robot.status + 3) % 4);
    }
  }

  /* apply one board tile to the robot */
  static void robot_apply(const Board &board, Robot &robot)
  {
    try
    {
      /* find the tile the robot is on, throws if does not exist */
      Board::TileType tile = board.tiles.at(robot.location);

      /* direction of the robot tile */
      Direction dir = tile_direction(tile);

      if (dir == Direction::NONE)
      {
        /* rotate the robot if necessary */
        robot_rotate(robot, tile_rotation(tile, Direction::NONE));
      }

      /* the robot is on a belt, move it */
      robot_push(robot, dir);

      /* get the arrival tile, throws if does not exist */
      Board::TileType dest_tile = board.tiles.at(robot.location);

      /* handle the rotation of the destination tile */
      robot_rotate(robot, tile_rotation(dest_tile, (Direction)((+dir + 2) % 4)));
    }
    catch (std::out_of_range &e)
    {
      /* the robot is in the void */
      robot.status = Robot::Status::DEAD;
    }
  }

  /**** Public interface ****/

  /** Construction **/

  Board::Board(const std::string &filename)
  {
    /* open board file */
    std::ifstream board_file;
    board_file.exceptions(std::ifstream::badbit);
    board_file.open(filename);

    /* read the board */
    while (true)
    {
      Location spot;
      int type;
      board_file >> spot.line >> spot.column >> type;

      if (!board_file.eof())
      {
        tiles[spot] = (TileType)type;
      }
      else
      {
        break;
      }
    }
  }

  void Board::save(const std::string &filename) const
  {
    /* open the file for writing */
    std::ofstream output;
    output.exceptions(std::ofstream::badbit);
    output.open(filename);

    for (std::pair<Location, TileType> tile : tiles)
    {
      output << tile.first.line
             << " " << tile.first.column
             << " " << +tile.second
             << std::endl;
    }
  }

  void Board::json(std::ostream &output) const
  {
    /* open the file for writing */
    output << "[";
    bool first = true;
    for (std::pair<Location, TileType> tile : tiles)
    {
      if (!first)
      {
        output << ", ";
      }
      output << "{ "
             << "\"line\" : " << tile.first.line << ","
             << "\"column\" : " << tile.first.column << ","
             << "\"type\" : " << +tile.second
             << " }";
      first = false;
    }
    output << "]";
  }

  /** Robot moving **/

  void Board::play(Robot &robot, Robot::Move move) const
  {
    /* fast exit for dead robots */
    if (robot.status == Robot::Status::DEAD)
      return;

    /* determine the varying coordinate */
    switch (move)
    {
    case Robot::Move::FORWARD_1:
      robot_push(robot, (Direction)robot.status);
      break;
    case Robot::Move::FORWARD_2:
      robot_push(robot, (Direction)robot.status);
      robot_push(robot, (Direction)robot.status);
      break;
    case Robot::Move::FORWARD_3:
      robot_push(robot, (Direction)robot.status);
      robot_push(robot, (Direction)robot.status);
      robot_push(robot, (Direction)robot.status);
      break;
    case Robot::Move::BACKWARD_1:
      robot_push(robot, (Direction)((+robot.status + 2) % 4));
      break;
    case Robot::Move::TURN_LEFT:
      robot_rotate(robot, Rotation::LEFT);
      break;
    case Robot::Move::TURN_RIGHT:
      robot_rotate(robot, Rotation::RIGHT);
      break;
    case Robot::Move::U_TURN:
      robot_rotate(robot, Rotation::RIGHT);
      robot_rotate(robot, Rotation::RIGHT);
      break;
    }

    try
    {
      /* apply the tile to the robot */
      Board::TileType tile = tiles.at(robot.location);
      robot_apply(*this, robot);
      Board::TileType dest_tile = tiles.at(robot.location);

      /* handle fast belts */
      if (tile_is_fast(tile) && tile_is_fast(dest_tile))
      {
        robot_apply(*this, robot);
      }
    }
    catch (std::out_of_range &e)
    {
      robot.status = Robot::Status::DEAD;
      return;
    }
  }

  Cellule::Cellule(Robot::Status s, Location l) : stat(s), loc(l){}

  Graph::Graph(unsigned int x_, unsigned int y_, std::string b): x(x_), y(y_), str_board(b) {
    dead = nullptr;
    //initialisation du tableau (les cases)
    Board board(str_board);
    int numL = 0;
    int numC = 0;
    std::vector<std::vector<Cellule*>> v1;
    std::vector<Cellule*> v2;
    v2.clear();
    Location spot;
    std::ifstream file("board.txt");
    
    //on parcours le fichier du plateau pour créer le tableau avec les bonnes dimensions
    while (!file.eof())
    {
      int type;
      file >> spot.line >> spot.column >> type;
      if (spot.line != numL) {
        numL = spot.line;
        numC=0; //num de colonne
        tabCell.push_back(v1);
        v1.clear();
      }
      while (numC<spot.column){
        v1.push_back(v2); //on met un vector vide pour que la prochaine cellule soient accessible avec les bons coordonnés
        numC++;
      }
      Location l(spot.line, spot.column);
      v2.push_back(new Cellule(Robot::Status::EAST, l));
      v2.push_back(new Cellule(Robot::Status::NORTH, l));
      v2.push_back(new Cellule(Robot::Status::WEST, l));
      v2.push_back(new Cellule(Robot::Status::SOUTH, l));
      v1.push_back(v2);
      v2.clear();
      numC++;
    }
    tabCell.push_back(v1);
    file.close();

    //on fait les aretes
    Robot::Move tabMoves[7] = {Robot::Move::FORWARD_1,
                              Robot::Move::FORWARD_2,
                              Robot::Move::FORWARD_3,
                              Robot::Move::BACKWARD_1,
                              Robot::Move::TURN_LEFT,
                              Robot::Move::TURN_RIGHT,
                              Robot::Move::U_TURN };
    RR::Robot rob;
    for (int i = 0; i<tabCell.size(); i++){
      for (int j = 0; j<tabCell[i].size(); j++){
        if (!tabCell[i][j].empty()){
          for(int k = 0; k<4; k++){
            Robot::Status s;
            switch(k){
            case 0 : s = Robot::Status::EAST; break;
            case 1 : s = Robot::Status::NORTH; break;
            case 2 : s = Robot::Status::WEST; break;
            case 3 : s = Robot::Status::SOUTH; break;
            default: break;
            } 

            for (int l=0; l<7; l++){
              rob = {{i, j}, s} ;
              board.play(rob, tabMoves[l]);
              switch (rob.status){
                case Robot::Status::DEAD : 
                  tabCell[i][j][k]->tab.push_back(dead); break;
                case Robot::Status::EAST : 
                  tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][0]); break;
                case Robot::Status::NORTH : 
                  tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][1]); break;
                case Robot::Status::WEST : 
                  tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][2]); break;
                case Robot::Status::SOUTH : 
                  tabCell[i][j][k]->tab.push_back(tabCell[rob.location.line][rob.location.column][3]); break;
                default : break;
              }
            }
          }
        }
      }
    }
  }

  void Graph::afficher() const{
    for (unsigned int i = 0; i<tabCell.size(); i++){
      for (unsigned int j = 0; j<tabCell[i].size(); j++){
        if (!tabCell[i][j].empty()){
          for(unsigned int k = 0; k<4; k++){
            std::cout<<i<<" "<<j<<" "<<k<<std::endl;
            std::cout<<tabCell[i][j][k]->tab.size()<<std::endl;
            std::cout<<std::endl;
          }
        }
      }
      std::cout<<std::endl;
    }

    /*std::cout<<"ex: "<<std::endl;
    for (unsigned int i = 0; i<tabCell[4][0][1]->tab.size(); i++){
      std::cout<<i<<std::endl;
      if (tabCell[4][0][1]->tab[i] == dead) std::cout<<"=="<<std::endl;
    }*/
  }

  Graph::~Graph(){
  for (unsigned int i = 0; i<tabCell.size(); i++){
    for (unsigned int j = 0; j<tabCell[i].size(); j++){
      if (!tabCell[i][j].empty()){
        for(unsigned int k = 0; k<4; k++){
          delete tabCell[i][j][k];
        }
      }
    }
  }
}

} //end of namespace RR
