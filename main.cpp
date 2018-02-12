#include <SFML/Graphics.hpp>
#include <string.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <stack>
#include <cstdlib>

using namespace sf;
using namespace std;

//const int N = 6;
#define N 13 //maximo numero de vértices
#define nodo pair< int , int > //definimos el nodo como un par( first , second ) donde first es el vertice adyacente y second el peso de la arista
#define INF 100000
int rutaCorta[21];
//string verticeEliminado[13];
int posicionMouse[10];//0-1 son click,,,,2-3 son inicio,,,,,4-5 son fin
int inicio = INF;
int fin = INF;
int inicio2 = INF;
int fin2 = INF;
int jugadores = 2;
int jugadorActual = 1;


//La cola de prioridad de C++ por default es un max-Heap (elemento de mayor valor en el tope)
//por ello es necesario realizar nuestro comparador para que sea un min-Heap
struct cmp {
    bool operator() ( const nodo &a , const nodo &b ) {
        return a.second > b.second;
    }
};

struct par {
    int valorDistancia;
    int verticeOrigen;
};

priority_queue< nodo , vector<nodo> , cmp > Q, P; //priority queue propia del c++, usamos el comparador definido para que el de menor valor este en el tope
int matriz[N][N]; //matriz de adyacencia
bool visitado[N];      //para vértices visitados
par distancia[N];
int matrizOriginal[N][N];
//Structs para el grafo
struct arco {
    //char destino;
    struct vertice *destino;
    int distancia;
    struct arco *sigA;
};

struct vertice{
    string ciudad;
    int numero;
    bool inicio;
    bool fin;
    bool inicio2;
    bool fin2;
    bool eliminado = false;
    bool eliminado2 = false;
    int posicion[2];
    struct vertice *sigV;
    struct arco *sigA;
    bool visitado;
    bool visitado2;
}*grafo;

//voids para grafo
void insertarV(string pciudad,int x,int y){

struct vertice *nnv = new vertice();
nnv->ciudad=pciudad;
nnv->visitado =false;
nnv->posicion[0] = x;
nnv->posicion[1] = y;
nnv->sigV=grafo;
grafo =nnv;
if(nnv->sigV != NULL)
nnv->numero = nnv->sigV->numero+1;
}
//busca un vertice
struct vertice* buscar(string pciudad){
    struct vertice *tempV = grafo;
    while(tempV!=NULL){

        if(tempV->ciudad==pciudad)
            return tempV;

        tempV=tempV->sigV;

    }
    return NULL;
}
//////////////inserta ARCOS///////////

//insercion al inicio de la sublista del vertice origen
void insertarA(string porigen,string pdestino, int pdistancia){

    struct vertice *origen =buscar (porigen);
    struct vertice *destino =buscar (pdestino);
    if((origen ==NULL )||(destino ==NULL)){
        cout << "Datos inconsistentes\n";
        return;
    }

    struct arco *nna = new arco();
    nna->destino=destino;
    nna->distancia=pdistancia;

    nna->sigA =origen->sigA;
    origen->sigA=nna;
}

int posicion(int numero, int tipo){
    int x;
    int y;
    if(grafo==NULL)
    {  cout<<"No hay grafo";
        return 0;
    }
    struct vertice *tempV=grafo;
    struct arco *tempA;
    while(tempV!=NULL)
    {
        if (numero == tempV->numero){
            x = tempV->posicion[0];
            y = tempV->posicion[1];
            if (tipo == 1){
                return x;
            }
            else{
                return y;
            }
            cout << x <<","<< y <<endl;
        }
        tempV=tempV->sigV;
    }
}
struct vertice* existeInicio(int i){
    struct vertice *tempV = grafo;
    if (i == 1){
        while(tempV!=NULL){
            if(tempV->inicio==true)
                return tempV;
            tempV=tempV->sigV;
        }
        return NULL;
    }
    else{
        while(tempV!=NULL){
            if(tempV->inicio2==true)
                return tempV;
            tempV=tempV->sigV;
        }
        return NULL;
    }

}
struct vertice* existeFin(int i){
    struct vertice *tempV = grafo;
    if (i == 1){
        while(tempV!=NULL){
            if(tempV->fin==true)
                return tempV;
            tempV=tempV->sigV;
        }
        return NULL;
    }
    else{
        while(tempV!=NULL){
            if(tempV->fin2==true)
                return tempV;
            tempV=tempV->sigV;
        }
        return NULL;
    }
}

