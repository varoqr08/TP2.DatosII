#include "mainwindow.h"
#include <QApplication>
#include <QApplication>
#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>
#include <QHBoxLayout>
#include <QWidget>
#include <iostream>
#include <iomanip>
#include <queue>
#include <string>
#include <math.h>
#include <ctime>
#include "maze.h"
using namespace std;
int PosXNave=0;
int PosYNave=4;
int PosXFinal=5;
int PosYFinal=6;
int PosXT1=6;
int PosYT1=1;
int PosXT2=6;
int PosYT2=4;
int PosXAsteroide1=3;
int PosYAsteroide1=4;
int PosXAsteroide2=5;
int PosYAsteroide2=4;
int PosXAsteroide3=3;
int PosYAsteroide3=5;
const int columnas=8; // tamaño horizontal del mapa
const int filas=8;
int direcciones[100]={0};
int contadorDirecciones=0;

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
Maze mazeMapa;
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
       char block = '+';
       maze[2][0] = block;
       maze[1][1]= block;
       maze[1][3]= block;
       maze[0][3]= block;
       maze[3][3]= block;
       maze[4][5]= block;
       maze[4][1]= block;
       maze[4][2]= block;
       maze[6][1]= block;
       maze[7][6]= block;
       maze[4][3]= block;
       maze[5][6]= block;
       maze[7][6]= block;
       maze[2][5]= block;
       maze[3][7]= block;
}

void Maze::writeExit()
{
    maze[6][5] = 'F';
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
        //cout << "Celda " << row << ", " << col << " esta fuera de la matriz!" << endl;
        return false;
    }
    else if ( getCell( row, col ) == '+' )
    {
        //cout << "Celda encontrada: " << row << ", " << col << endl;
        return false;
    }
    else if ( getCell( row, col ) == 'X' )
    {
        //cout << "Celda visitada: " << row << ", " << col << endl;
        return false;
    }

    else if ( getCell( row, col ) == 'F' )
    {
        //cout << "Salida encontrada" << endl;
        return true;
    }
    else
    {
        setCell( row, col, 'X' ); //marca celdas vistadas
        //busca en las celdas adyacentes pero solo si ya no se alcanzo el final
        if ( backTrack( row, (col + 1) ) == true )
        {
            cout<<6<<endl;
            direcciones[contadorDirecciones]=6;
            contadorDirecciones+=1;
            return true;
        }
        else if (  backTrack( (row + 1), col ) == true )
        {
            cout<<8<<endl;
            direcciones[contadorDirecciones]=8;
            contadorDirecciones+=1;
            return true;
        }
        else if ( backTrack( row, (col - 1) ) == true  )
        {
            cout<<4<<endl;
            direcciones[contadorDirecciones]=4;
            contadorDirecciones+=1;
            return true;
        }
        else if ( backTrack( (row - 1), col ) == true  )
        {
            cout<<2<<endl;
            direcciones[contadorDirecciones]=2;
            contadorDirecciones+=1;
            return true;
        }
        else{
            return false;
        }

    }

}

void crearMatriz(int argc, char *argv[],int XStart,int YStart,int XFinish,int YFinish){
    QApplication a(argc, argv);
    int contadorX=0;
    int contadorY=0;
    QLabel l;
    QLabel l2;
    QPicture pi;
    QPainter p(&pi);
    QPixmap q("/home/gabriel/Proyecto2/s.jpg");
    QPixmap q2("/home/gabriel/Proyecto2/Falcon.png");
    QPixmap q3("/home/gabriel/Proyecto2/asteroide.png");
    QPixmap q4("/home/gabriel/Proyecto2/rebelBase.jpg");
    p.drawPixmap(0,0,q);
    p.drawPixmap(XStart*100,YStart*100,q2);
    p.drawPixmap(XFinish*100,YFinish*100,q4);
    while(contadorX<filas){
        contadorY=0;
        while(contadorY<columnas){
            if(mazeMapa.getCell(contadorX,contadorY)=='+'){
                 p.drawPixmap(contadorY*100,contadorX*100,q3);
                 contadorY+=1;
            }
            else{
                contadorY+=1;
            }

        }
        contadorX+=1;
    }
    int posXInicial=100;
    int posYInicial=100;
    p.setRenderHint(QPainter::Antialiasing);
    p.drawLine(0,800,0,0);
    p.drawLine(800,0,0,0);
    while(posXInicial<=800){
        p.drawLine(800,posXInicial,0,posXInicial);
        posXInicial+=100;
    }
    while(posYInicial<=800){
        p.drawLine(posYInicial,800,posYInicial,0);
        posYInicial+=100;
    }
    p.end();
    l.setPicture(pi);
    l2.setPixmap(q);
    l.show();
    a.exec();
}
void imprimirDirecciones(){
    int x=0;
    while(direcciones[x]!=0){
        cout<<direcciones[x];
        x+=1;
    }
}

int main(int argc, char *argv[])
{
    cout<<direcciones[0]<<endl;
    mazeMapa.setStart(PosYNave,PosXNave);
    mazeMapa.backTrack(PosYNave,PosXNave);
    crearMatriz(argc,argv,PosXNave,PosYNave,PosXFinal,PosYFinal);
    int recorrer=contadorDirecciones-1;
    while(PosXNave!=PosXFinal||PosYNave!=PosYFinal){
            if(direcciones[recorrer]==6){
                PosXNave+=1;
                //derecha
            }
            else if(direcciones[recorrer]==2){
                PosYNave+=1;
                //abajo
            }
            else if(direcciones[recorrer]==4){
                PosXNave-=1;
                //izquierda
            }
            else if(direcciones[recorrer]==8){
                PosYNave+=1;
                //arriba
            }
            crearMatriz(argc,argv,PosXNave,PosYNave,PosXFinal,PosYFinal);
            recorrer-=1;
        }

}
