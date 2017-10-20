#include <iostream>
#include "Maze.h"
using namespace std;

int main()
{

    bool setFlag = false;
    int row;
    int col;
    Maze maze1;

    maze1.printMaze();


    while ( setFlag == false )
    {
        cout << "Incerte la posicion de inicio.\n"
             << "Incerte la fila con un numero de 0 a " << (ROWS - 1) << endl;
        cin >> row;
        cout << "Incerte la columna con un numero de 0 a " << (COLUMNS - 1) << endl;
        cin >> col;
        cout << endl;
        cout << "El lugar de salida es: " << row << ", " << col << endl;
        if ( maze1.getCell( row, col ) != '+' && maze1.getCell( row, col ) != 'E' )
        {
            maze1.setStart( row, col );
            setFlag = true;
        }
        else
        {
            cout << "Posicion invalida, intente de nuevo." << endl;
            cout << endl;
        }
    }

    cout << endl;
    maze1.printMaze();
    cout << "Realizando backTracking..." << endl;
    cout << endl;
    if ( maze1.backTrack( row, col ) == true )
    {
        cout << "Terminado!" << endl;
    }
    else
    {
        cout << "No hay recorrido posible." << endl;
    }
    maze1.printMaze();

}