struct vertice* mouseVertice(int x, int y){
    struct vertice *tempV = grafo;
    while(tempV!=NULL){
        for (int i=0;i<=13;i++){
            if(x-i == tempV->posicion[0] || x+i == tempV->posicion[0]){
                for (int k=0;k<=13;k++){
                    if(y-k == tempV->posicion[1] || y+k == tempV->posicion[1]){
                        return tempV;
                    }
                }
            }
        }
        tempV=tempV->sigV;
    }
    return NULL;
}

VertexArray dibujaLineas(VertexArray lines){
    if(grafo==NULL)
    {  cout<<"No hay grafo";
        return lines;
    }
    int p =-1;
    struct vertice *tempV=grafo;
    struct arco *tempA;
    while(tempV!=NULL)
    {
        tempA=tempV->sigA;
        while(tempA!=NULL)
        {
            p+=1;
            lines[p].position = sf::Vector2f(tempV->posicion[0]+5,tempV->posicion[1]+5);
            lines[p].color = sf::Color::Green;
            p+=1;
            lines[p].position = sf::Vector2f(tempA->destino->posicion[0]+5,tempA->destino->posicion[1]+5);
            lines[p].color = sf::Color::Green;
            tempA=tempA->sigA;
        }
        tempV=tempV->sigV;
    }
    return lines;
}

VertexArray dibujaLineasRC(VertexArray linesRC){
    int p =0;
    int q =0;
    if(grafo==NULL)
    {  cout<<"No hay grafo";
        return linesRC;
    }
    struct vertice *tempV=grafo;
    int x =0;
    while(tempV!=NULL)
    {
        x=0;
        if(tempV->numero == rutaCorta[p])
        {
                //cout <<rutaCorta[p]<<endl;
                linesRC[p].position = sf::Vector2f(tempV->posicion[0]+5,tempV->posicion[1]+5);
                linesRC[p].color = sf::Color::White;
                p+=1;
            if(rutaCorta[p]!=INF){
                tempV=grafo;
                x=1;
            }
            else{
                    for(int i = p;i<21;i++){
                        linesRC[i].position = linesRC[i-1].position;
                        linesRC[i].color = sf::Color::White;
                    }
                break;
            }
        }
        if (x==0){
            tempV=tempV->sigV;
        }
    }
    return linesRC;
}
VertexArray eliminaRC(VertexArray linesRC){
    for (int i=0;i<21;i++){
        linesRC[i].position = sf::Vector2f(0,0);
    }
    return linesRC;
}

void borrarA(){
    if (jugadorActual == 1){
        struct vertice *tempV = grafo;
        while(tempV!=NULL){
            if(tempV->eliminado == true){
                for(int k = 0; k<N; k++){
                    matriz[tempV->numero][k] = INF;
                    matriz[k][tempV->numero] = INF;
                    if (inicio==tempV->numero){
                        inicio = INF;
                        tempV->inicio=false;
                    }
                    else if (fin == tempV->numero){
                        fin = INF;
                        tempV->fin = false;
                    }
                }
            }
            tempV=tempV->sigV;
        }
    }
    else{
        struct vertice *tempV = grafo;
        while(tempV!=NULL){
            if(tempV->eliminado2 == true)
                for(int k = 0; k<N; k++){
                    matriz[tempV->numero][k] = INF;
                    matriz[k][tempV->numero] = INF;
                    if (tempV->numero == inicio2){
                        inicio2 = INF;
                        tempV->inicio2 = false;
                    }
                    else if (tempV->numero == fin2){
                        fin2 = INF;
                        tempV->fin2 = false;
                    }
                }
            tempV=tempV->sigV;
        }
    }
}


void amplitud(){
    if(grafo==NULL)
    {  cout<<"No hay grafo";
        return;
    }
    struct vertice *tempV=grafo;
    struct arco *tempA;
    while(tempV!=NULL)
    {
        tempA=tempV->sigA;
        while(tempA!=NULL)
        {
            matriz[tempV->numero][tempA->destino->numero] = tempA->distancia;
            //cout <<tempV->ciudad <<tempA->destino->ciudad <<tempA->distancia <<"numero"<<tempV->numero+1<<"x,y"<< tempV->posicion[0]<<tempV->posicion[1]<<endl;
            tempA=tempA->sigA;

        }
        tempV=tempV->sigV;
    }
}


