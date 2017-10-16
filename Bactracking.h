//
// Created by varo on 15/10/17.
//

#ifndef TPII_BACTRACKING_H
#define TPII_BACTRACKING_H


#include <cstdio>

class Bactracking {   //Funca perfectamente pero solo en matrices 4x4, mayores puede
                        //presentar errores
public:

#include<stdio.h>

// Tamano matriz
#define N 5

    //bool solveMazeUtil(int maze[N][N], int x, int y, int sol[N][N]);

// Imprime la matriz
    void printSolution(int sol[N][N])
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                printf(" %d ", sol[i][j]);
            printf("\n");
        }
    }

    void evaluarSolucion(int sol[N][N]){
        int listasol[15];
        int aux = 0;
        for (int i = 0; i<N; i++){
            for (int j = 0; j<N; j++){
                if(sol[i+1][j] = 1){
                    listasol[aux]=6;
                    aux++;
                }else{
                    if(sol[i][j+1]=1){
                        listasol[aux]=8;
                        aux++;
                    }
                }
            }
        }
    }

    bool isSafe(int maze[N][N], int x, int y)
    {
        // if (x,y outside maze) return false
        if(x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1)
            return true;

        return false;
    }

    bool solveMaze(int maze[N][N])
    {
        int sol[N][N] = { {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0},
                          {0, 0, 0, 0, 0}
        };

        if(solveMazeUtil(maze, 0, 0, sol) == false)
        {
            printf("Solution doesn't exist");
            return false;
        }

        printSolution(sol);
        return true;
    }

    bool solveMazeUtil(int maze[N][N], int x, int y, int sol[N][N])
    {
        if(x == N-1 && y == N-1)
        {
            sol[x][y] = 1;
            return true;
        }

        if(isSafe(maze, x, y) == true)
        {
            sol[x][y] = 1;

            if (solveMazeUtil(maze, x+1, y, sol) == true)
                return true;

            if (solveMazeUtil(maze, x, y+1, sol) == true)
                return true;

            sol[x][y] = 0;
            return false;
        }

        return false;
    }

// Main de prueba
    int main()
    {
        int maze[N][N]  =  { {1, 0, 1, 1, 1},
                             {1, 0, 1, 0, 1},
                             {1, 1, 1, 0, 1},
                             {1, 0, 0, 0, 1},
                             {0, 0, 1, 0, 1}
        };

        solveMaze(maze);
        return 0;
    }

};


#endif //TPII_BACTRACKING_H
