#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int arrayResoluciones[50];
Mat arrayIndividuos[1000];
int contadorInsertarIndividuos=100;
int limiteGeneraciones = 0;
int cantDivisiones;
VideoCapture capture;
int revisarRepeticion(Mat imagen,Mat rect);
class Individuos{
public:

    int fitness=-1;
    Mat imagen;
    int posFitness[100]={0};
    int cantidadPos=0;
};
Individuos arrayFitness[1000];
Individuos arrayNuevos[100];
Mat arraryRects(Mat imagen, const int cantDivisiones) {
    double minVal;double fitness;Point minLoc;Point maxLoc;
    Mat imagenNueva(imagen.rows, imagen.cols, imagen.type());
    int contador = 0;
    int espacio = imagen.rows/(sqrt(cantDivisiones));
    Mat partes[cantDivisiones];
    int cortarX = 0;
    int cortarY = 0;
    int rows = imagen.rows;
    int cols = imagen.cols;
    while (rows > 0) {
        cols= imagen.cols;
        cortarX = 0;
        while (cols > 0) {
            Mat imagenCortada = imagen(Rect(cortarX,cortarY,espacio,espacio));
            partes[contador] = imagenCortada;
            contador++;
            cortarX += espacio;
            cols -= espacio;
        }
        cortarY += espacio;
        rows -= espacio;
    }
    Mat almacenador(1,1,1);
    matchTemplate(partes[0],partes[0],almacenador,CV_TM_CCORR_NORMED);
    minMaxLoc(almacenador,&minVal,&fitness,&minLoc,&maxLoc,Mat());;
    //cout<<fitness;
    random_shuffle(&partes[0],&partes[cantDivisiones-1]);
    //revisarRepeticion(imagen,partes[0]);
    rows = imagen.rows;
    cols = imagen.cols;
    int contadorInsertar = 0;
    cortarX = 0;
    cortarY = 0;
    while(rows>0){
        cols= imagen.cols;
        cortarX = 0;
        while (cols > 0) {
            partes[contadorInsertar].copyTo(imagenNueva(Rect(cortarX,cortarY,espacio,espacio)));
            //revisarRepeticion(imagen,imagenNueva(Rect(cortarX,cortarY,espacio,espacio)));
            contadorInsertar+=1;
            contador++;
            cortarX += espacio;
            cols -= espacio;
        }
        cortarY += espacio;
        rows -= espacio;
    }
    return imagenNueva;
}