void cargarDatos(){
	for(int i=0; i< N; i++)
		for(int j= 0; j < N; j++)
			matriz[i][j]= INF;

    insertarV("Estados Unidos",300,350); //Estados unidos
    insertarV("Brasil",450,550); // Brasil
    insertarV("Colombia",385,475); // Colombia
    insertarV("Russia",950,210); // Rusia
    insertarV("India",890,380); // India
    insertarV("Australia",1100,560); // Australia
    // SIN RELACION AMOROSA
    insertarV("Nigeria",640,430); // Nigeria
    insertarV("China",1000,360); // China
    insertarV("Argentina",400,650); // Argentina
    insertarV("Alemania",650,275); // Alemania
    insertarV("Irak",770,350);// Irak
    insertarV("Kazakhstan",850,270); // Kazakhstan
    insertarV("Canada",250,250); // Canada

    insertarA("Estados Unidos","Canada",300);
    insertarA("Canada","Estados Unidos",300);

    insertarA("Estados Unidos","Alemania",840);
    insertarA("Alemania","Estados Unidos",840);

    insertarA("Estados Unidos","Colombia",500);
    insertarA("Colombia","Estados Unidos",500);

    insertarA("Estados Unidos","Argentina",700);
    insertarA("Argentina","Estados Unidos",700);

    insertarA("Brasil", "Alemania",525);
    insertarA("Alemania","Brasil",525);

    insertarA("Brasil", "Nigeria",370);
    insertarA("Nigeria","Brasil",370);

    insertarA("Brasil", "Argentina",150);
    insertarA("Argentina","Brasil",150);

    insertarA("Colombia", "Alemania",725);
    insertarA("Alemania", "Colombia",725);

    insertarA("Colombia", "Kazakhstan",900);
    insertarA("Kazakhstan","Colombia", 900);

    insertarA("Colombia", "Nigeria",480);
    insertarA("Nigeria","Colombia", 480);

    insertarA("Colombia", "Argentina",200);
    insertarA("Argentina","Colombia",200);

    insertarA("Russia","Alemania",850);
    insertarA("Alemania","Russia",850);

    insertarA("Russia","India",400);
    insertarA("India","Russia",400);

    insertarA("Russia","China",550);
    insertarA("China","Russia",550);

    insertarA("India","Australia",820);
    insertarA("Australia","India",820);

    insertarA("India","Kazakhstan",350);
    insertarA("Kazakhstan","India",350);

    insertarA("India","Nigeria",700);
    insertarA("Nigeria","India",700);

    insertarA("Australia","China",990);
    insertarA("China","Australia",990);

    insertarA("Nigeria","Irak",120);
    insertarA("Irak","Nigeria",120);

    insertarA("Alemania","Canada",480);
    insertarA("Canada","Alemania",480);

    insertarA("Irak","Kazakhstan",200);
    insertarA("Kazakhstan","Irak",200);

    amplitud();

}
//función de inicialización
void inicializar(){
    for( int i = 0 ; i <= N ; ++i ){
        distancia[ i ].valorDistancia = INF;  //inicializamos todas las distancias con valor infinito
        distancia[ i ].verticeOrigen = INF;  //inicializamos todas las distancias con valor infinito
        visitado[ i ] = false; //inicializamos todos los vértices como no visitados
    }
}
void Dijkstra( int inicio){
	inicializar();
	int x = Q.size();
	Q.push( nodo( inicio , 0 ) ); //Insertamos el vértice inicial en la Cola de Prioridad
    distancia[ inicio ].valorDistancia = 0;      //Este paso es importante, inicializamos la distancia del inicial como 0
    int actual , adyacente , peso;
    while( !Q.empty() ){                   //Mientras cola no este vacia
        actual = Q.top().first;            //Obtengo de la cola el nodo con menor peso, en un comienzo será el inicial
        Q.pop();                           //Sacamos el elemento de la cola
        if( visitado[ actual ] )
			continue; //Si el vértice actual ya fue visitado entonces sigo sacando elementos de la cola

		visitado[ actual ] = true;         //Marco como visitado el vértice actual

        for( int i = 0 ; i < N ; ++i ){ //reviso sus adyacentes del vertice actual
            if(matriz[actual][i] != INF){
	            peso = matriz[actual][i]  ;        //peso de la arista que une actual con adyacente ( actual , adyacente )
	            if( !visitado[ i ] ){        //si el vertice adyacente no fue visitado

	            	if( distancia[ actual ].valorDistancia + peso < distancia[ i ].valorDistancia ){
				        distancia[ i ].valorDistancia = distancia[ actual ].valorDistancia + peso; //relajamos el vertice actualizando la distancia
				        distancia[ i ].verticeOrigen = actual;
				        Q.push( nodo( i , distancia[ i ].valorDistancia ) ); //agregamos adyacente a la cola de prioridad
    				}

				}
        	}
        }
    }

}
void imprimir(){

	int i,j;
	for(i=0; i< N; i++){
		for(j= 0; j < N; j++)
			cout<<matriz[i][j]<<"  ";
		cout<<endl;

	}
}
void imprimirDistancias(){
	for(int i=0; i< N; i++)
		cout<< "(" << distancia[ i ].valorDistancia << "-" << distancia[ i ].verticeOrigen <<"), ";
    cout << endl;
}

