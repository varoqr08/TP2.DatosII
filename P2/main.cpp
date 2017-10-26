#include "mainwindow.h"
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
const int filas=8; // vertical
static int mapa[columnas][filas];
static int visitados[columnas][filas]; // nodos que ya se probaron
static int noVisitados[columnas][filas]; // nodos que no se han probado
static int dir_map[columnas][filas];
const int dir=8;
static int dx[dir]={1, 1, 0, -1, -1, -1, 0, 1};
static int dy[dir]={0, 1, 1, 1, 0, -1, -1, -1};

class node
{
    // posicion de nodo en matriz
    int Posx;
    int Posy;
    int distancia;
    int prioridad;

public:
    node(int xp, int yp, int d, int p)
    {Posx=xp; Posy=yp; distancia=d; prioridad=p;}

    int getPosx() const {return Posx;}
    int getPosy() const {return Posy;}
    int getDistancia() const {return distancia;}
    int getPrioridad() const {return prioridad;}

    void updatePriority(const int & xDest, const int & yDest)
    {
        prioridad=distancia+estimate(xDest, yDest)*10; //A*
    }

    void nextLevel(const int & i) // i: direction
    {
        distancia+=(dir==8?(i%2==0?10:14):10);
    }

    const int & estimate(const int & xDest, const int & yDest) const
    {
        static int xd, yd, d;
        xd=xDest-Posx;
        yd=yDest-Posy;

        d=static_cast<int>(sqrt(xd*xd+yd*yd));

        // Manhattan distance
        //d=abs(xd)+abs(yd);

        // Chebyshev distance
        //d=max(abs(xd), abs(yd));

        return(d);
    }
};

bool operator<(const node & a, const node & b)
{
    return a.getPrioridad() > b.getPrioridad();
}

