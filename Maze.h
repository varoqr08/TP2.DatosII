//
// Created by varo on 19/10/17.
//

#ifndef BACKTRACKING_MAZE_H
#define BACKTRACKING_MAZE_H

#ifndef MAZE_H
#define MAZE_H

#include <iostream>
using namespace std;

static const int ROWS = 4;
static const int COLUMNS = 5;

class Maze
{
public:
    Maze();
    void printMaze();
    void writeBlock();
    void writeExit();
    void setStart( int row, int col );
    char getCell( int row, int col );
    void setCell( int row, int col, char val );
    bool backTrack( int row, int col );
private:
    char maze[ ROWS ][ COLUMNS ];


};


#endif



#endif //BACKTRACKING_MAZE_H