void imprimirRutaCorta(int destino){
    for (int i = 0;i<21;i++){
        rutaCorta[i]=INF;
    }
    int cont = -1;
    int dest = destino;
    stack<int> pila;
    int dist=distancia[dest].valorDistancia;
    while(dest != INF)
    {
        pila.push(dest);
        dest= distancia[dest].verticeOrigen;
    }

    //cout << "Ruta más corta para llegar a " << destino << " desde el vertice inicial es:" << endl;
    while(pila.size()!=0)
    {
        cont+=1;
      //  cout << pila.top()+1 ;
        rutaCorta[cont]=pila.top();
        pila.pop();
        //if (pila.size() != 0){}
          //  cout << " - ";
    }
    //cout << endl;
    //cout << "Distancia total de la ruta: " << dist << endl;
}
CircleShape circulo(int x,int y){ // recibe las posiciones x,y
    CircleShape circulo1(5.f);
    circulo1.setFillColor(Color::Red); // color
    circulo1.setPosition(x,y);
    return circulo1;

}

CircleShape circuloDesabilitado(int x,int y){ // recibe las posiciones x,y
    CircleShape circulo1(5.f);
    circulo1.setFillColor(Color::Black); // color
    circulo1.setPosition(x,y);
    return circulo1;
}
CircleShape circuloInicio(int x,int y){ // recibe las posiciones x,y
    CircleShape circulo1(5.f);
    circulo1.setFillColor(Color::Yellow); // color
    circulo1.setPosition(x,y);
    return circulo1;
}
CircleShape circuloFin(int x,int y){ // recibe las posiciones x,y
    CircleShape circulo1(5.f);
    circulo1.setFillColor(Color::Blue); // color
    circulo1.setPosition(x,y);
    return circulo1;
}