// A-star algorithm.
// The route returned is a string of direction digits.
string pathFind( const int & xStart, const int & yStart,
                 const int & xFinish, const int & yFinish )
{
    static priority_queue<node> cola[2];
    static int contador; // pq index
    static node* nodo1;
    static node* nodo2;
    static int i, j, x, y, w, z;//contadoresNecesarios
    static char camino;
    contador=0;

    for(y=0;y<filas;y++)
    {
        for(x=0;x<columnas;x++)
        {
            visitados[x][y]=0;
            noVisitados[x][y]=0;
        }
    }

    nodo1=new node(xStart, yStart, 0, 0);
    nodo1->updatePriority(xFinish, yFinish);
    cola[contador].push(*nodo1);
    noVisitados[xStart][yStart]=nodo1->getPrioridad();

    while(!cola[contador].empty())
    {

        nodo1=new node( cola[contador].top().getPosx(), cola[contador].top().getPosy(),
                     cola[contador].top().getDistancia(), cola[contador].top().getPrioridad());

        x=nodo1->getPosx(); y=nodo1->getPosy();

        cola[contador].pop();
        noVisitados[x][y]=0;
        visitados[x][y]=1;

        //if((*n0).estimate(xFinish, yFinish) == 0)
        if(x==xFinish && y==yFinish)
        {

            string path="";
            while(!(x==xStart && y==yStart))
            {
                j=dir_map[x][y];
                camino='0'+(j+dir/2)%dir;
                path=camino+path;
                x+=dx[j];
                y+=dy[j];
            }

            // garbage collection
            delete nodo1;
            // empty the leftover nodes
            while(!cola[contador].empty()) cola[contador].pop();
            return path;
        }

        // generate moves (child nodes) in all possible directions
        for(i=0;i<dir;i++)
        {
            w=x+dx[i]; z=y+dy[i];

            if(!(w<0 || w>columnas-1 || z<0 || z>filas-1 || mapa[w][z]==1 || mapa[w][z] == 2
                 || visitados[w][z]==1))
            {
                // generate a child node
                nodo2=new node( w, z, nodo1->getDistancia(),
                             nodo1->getPrioridad());
                nodo2->nextLevel(i);
                nodo2->updatePriority(xFinish, yFinish);

                // if it is not in the open list then add into that
                if(noVisitados[w][z]==0)
                {
                    noVisitados[w][z]=nodo2->getPrioridad();
                    cola[contador].push(*nodo2);
                    // mark its parent node direction
                    dir_map[w][z]=(i+dir/2)%dir;
                }
                else if(noVisitados[w][z]>nodo2->getPrioridad())
                {
                    // update the priority info
                    noVisitados[w][z]=nodo2->getPrioridad();
                    // update the parent direction info
                    dir_map[w][z]=(i+dir/2)%dir;

                    // replace the node
                    // by emptying one pq to the other one
                    // except the node to be replaced will be ignored
                    // and the new node will be pushed in instead
                    while(!(cola[contador].top().getPosx()==w &&
                            cola[contador].top().getPosy()==z))
                    {
                        cola[1-contador].push(cola[contador].top());
                        cola[contador].pop();
                    }
                    cola[contador].pop(); // remove the wanted node

                    // empty the larger size pq to the smaller one
                    if(cola[contador].size()>cola[1-contador].size()) contador=1-contador;
                    while(!cola[contador].empty())
                    {
                        cola[1-contador].push(cola[contador].top());
                        cola[contador].pop();
                    }
                    contador=1-contador;
                    cola[contador].push(*nodo2); // add the better node instead
                }
                delete nodo2; // garbage collection
            }
        }
        delete nodo1; // garbage collection
    }
    return ""; // no route found
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
    QPixmap q5("/home/gabriel/Proyecto2/t.png");
    p.drawPixmap(0,0,q);
    p.drawPixmap(XStart*100,YStart*100,q2);
    p.drawPixmap(XFinish*100,YFinish*100,q4);
    while(contadorX<filas){
        contadorY=0;
        while(contadorY<columnas){
            if(mapa[contadorX][contadorY]==1){
                 p.drawPixmap(contadorX*100,contadorY*100,q3);
                 contadorY+=1;
            }
            else if(mapa[contadorX][contadorY]==2){
                p.drawPixmap(contadorX*100,contadorY*100,q5);
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
void limpiarMapa(){
    /*
    int contadorX=0;
    int contadorY=0;
    while(contadorX<filas){
        contadorY=0;
        while(contadorY<columnas){
            mapa[contadorX][contadorY]=0;
            contadorY+=1;;
        }
        contadorX+=1;
    }
    */
    mapa[PosXAsteroide1][PosYAsteroide1]=0;
    mapa[PosXAsteroide2][PosYAsteroide2]=0;
    mapa[PosXAsteroide3][PosYAsteroide3]=0;
    mapa[PosXT1][PosYT1]=0;
    mapa[PosXT2][PosYT2]=0;
}
void revisarMovT(int identificador,string direccion){
    if(direccion.compare("0")==0){
        if(identificador == 1){
            PosXT1+=1;
        }else{
            PosXT2+=1;
        }
    }
    else if(direccion.compare("1")==0){
        if(identificador == 1){
            PosXT1+=1;
            PosYT1+=1;
        }else{
            PosXT2+=1;
            PosYT2+=1;
        }
        //d/f
    }
    else if(direccion.compare("2")==0){
        if(identificador == 1){
            PosYT1+=1;
        }else{
            PosYT2+=1;
        }
        //abajo
    }
    else if(direccion.compare("3")==0){
        if(identificador == 1){
            PosYT1+=1;
            PosXT1-=1;
        }else{
            PosYT2+=1;
            PosXT2-=1;
        }

        //d/b
    }
    else if(direccion.compare("4")==0){
        if(identificador == 1){
            PosXT1-=1;
        }else{
            PosXT2-=1;
        }
        //izquierda
    }
    else if(direccion.compare("5")==0){
        if(identificador == 1){
            PosYT1-=1;
            PosXT1-=1;
        }else{
            PosYT2-=1;
            PosXT2-=1;
        }
        //u/b
    }
    else if(direccion.compare("6")==0){
        if(identificador == 1){
           PosYT1-=1;
        }else{
           PosYT2-=1;
        }
        PosYT1-=1;
        //arriba
    }
    else if(direccion.compare("7")==0){
        if(identificador == 1){
            PosXT1+=1;
            PosYT1-=1;
        }else{
            PosXT2+=1;
            PosYT2-=1;
        }
        //u/f
    }
}
void moverAsteroide(int identificador){
    int mover = rand() % 100 + 1;
    if(mover>50){
        int direccion = rand()%40+1;
        if(direccion<=10&&direccion>0){
            if(identificador==1){
                if(PosYAsteroide1-1==PosYNave||PosYAsteroide1-1==PosYFinal){

                }

                else{
                    PosYAsteroide1-=1;
                }
            }
            else if(identificador==2){
                if(PosYAsteroide2-1==PosYNave||PosYAsteroide2-1==PosYFinal){

                }

                else{
                    PosYAsteroide2-=1;
                }
            }
            else{
                if(PosYAsteroide3-1==PosYNave||PosYAsteroide3-1==PosYFinal){

                }

                else{
                    PosYAsteroide3-=1;
                }
            }
            //arriba
        }
        else if(direccion<=20&&direccion>10) {
            if(identificador==1){
                if(PosYAsteroide1+1==PosYNave||PosYAsteroide1+1==PosYFinal||mapa[PosXAsteroide1][PosYAsteroide1+1]!=0){

                }

                else{
                    PosYAsteroide1+=1;
                }
            }
            else if(identificador==2){
                if(PosYAsteroide2+1==PosYNave||PosYAsteroide2+1==PosYFinal||mapa[PosXAsteroide1][PosYAsteroide2+1]!=0){

                }

                else{
                    PosYAsteroide2+=1;
                }
            }
            else{
                if(PosYAsteroide3+1==PosYNave||PosYAsteroide3+1==PosYFinal||mapa[PosXAsteroide3][PosYAsteroide3+1]!=0){

                }

                else{
                    PosYAsteroide3+=1;
                }
            }
            //abajo
        }
        else if(direccion<=30&&direccion>20) {
            if(identificador==1){
                if(PosXAsteroide1+1==PosYNave||PosXAsteroide1+1==PosXFinal||mapa[PosXAsteroide1+1][PosYAsteroide1]!=0){

                }
                else{
                    PosXAsteroide1+=1;
                }
            }
            else if(identificador==2){
                if(PosXAsteroide2+1==PosYNave||PosXAsteroide2+1==PosXFinal||mapa[PosXAsteroide2+1][PosYAsteroide2]!=0){

                }
                else{
                    PosXAsteroide2+=1;
                }
            }
            else{
                if(PosXAsteroide2+1==PosYNave||PosYAsteroide3+1==PosXFinal||mapa[PosXAsteroide3+1][PosYAsteroide3]!=0){

                }
                else{
                    PosXAsteroide3+=1;
                }
            }
            //derecha
        }
        else if(direccion<=40&&direccion>30) {
            if(identificador==1){
                if(PosXAsteroide1-1==PosYNave||PosYAsteroide1-1==PosYFinal||mapa[PosXAsteroide1-1][PosYAsteroide1]!=0){

                }
                else{
                    PosXAsteroide1-=1;
                }
            }
            else if(identificador==2){
                if(PosXAsteroide2-1==PosYNave||PosXAsteroide2-1==PosXFinal||mapa[PosXAsteroide2-1][PosYAsteroide2]!=0){

                }
                else{
                    PosXAsteroide2-=1;
                }
            }
            else{
                if(PosXAsteroide3-1==PosYNave||PosXAsteroide3-1==PosXFinal||mapa[PosXAsteroide3-1][PosYAsteroide3]!=0){

                }

                else{
                    PosXAsteroide3-=1;
                }
            }
            //izquierda
        }
    }
}
int main(int argc, char *argv[])
{
   mapa[PosXAsteroide1][PosYAsteroide1]=1;
   mapa[PosXAsteroide2][PosYAsteroide2]=1;
   mapa[PosXAsteroide3][PosYAsteroide3]=1;
   mapa[PosXT1][PosYT1]=2;
   mapa[PosXT2][PosYT2]=2;
   crearMatriz(argc,argv,PosXNave,PosYNave,PosXFinal,PosYFinal);
   string caminoT1;
   string proximaCeldaT1;
   string caminoT2;
   string proximaCeldaT2;
   string caminoNave = pathFind(PosXNave,PosYNave,PosXFinal,PosYFinal);
   string proximaCeldaNave = caminoNave.substr(0,1);
   cout<<caminoNave<<endl;
   cout<<proximaCeldaNave<<endl;
   while(PosXNave!=PosXFinal||PosYNave!=PosYFinal){
       if(proximaCeldaNave.compare("0")==0){
           PosXNave+=1;
           //derecha
       }
       else if(proximaCeldaNave.compare("1")==0){
           PosXNave+=1;
           PosYNave+=1;
           //d/f
       }
       else if(proximaCeldaNave.compare("2")==0){
           PosYNave+=1;
           //abajo
       }
       else if(proximaCeldaNave.compare("3")==0){
           PosYNave+=1;
           PosXNave-=1;
           //d/b
       }
       else if(proximaCeldaNave.compare("4")==0){
           PosXNave-=1;
           //izquierda
       }
       else if(proximaCeldaNave.compare("5")==0){
           PosYNave-=1;
           PosXNave-=1;
           //u/b
       }
       else if(proximaCeldaNave.compare("6")==0){
           PosYNave-=1;
           //arriba
       }
       else if(proximaCeldaNave.compare("7")==0){
           PosXNave+=1;
           PosYNave-=1;
           //u/f
       }
       crearMatriz(argc,argv,PosXNave,PosYNave,5,6);
       caminoNave = pathFind(PosXNave,PosYNave,PosXFinal,PosYFinal);
       limpiarMapa();
       moverAsteroide(1);
       moverAsteroide(2);
       moverAsteroide(3);
       mapa[PosXAsteroide1][PosYAsteroide1]=1;
       mapa[PosXAsteroide2][PosYAsteroide2]=1;
       mapa[PosXAsteroide3][PosYAsteroide3]=1;
       caminoT1 = pathFind(PosXT1,PosYT1,PosXNave,PosYNave);
       proximaCeldaT1= caminoT1.substr(0,1);
       revisarMovT(1,proximaCeldaT1);
       mapa[PosXT1][PosYT1]=2;
       caminoT2 =pathFind(PosXT2,PosYT2,PosXNave,PosYNave);
       proximaCeldaT2 = caminoT2.substr(0,1);
       revisarMovT(2,proximaCeldaT2);
       mapa[PosXT2][PosYT2]=2;
       proximaCeldaNave = caminoNave.substr(0,1);
   }
}
