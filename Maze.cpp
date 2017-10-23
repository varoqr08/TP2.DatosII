//
// Created by varo on 19/10/17.
//

#include "Maze.h"
#include <iostream>
#include "Maze.h"
using namespace std;

Maze::Maze() //constructor
{
    char def = 'O';
    for ( int i = 0; i < ROWS; i++ )
    {
        for (int j = 0; j < COLUMNS; j++ )
        {
            maze[ i ][ j ] = def;
        }
    }
    writeBlock();
    writeExit();
}
void imprimirAux(int x)
{
    cout << x <<endl;
}

void Maze::printMaze()
{
    for ( int i = 0; i < ROWS; i++ )
    {
        cout << "+---+---+---+---+---+\n";

        for ( int j = 0; j < COLUMNS; j++ )
        {
            cout << "| " << maze[ i ][ j ] << " ";
        }
        cout << "|" << endl;
    }
    cout << "+---+---+---+---+---+" << endl;
}


void Maze::writeBlock()
{
    int x = 1;
    int y = 4;
    char block = '+';
    maze[ x ][ y ] = block; //set block at 1,4
    maze[ ++x ][ y ] = block; //set block at 2,4
    maze[ ++x ][ y ] = block; //set block at 3,4
    --y; //y = 2
    maze[ x ][ --y ] = block; //set block at 3,1
    maze[ x ][ --y ] = block; //set block at 3,0
    --x; //x = 2
    maze[ --x ][ ++y ] = block; //set block at 1,1
    maze[ --x ][ ++y ] = block; //set block at 0,2
    maze[6][7] = block;
    maze[1][6]= block;
    maze[4][7]= block;
    maze[7][5]= block;
    maze[5][5]= block;
    maze[6][5]= block;
    maze[5][3]= block;
    maze[4][2]= block;
    maze[6][1]= block;
    maze[7][2]= block;
}

void Maze::writeExit()
{
    maze[ 5 ][ 6 ] = 'F'; //set exit at 0,3
}

//Se escoge la posicion de inicio
void Maze::setStart( int row, int col )
{
    setCell( row, col, 'C' );
}

char Maze::getCell( int row, int col )
{
    char cell = maze[ row ][ col ];
    return cell;
}

void Maze::setCell( int row, int col, char val )
{
    maze[ row ][ col ] = val;
}

bool Maze::backTrack( int row, int col )
{

    if ( row < 0 || col < 0 || row >= ROWS || col >= COLUMNS ) //search off the maze
    {
        cout << "Celda " << row << ", " << col << " esta fuera de la matriz!" << endl;
        return false;
    }
    else if ( getCell( row, col ) == '+' )
    {
        cout << "Celda encontrada: " << row << ", " << col << endl;
        return false;
    }
    else if ( getCell( row, col ) == 'X' )
    {
        cout << "Celda visitada: " << row << ", " << col << endl;
        return false;
    }

    else if ( getCell( row, col ) == 'F' )
    {
        cout << "Salida encontrada" << endl;
        return true;
    }
    else
    {
        setCell( row, col, 'X' ); //marca celdas vistadas
        //busca en las celdas adyacentes pero solo si ya no se alcanzo el final

        if ( backTrack( row, (col + 1) ) == true )
        {
            imprimirAux(6);
            return true;
        }else if ( backTrack( (row + 1), col ) == true ) {
            imprimirAux(8);
            return true;
        }
        else if ( backTrack( row, (col - 1) ) == true )
        {
            imprimirAux(4);
            return true;
        }
        else if ( backTrack( (row - 1), col ) == true )
        {
            imprimirAux(2);
            return true;
        }
        else
        {
            return false;
        }

    }

}