RectangleShape linea(int  x,int y, int rot, int largo){ // recibe posicion (x,y) y la rotacion de la linea y el largo
    RectangleShape rectangulo(Vector2f(largo,2)); //ancho,alto
    rectangulo.setPosition(x,y);
    rectangulo.setFillColor(Color::Green); //poner color
    rectangulo.rotate(rot); // poner totacion a la linea
    return rectangulo;
}
void borrarV(int x, int y){
    struct vertice *tempV = mouseVertice(x ,y);
    if (jugadorActual==1 && tempV!=NULL){
        if (tempV->eliminado==true){
            tempV->eliminado = false;
        }
        else{
            tempV->eliminado = true;
        }
    }
    else if(jugadorActual == 2 && tempV!=NULL){
        if (tempV->eliminado2==true){
            tempV->eliminado2 = false;
        }
        else{
            tempV->eliminado2 = true;
        }
    }

}
void marcarVertice(int x, int y){
    struct vertice *tempV = mouseVertice(x ,y);
    posicionMouse[0]= x;
    posicionMouse[1]=y;
    if (jugadorActual == 1){
        struct vertice *tempVInicio = existeInicio(1);
        struct vertice *tempVFin = existeFin(1);

        if (tempVInicio==NULL && tempVFin==NULL){
            if(tempV!=NULL && tempV->eliminado == false){
                tempV->inicio = true;
                posicionMouse[2]=x;
                posicionMouse[3]=y;
                inicio = tempV->numero;
            }
        }
        else if (tempVInicio!=NULL && tempVFin==NULL){
            if(tempV == tempVInicio){
                    tempV->inicio = false;
                    posicionMouse[2]= INF;
                    posicionMouse[3]= INF;
                    inicio = INF;
                }
                else if (tempV != tempVFin && tempV->eliminado == false){
                    tempV->fin = true;
                    posicionMouse[4]= x;
                    posicionMouse[5]=y;
                    fin = tempV->numero;
                }
        }
        else if (tempVInicio==NULL && tempVFin!=NULL){
                if (tempV == tempVFin){
                    tempV->fin = false;
                    posicionMouse[4]= INF;
                    posicionMouse[5]= INF;
                    fin = INF;
                }
                else if(tempV != tempVInicio && tempV->eliminado == false){
                    tempV->inicio = true;
                    posicionMouse[2]= x;
                    posicionMouse[3]=y;
                    inicio = tempV->numero;
                }


        }
        else{
                if(tempV == tempVInicio){
                    tempV->inicio = false;
                    posicionMouse[2]= INF;
                    posicionMouse[3]= INF;
                    inicio = INF;
                }
                else if (tempV== tempVFin){
                    tempV->fin = false;
                    posicionMouse[4]= INF;
                    posicionMouse[5]= INF;
                    fin = INF;
                }
            }
    }
    else{
        struct vertice *tempVInicio = existeInicio(2);
        struct vertice *tempVFin = existeFin(2);

        if(tempVInicio==NULL && tempVFin==NULL){
            if(tempV!=NULL && tempV->eliminado2 == false){
                tempV->inicio2 = true;
                posicionMouse[6]=x;
                posicionMouse[7]=y;
                inicio2 = tempV->numero;
            }
        }
        else if (tempVInicio!=NULL && tempVFin==NULL){
                if(tempV == tempVInicio){
                    tempV->inicio2 = false;
                    posicionMouse[6]= INF;
                    posicionMouse[7]= INF;
                    inicio2 = INF;
                }
                else if (tempV != tempVFin && tempV->eliminado2 == false){
                    tempV->fin2 = true;
                    posicionMouse[8]= x;
                    posicionMouse[9]=y;
                    fin2 = tempV->numero;
                }
        }
        else if (tempVInicio==NULL && tempVFin!=NULL){
            if (tempV == tempVFin){
                    tempV->fin2 = false;
                    posicionMouse[8]= INF;
                    posicionMouse[9]= INF;
                    fin2 = INF;
                }
                else if(tempV != tempVInicio && tempV->eliminado2 == false){
                    tempV->inicio2 = true;
                    posicionMouse[6]= x;
                    posicionMouse[7]=y;
                    inicio2 = tempV->numero;
                }
        }
        else{
            if(tempV == tempVInicio){
                    tempV->inicio2 = false;
                    posicionMouse[6]= INF;
                    posicionMouse[7]= INF;
                    inicio2 = INF;
                }
                else if (tempV== tempVFin){
                    tempV->fin2 = false;
                    posicionMouse[8]= INF;
                    posicionMouse[9]= INF;
                    fin2 = INF;
                }
        }
    }

    for(int i = 0; i < 6 ;i++){
        cout << "posicion: " << i<<endl;
        cout << posicionMouse[i] << endl;
    }
}
///////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    cargarDatos();
    amplitud();
    borrarA();
	cout<<"Grafo............\n";
	imprimir();
    //borrarA("Kazakhstan");
    //borrarA("Estados Unidos");
    //borrarA("Colombia");
    RenderWindow window(VideoMode(1300,768), "Proyecto Estructuras");
    sf::VertexArray linesEU(sf::LinesStrip,8);
    sf::VertexArray linesBR(sf::LinesStrip,6);

    Texture img; // crear la textura para colocar la imagen
    img.loadFromFile("imagenes/mapa.jpg"); // cargar la imagen desde la direccion
    Sprite fondo(img); // crear un sprite de la imagen
    //int x1,y2;
    Font fuente;
	Text texto1,texto2,texto3,texto4,texto5,texto6,texto7,texto8,texto9,
        texto10,texto11,texto12,texto13,texto14,texto15;
	fuente.loadFromFile("fuente.ttf");

	texto1.setString("Canada");// Asignamos la cadena al texto
	texto1.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto1.setColor(Color::Black); // Poner color al texto
	texto1.setCharacterSize(12);// Tamaño de la fuente
    texto1.setPosition(255,255); // Posición del texto

    texto2.setString("Estados Unidos");// Asignamos la cadena al texto
	texto2.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto2.setColor(Color::Black); // Poner color al texto
	texto2.setCharacterSize(12);// Tamaño de la fuente
    texto2.setPosition(305,360); // Posición del texto

    texto3.setString("Argentina");// Asignamos la cadena al texto
	texto3.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto3.setColor(Color::Black); // Poner color al texto
	texto3.setCharacterSize(12);// Tamaño de la fuente
    texto3.setPosition(405,655); // Posición del texto

    texto4.setString("Colombia");// Asignamos la cadena al texto
	texto4.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto4.setColor(Color::Black); // Poner color al texto
	texto4.setCharacterSize(12);// Tamaño de la fuente
    texto4.setPosition(390,480); // Posición del texto

    texto5.setString("Brasil");// Asignamos la cadena al texto
	texto5.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto5.setColor(Color::Black); // Poner color al texto
	texto5.setCharacterSize(12);// Tamaño de la fuente
    texto5.setPosition(455,555); // Posición del texto

    texto6.setString("Nigeria");// Asignamos la cadena al texto
	texto6.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto6.setColor(Color::Black); // Poner color al texto
	texto6.setCharacterSize(12);// Tamaño de la fuente
    texto6.setPosition(645,435); // Posición del texto

    texto7.setString("Alemania");// Asignamos la cadena al texto
	texto7.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto7.setColor(Color::Black); // Poner color al texto
	texto7.setCharacterSize(12);// Tamaño de la fuente
    texto7.setPosition(655,280); // Posición del texto

    texto8.setString("Irak");// Asignamos la cadena al texto
	texto8.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto8.setColor(Color::Black); // Poner color al texto
	texto8.setCharacterSize(12);// Tamaño de la fuente
    texto8.setPosition(775,355); // Posición del texto

    texto9.setString("Kazakhstan");// Asignamos la cadena al texto
	texto9.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto9.setColor(Color::Black); // Poner color al texto
	texto9.setCharacterSize(12);// Tamaño de la fuente
    texto9.setPosition(855,275); // Posición del texto

    texto10.setString("Rusia");// Asignamos la cadena al texto
	texto10.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto10.setColor(Color::Black); // Poner color al texto
	texto10.setCharacterSize(12);// Tamaño de la fuente
    texto10.setPosition(955,215); // Posición del texto

    texto11.setString("India");// Asignamos la cadena al texto
	texto11.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto11.setColor(Color::Black); // Poner color al texto
	texto11.setCharacterSize(12);// Tamaño de la fuente
    texto11.setPosition(895,385); // Posición del texto

    texto12.setString("China");// Asignamos la cadena al texto
	texto12.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto12.setColor(Color::Black); // Poner color al texto
	texto12.setCharacterSize(12);// Tamaño de la fuente
    texto12.setPosition(1005,365); // Posición del texto

    texto13.setString("Australia");// Asignamos la cadena al texto
	texto13.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto13.setColor(Color::Black); // Poner color al texto
	texto13.setCharacterSize(12);// Tamaño de la fuente
    texto13.setPosition(1105,565); // Posición del texto

    texto14.setString("Amarillo :: Inicio");// Asignamos la cadena al texto
	texto14.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto13.setColor(Color::Black); // Poner color al texto
	texto14.setCharacterSize(14);// Tamaño de la fuente
    texto14.setPosition(0,0); // Posición del texto

    texto15.setString("Cian :: Fin");// Asignamos la cadena al texto
	texto15.setFont(fuente); // Asignamos la fuente que hemos cargado al texto
	//texto13.setColor(Color::Black); // Poner color al texto
	texto15.setCharacterSize(14);// Tamaño de la fuente
    texto15.setPosition(0,16); // Posición del texto


    while (window.isOpen())
    {
        amplitud();
        borrarA();
        Vector2i pos = Mouse::getPosition(window);
        Event event;
        while (window.pollEvent(event)){
            if(event.type == Event::MouseButtonPressed){
                if(event.key.code == Mouse::Left){
                    marcarVertice(pos.x ,pos.y);
                }
                if(event.key.code == Mouse::Right){
                    borrarV(pos.x,pos.y);
                    amplitud();
                    borrarA();
                }
            }
            if (event.type == Event::Closed){
                window.close();
            }
            if (event.type == Event::KeyPressed){
                if (event.key.code == sf::Keyboard::Num1){
                    if(jugadores == 2){
                        jugadorActual = 1;
                    }
                }
                if (event.key.code == sf::Keyboard::Num2){
                    if(jugadores == 2){
                        jugadorActual = 2;
                    }
                }
            }
        }
        window.clear();
        window.draw(fondo); // poner el fondo
        sf::VertexArray lines(sf::Lines,145);
        sf::VertexArray linesRC(sf::LinesStrip,21);
        sf::VertexArray linesRC2(sf::LinesStrip,21);
        window.draw(dibujaLineas(lines));
        if (jugadorActual==1){
            if (inicio != INF && fin != INF){
                Dijkstra(inicio);
                imprimirRutaCorta(fin);
                window.draw(dibujaLineasRC(linesRC));
            }
            else{
                window.draw(eliminaRC(linesRC));
            }
        }
        else{
            if (inicio2 != INF && fin2 != INF){
                Dijkstra(inicio2);
                imprimirRutaCorta(fin2);
                window.draw(dibujaLineasRC(linesRC2));
            }
            else{
                window.draw(eliminaRC(linesRC2));
            }
        }



        //cout << "entra a imprimir RC"<<endl;
        //cout << "Sale a imprimir RC"<<endl;
        window.draw(texto1);
        window.draw(texto2);
        window.draw(texto3);
        window.draw(texto4);
        window.draw(texto5);
        window.draw(texto6);
        window.draw(texto7);
        window.draw(texto8);
        window.draw(texto9);
        window.draw(texto10);
        window.draw(texto11);
        window.draw(texto12);
        window.draw(texto13);
        window.draw(texto14);
        window.draw(texto15);


        window.draw(circulo(posicion(0,1), posicion(0,2)));
        window.draw(circulo(posicion(1,1), posicion(1,2)));
        window.draw(circulo(posicion(2,1), posicion(2,2)));
        window.draw(circulo(posicion(3,1), posicion(3,2)));
        window.draw(circulo(posicion(4,1), posicion(4,2)));
        window.draw(circulo(posicion(5,1), posicion(5,2)));
        window.draw(circulo(posicion(6,1), posicion(6,2)));
        window.draw(circulo(posicion(7,1), posicion(7,2)));
        window.draw(circulo(posicion(8,1), posicion(8,2)));
        window.draw(circulo(posicion(9,1), posicion(9,2)));
        window.draw(circulo(posicion(10,1), posicion(10,2)));
        window.draw(circulo(posicion(11,1), posicion(11,2)));
        window.draw(circulo(posicion(12,1), posicion(12,2)));
        if (inicio != INF && jugadorActual ==1){
            window.draw(circuloInicio(posicion(inicio,1), posicion(inicio,2)));
        }
        if (fin != INF && jugadorActual ==1){
            window.draw(circuloFin(posicion(fin,1), posicion(fin,2)));
        }
        if (inicio2 != INF && jugadorActual ==2){
            window.draw(circuloInicio(posicion(inicio2,1), posicion(inicio2,2)));
        }
        if (fin2 != INF && jugadorActual ==2){
            window.draw(circuloFin(posicion(fin2,1), posicion(fin2,2)));
        }
        if (jugadorActual == 1){
            struct vertice *tempV = grafo;
            while(tempV!=NULL){
                if(tempV->eliminado == true){
                    window.draw(circuloDesabilitado(posicion(tempV->numero,1), posicion(tempV->numero,2)));
                    tempV=tempV->sigV;
                }
                else{
                    tempV= tempV->sigV;
                }
            }
        }
        else{
            struct vertice *tempV = grafo;
            while(tempV!=NULL){
                if(tempV->eliminado2 == true){
                    window.draw(circuloDesabilitado(posicion(tempV->numero,1), posicion(tempV->numero,2)));
                    tempV=tempV->sigV;
                }
                else{
                    tempV= tempV->sigV;
                }
            }
        }

        //cout << "cantidad de jugadores:"<<jugadores<<", jugador actual:"<<jugadorActual<< endl;
        cout << inicio2 <<", " << fin2 << endl;
        window.display();

    }
    return 0;
}