int revisarDivision(int resolucion,int cantPiezas){
    int contador=0;
    while(arrayResoluciones[contador]!=7896){
        if(resolucion==arrayResoluciones[contador]){
            while(arrayResoluciones[contador]!=7896){
                if(arrayResoluciones[contador]==0){
                    break;
                }
                else if(arrayResoluciones[contador]==cantPiezas){
                    return cantPiezas;
                }
                else{
                    contador+=1;
                }
            }
        }
        else{
            contador+=1;
        }
    }
}
Individuos fitness2(Mat imagenOriginal,Mat imagenComparar){
    Individuos individuo;
    int espacio = imagenOriginal.rows/(sqrt(cantDivisiones));
    individuo.imagen=imagenComparar;
    int contadorFitness=0;
    int cortarImagenX=0;
    int cortarImagenY=0;
    Mat almacenador(1,1,1);
    int fitnessTotal=0;
    double minVal;double fitness;Point minLoc;Point maxLoc;
    while(cortarImagenY<imagenOriginal.rows){
        cortarImagenX=0;
        while(cortarImagenX<imagenOriginal.rows){
            matchTemplate(imagenOriginal(Rect(cortarImagenX,cortarImagenY,espacio,espacio)),imagenComparar(Rect(cortarImagenX
                    ,cortarImagenY,espacio,espacio)),almacenador,CV_TM_CCORR_NORMED);
            minMaxLoc(almacenador,&minVal,&fitness,&minLoc,&maxLoc,Mat());;
            if(fitness>=1){
                individuo.posFitness[contadorFitness]=cortarImagenX;
                individuo.posFitness[contadorFitness+1]=cortarImagenY;
                individuo.cantidadPos+=1;
                contadorFitness+=2;
                fitnessTotal+=1;
                cortarImagenX+=espacio;
            }
            else{
                cortarImagenX+=espacio;
            }
        }
        cortarImagenY+=espacio;
    }
    if(fitnessTotal>=cantDivisiones){
        imshow("Solucion",imagenComparar);
        waitKey(0);
        individuo.fitness=-100;
        return individuo;
    }else{
        individuo.fitness=fitnessTotal;
        return individuo;
    }

}
void swapFitness(int pos1,int pos2){
    Individuos x = arrayFitness[pos2];
    arrayFitness[pos2] = arrayFitness[pos1];
    arrayFitness[pos1] = x;
}
void imprimirLista(){
    int x=0;
    while(arrayFitness[x].fitness!=0){
        cout<<arrayFitness[x].fitness<<endl;
        x+=1;
    }
}
void ordenarArrayFitness(){
    int fin=0;
    while(arrayFitness[fin].fitness!=0){
        fin++;
    }
    int inicio=0;
    int inicio2=0;
    for(inicio=0;inicio<fin-1;inicio++){
        for(inicio2=0;inicio2<fin-inicio-1;inicio2++){
            if(arrayFitness[inicio2].fitness<arrayFitness[inicio2+1].fitness){
                swapFitness(inicio2,inicio2+1);
            }
        }
    }
    int copiar=0;
    while(copiar<100){
        arrayNuevos[copiar]=arrayFitness[copiar];
        copiar+=1;
    }
}
int encontrarFitness(Mat imagenOriginal){
    int contador=0;
    while(arrayIndividuos[contador].rows!=0){
        arrayFitness[contador]= fitness2(imagenOriginal,arrayIndividuos[contador]);
        if(arrayFitness[contador].fitness==-100){
            return -100;
        }else{
            contador+=1;
        }

    }
    ordenarArrayFitness();
}
int longitud(){
    int contador=0;
    while(arrayIndividuos[contador].rows!=0){
        contador+=1;
    }
    return contador;
}
int revisarRepeticion(Mat imagen,Mat rect){
    double minVal;double fitness;Point minLoc;Point maxLoc;
    int espacio = imagen.rows/(sqrt(cantDivisiones));
    Mat almacenador(1,1,1);
    int compararX=0;
    int compararY=0;
    while(compararY<imagen.rows){
        compararX=0;
        while(compararX<imagen.rows){
            matchTemplate(imagen(Rect(compararX,compararY,espacio,espacio)),rect,almacenador,CV_TM_CCORR_NORMED);
            minMaxLoc(almacenador,&minVal,&fitness,&minLoc,&maxLoc,Mat());;
            if(fitness==1){
                return -1;
            }else{
                compararX+=espacio;
            }
        }
        compararY+=espacio;
    }
    return 1;
}
Mat crearNuevoIndividuo2(Individuos padre,Individuos madre,Mat hijoNuevo1){
    imshow("padre",madre.imagen);
    waitKey(0);
    int columnas=0;
    int cortarImagenX=0;
    int cortarImagenY=0;
    while(cortarImagenY<padre.imagen.rows){
        if(columnas==3){
            if(cortarImagenX==padre.imagen.rows){
                cortarImagenX=0;
            }
            break;
        }
        cortarImagenX=0;
        while(cortarImagenX<padre.imagen.rows){
            if(revisarRepeticion(hijoNuevo1,padre.imagen(Rect(cortarImagenX,cortarImagenY,45,45)))==-1){
                cortarImagenX+=45;
            }
            else{
                if(sum(hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)))==Scalar(0,0,0,0)){
                    padre.imagen(Rect(cortarImagenX,cortarImagenY,45,45)).copyTo( hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)));
                    cortarImagenX+=45;
                }
                else{
                    bool salir=false;
                    int x=0;
                    int y=0;
                    while(y<padre.imagen.rows){
                        x=0;
                        while(x<padre.imagen.rows){
                            if(sum(hijoNuevo1(Rect(x,y,45,45)))==Scalar(0,0,0,0)){
                                padre.imagen(Rect(cortarImagenX,cortarImagenY,45,45)).copyTo( hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)));
                                cortarImagenX+=45;
                                salir= true;
                                break;
                            }
                            else{
                                x+=45;
                            }
                        }
                        if(salir){
                            break;
                        }else{
                            y+=45;
                        }
                    }
                }
            }
        }
        columnas+=1;
        cortarImagenY+=45;
    }
    int cortarMadreX=0;
    int cortarMadreY=0;
    while(cortarMadreY<madre.imagen.rows){
        cortarMadreX=0;
        while(cortarMadreX<madre.imagen.rows){
            if(revisarRepeticion(hijoNuevo1,madre.imagen(Rect(cortarMadreX,cortarMadreY,45,45)))==-1){
                cortarMadreX+=45;
            }
            else{
                if(sum(hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)))==Scalar(0,0,0,0)){
                    madre.imagen(Rect(cortarMadreX,cortarMadreY,45,45)).copyTo(hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)));
                    cortarImagenX+=45;
                    if(cortarImagenX==madre.imagen.rows){
                        cortarImagenX=0;
                        cortarImagenY+=45;
                    }
                    if(cortarImagenY==madre.imagen.rows){
                        cortarImagenY=0;
                    }
                    cortarMadreX+=45;
                }else{
                    bool salir2= false;
                    int x2=0;
                    int y2=0;
                    while(y2<padre.imagen.rows){
                        x2=0;
                        while(x2<padre.imagen.rows){
                            if(sum(hijoNuevo1(Rect(x2,y2,45,45)))==Scalar(0,0,0,0)){
                                madre.imagen(Rect(cortarMadreX,cortarMadreY,45,45)).copyTo(hijoNuevo1(Rect(cortarImagenX,cortarImagenY,45,45)));
                                cortarImagenX+=45;
                                if(cortarImagenX==madre.imagen.rows){
                                    cortarImagenX=0;
                                    cortarImagenY+=45;
                                }
                                if(cortarImagenY==madre.imagen.rows){
                                    cortarImagenY=0;
                                }
                                salir2= true;
                                cortarMadreX+=45;
                                break;
                            }
                            else{
                                x2+=45;
                            }
                        }
                        if(salir2){
                            break;
                        }else{
                            y2+=45;
                        }
                    }
                }

            }
        }
        cortarMadreY+=45;
    }
    return hijoNuevo1;
}
Mat crearNuevoIndividuo1(Individuos padre,Individuos madre,Mat imagen){
    //imshow("padre",padre.imagen);
    //imshow("madre",madre.imagen);
    int x =0;
    int espacio = imagen.rows/(sqrt(cantDivisiones));
    int parada=padre.cantidadPos*2-1;
    Mat imagenNueva=Mat::zeros(padre.imagen.rows,padre.imagen.cols,padre.imagen.type());
    Mat almacenador(1,1,1);
    while(x<=parada){
        padre.imagen(Rect(padre.posFitness[x],padre.posFitness[x+1],espacio,espacio)).copyTo(imagenNueva(Rect(padre.posFitness[x],padre.posFitness[x+1],espacio,espacio)));
        x+=2;
    }
    int y=0;
    int parada2=madre.cantidadPos*2-1;
    while(y<=parada2){
        madre.imagen(Rect(madre.posFitness[y],madre.posFitness[y+1],espacio,espacio)).copyTo(imagenNueva(Rect(madre.posFitness[y],madre.posFitness[y+1],espacio,espacio)));
        y+=2;
    }
    //matchTemplate(padre.imagen(Rect(padre.posFitness[0],padre.posFitness[1],45,45)),imagenNueva(Rect(padre.posFitness[0],padre.posFitness[1],45,45)),almacenador,CV_TM_CCORR_NORMED);
    return imagenNueva;
}
void clearArray(){
    int x=0;
    Mat relleno;
    while(x<1000){
        arrayIndividuos[x]=relleno;
        x+=1;
    }
}
int generarHijos(Mat imagenO){
    int mutacion = rand()% 2000+1;
    int espacio = imagenO.rows/(sqrt(cantDivisiones));
    int contador1=0;
    int contador2=49;
    contadorInsertarIndividuos=0;
    clearArray();
    while(contador2<100){
        if(limiteGeneraciones>=1000){
            cout<<"Limite alcanzado"<<endl;
            return -10;
        }
        Mat hijoNuevo1=Mat::zeros(imagenO.rows,imagenO.cols,imagenO.type());
        hijoNuevo1=crearNuevoIndividuo1(arrayNuevos[contador1],arrayNuevos[contador2],imagenO);
        if(mutacion==2000){
            Mat temp=hijoNuevo1(Rect(0,0,espacio,espacio));
            hijoNuevo1(Rect(180,180,espacio,espacio)).copyTo(hijoNuevo1(Rect(0,0,espacio,espacio)));
            temp.copyTo(hijoNuevo1(Rect(180,180,espacio,espacio)));
        }
        //hijoNuevo2=crearNuevoIndividuo2(arrayNuevos[contador1],arrayNuevos[contador2],hijoNuevo1);
        arrayIndividuos[contadorInsertarIndividuos]=hijoNuevo1;
        contador1+=1;
        contador2+=1;
        cout<<contadorInsertarIndividuos<<endl;
        contadorInsertarIndividuos+=1;
        limiteGeneraciones+=1;
    }
}
void mostrarMejorFitness(){
    imshow("Mejor Fitness",arrayNuevos[0].imagen);
    imshow("Mejor Fitness2",arrayNuevos[1].imagen);
    imshow("Mejor Fitness3",arrayNuevos[2].imagen);
    waitKey(0);
}
void generarListaIndividuos(Mat imagenBase){
    for(int x=0;x<200;x++){
        arrayIndividuos[x]=arraryRects(imagenBase,cantDivisiones);
    }
}
int algGenetico(Mat imagenOriginal,int cantGeneraciones){
    while(cantGeneraciones>=0){
        if(encontrarFitness(imagenOriginal)==-100){
            cantGeneraciones=-1;
        }
        if(generarHijos(imagenOriginal)==-10){
            mostrarMejorFitness();
            return -10;
        }
        cantGeneraciones-=1;
    }
    mostrarMejorFitness();
}

int main() {
    /*
    srand(time(0));
    Mat image = imread("cloud.jpg");
    Mat kh = imread("KH.jpg");
    Mat almacenador(225,225,image.type());
    resize(image,almacenador,almacenador.size(),0,0);
    int divisiones=0;
    cout<<"Ingrese el numero de divisiones de la imagen 25 o 9";
    cin>>divisiones;
    cantDivisiones=divisiones;
    generarListaIndividuos(almacenador);
    bool salir=true;
    while(salir){
        int generaciones=0;
        cout<<"Ingrese las generaciones que desea adelantar:";
        cin>>generaciones;
        if(algGenetico(almacenador,generaciones)==-10){
            salir=false;
        }
    }
    */
    if(!capture.open(0)){
        return 0;
    }
    for(;;){
        Mat frame;
        capture>>frame;
        if(frame.empty()) break;
        imshow("WebCam",frame);
        if(waitKey(10)==27)break;
    }
    return 0;
}