#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "towerengine.h"

//Funciones inicializadoras
Plane* initPlane(void)
{
    Plane* p = (Plane*)malloc(sizeof(Plane));
    if(!p)
    {
        printf("No se inicializo la memoria del plano\n");
        exit(-1);
    }

    p->board2D = (char**)malloc(sizeof(char*)*DIMENSION);
    if(!(p->board2D))
    {
        printf("No se inicializo la memoria del tablero del plano\n");
        exit(-2);
    }

    for(int i = 0; i < DIMENSION; ++i)
    {
        p->board2D[i] = (char*)malloc(sizeof(char)*DIMENSION);
        if(!(p->board2D[i]))
        {
            printf("No se inicializo la memoria de la fila %d del plano\n", i);
            exit(-3);
        }

        memset(p->board2D[i], '0', DIMENSION);  // inicializar en '0'
    }

    return p;
}


//FUnción que inicia en memoria lo correspondiente al jugador
Player* initPlayer(char id)
{
	Player *p;
	p = (Player*)malloc(sizeof(Player));


	if(!p)
	{
		printf("No se inicializo la memoria del jugador\n");
		exit(-4);
	}

	p->id = id;
	p->ticket = 4;

	return p;
}


//Función que inicializa en memoria a la torre
Tower* initTower(void) {
    Tower *t = (Tower*)malloc(sizeof(Tower));
    if(!t) {
        printf("No se inicializo adecuadamente la torre\n");
        exit(-1);
    }

    t->board3D  = (char***)malloc(sizeof(char**)*DIMENSION);
    if(!(t->board3D)) {
        printf("No se inicializo la memoria del tablero 3D\n");
        exit(-2);
    }

    for(int i = 0; i < DIMENSION; ++i) {
        t->board3D[i] = (char**)malloc(sizeof(char*)*DIMENSION);
        if(!(t->board3D[i])) {
            printf("No se inicializo adecuadamente la memoria para las filas del piso %d\n", i);
            exit(-3);
        }

        for(int j = 0; j < DIMENSION; ++j) {
            t->board3D[i][j] = (char*)malloc(sizeof(char)*DIMENSION);
            if(!(t->board3D[i][j])) {
                printf("No se inicializo adecuadamente la memoria de la fila %d del piso %d\n", j, i);
                exit(-4);
            }

            memset(t->board3D[i][j], '0', DIMENSION);
        }
    }

    t->spacesLeft = DIMENSION*DIMENSION*DIMENSION;

    return t;
}


Point3D* initPoint3D(void)
{
	Point3D* p;
	p = (Point3D*)malloc(sizeof(Point3D));


	if(!p)
	{
		printf("No se inicializo el punto 3D\n");
		exit(-5);
	}
	return p;

}
Point2D* initPoint2D(void)
{
	Point2D* p;
	p = (Point2D*)malloc(sizeof(Point2D));

	if(!p)
	{
		printf("No se inicializo el punto 2D\n");
		exit(-6);
	}

	return p;

}

//--------------------------------
//Funciones de mantenimiento
//--------------------------------

Plane* obtainPlane(int n, int axis)
//-------------------------------
{
	Plane* plane =  initPlane();
	int x,y,z;

	//Se pasan dos argumentos.
	//El numero del plano cuando el eje Axis es fijo. De esa forma se diferencian
	//Entonces:

	for(int i=0; i <DIMENSION; i++) {
		for(int j=0; j < DIMENSION; j++) {
			switch (axis)
			{
			case X:
				x = n; y = i; z = j; break;
			case Y:
				x = i; y = n; z = j; break;
			case Z:
				x = i; y = j; z = n; break;
			case D1:
				x = i; y = j; z = j; break;
			case D2:
				x = i; z = j; y = (DIMENSION - 1) - j; break;
			}
			plane->board2D[i][j] = tower->board3D[x][y][z];
		}
	}
	return plane;
}

//Función que proyecta un punto 3D a uno 2D
Point2D* obtainPoint2D(Point3D* p, int axis)
{
	Point2D* p2d = initPoint2D(); //inicializar punto

	switch (axis)
	{

	case X: //(y,z)
		p2d->x = p->y;
		p2d->y = p->z;
		break;

	case Y: //(x,z)
		p2d->x = p->x;
		p2d->y = p->z;
		break;

	case Z: //(x,z)
		p2d->x = p->x;
		p2d->y = p->y;
		break;

	}
	return p2d;
}

//Crea una copia segura de la torre para hacer validación que el giro es permisible
Tower* copyTower(Tower* tower)
{
	Tower* t = initTower();

	for(int x=0; x < DIMENSION; x++)
	{
		for(int y=0; y < DIMENSION; y++)
		{
			for(int z = 0;   z < DIMENSION  ; z++)
			{
				t->board3D[x][y][z] = tower->board3D[x][y][z];
			}
		}
	}
	return t;
}

//--------------------
//Funciones de giro
//---------------------
Plane* turn90Left(Plane* p)
{

    Plane* ptr = initPlane();

    if(!ptr) return NULL;
    for(int i=0; i < DIMENSION; ++i)
        for(int j=0; j < DIMENSION; ++j)
            ptr->board2D[DIMENSION-j-1][i] = p->board2D[i][j]; //Se realiza el intercambio
    //Si todo sale bien

    return ptr; //Se regresa un 0, en señal de que salio bie
}

Plane* turn90Right(Plane* p)
{

	Plane* ptr = initPlane();
     if(!ptr) return NULL;

    //Hacer la rotación
    for(int i=0; i < DIMENSION; ++i)
        for(int j=0; j < DIMENSION; ++j)
                ptr->board2D[j][DIMENSION-i-1] = p->board2D[i][j];

    return ptr;
}

/*
 * PRIMERA Y PRINCIPAL FUNCIÓN DE LÓGICA
 * Toma las coordenadas de la canica puesta/que se quiere validar,
 * organiza la verificación en los planos que intersecta ese punto
 * y se hace la validación de que si forma una linea de 4 en lo horizontal,
 * vertical o diagonal.
 */

void verifyWin(Point3D *p, int *resultado)
{
	//Función que checa si ganó el jugador en su turno.
	//Nos basamos en la lógica de que, siemmpre un punto hace intersección en 3 planos
	Plane* plane;
	Point2D* p2d;

	for(int i=0; i < 3; i++)
	{
		switch(i)
		{
		case X:
			plane = obtainPlane(p->x, X);
			p2d = obtainPoint2D(p, X);
			break;

		case Y:
			plane = obtainPlane(p->y, Y);
			p2d = obtainPoint2D(p, Y);
			break;

		case Z:
			plane = obtainPlane(p->z, Z);
			p2d = obtainPoint2D(p, Z);
			break;
		}

		//proceso de validación
		verifyPlane(plane,resultado,0,p2d->x); //Checar horizontalmente
		if (*resultado) {free(plane); free(p2d); return;}
		verifyPlane(plane,resultado,1,p2d->y); //Checar verticalmente
		if (*resultado) {free(plane); free(p2d); return;}
		verifyPlane(plane,resultado,2,0); //Checar primera diagonal
		if (*resultado) {free(plane); free(p2d); return;}
		verifyPlane(plane,resultado,3,0); //Checar segunda diagonal
		if (*resultado) {free(plane); free(p2d); return;}

		//Estar liberando la memoria del plano
		free(plane);
		free(p2d);
	}
}

//Función que verifica dentro del plano dado
void verifyPlane(Plane* plane, int *resultado,int line, int n)
{
	int playerPoints[4] = {0,0,0,0};

	int x=0,y=0;
	for(int i=0; i < DIMENSION; i++)
	{
		switch (line) {
		case 0: x = n; y = i; break;
		case 1: x = i; y = n; break;
		case 2: x = i; y = i; break;
		case 3: x = i; y = DIMENSION-1-i;
		}
		for (int i = 0; i < PLAYERS; i++){
            if (plane->board2D[x][y] == '1'+i) playerPoints[i]++;
		}
	}
    //mutex
    //si resultado != 0 parar los hilos y destruirlos
    //unlock mutex
	for (int i = 0; i < PLAYERS; i++){
        *resultado = (playerPoints[i] == DIMENSION) ? i+1 : 0;
        if (*resultado) break;
	}
}

// Utilizar la función verifyPlane para verificar las diagonales 3D de la torre
void verifyDiagonals(int axis,int *resultado)
{
    Plane* plane = obtainPlane(0, axis);
    verifyPlane(plane,resultado,2,0); //Checar primera diagonal
	if (*resultado) {free(plane); return;}
	verifyPlane(plane,resultado,3,0); //Checar segunda diagonal
	if (*resultado) {free(plane); return;}

	free(plane);
}

//Checa todos los puntos de un plano dado para verificar que no se ha ganado alguien al realizar un giro
void checkAllTower(Tower* t, Plane* p, int index, int* resultado)
{
	Point3D* point = initPoint3D(); //Se crea un punto 3D que se va a estar iterando para verificar todo el plano
	point->x = index; //Se determina el plano

    verifyDiagonals(D1,resultado);
    if (*resultado) {free(point); return;}
    verifyDiagonals(D2,resultado);
    if (*resultado) {free(point); return;}

    for (point->y = 0; point->y < DIMENSION; ++(point->y)) //Eje y
        for(point->z=0; point->z < DIMENSION; ++(point->z)) //Eje z
        {
            verifyWin(point, resultado);
            if(*resultado) {free(point); return;} //Salirse automaticamente si se identificó que alguien va a ganar
        }

     free(point); //liberar memoria
}

//Funciones de liberación de memoria
//Función que limpia la memoria de la torre
void freeTower(Tower* t)
{
    if(!t) return;  // Si directamente se manda un NULL

    for(int i = 0; i < DIMENSION; ++i)
    {
        for(int j = 0; j < DIMENSION; ++j)
        {
            free(t->board3D[i][j]); // libera cada fila
        }
        free(t->board3D[i]); // libera la matriz de punteros de cada piso
    }

    free(t->board3D); // libera el arreglo de pisos
    free(t);          // libera la torre
}

// Función que libera la memoria del plano
void freePlane(Plane* p)
{
    if(!p) return; // si directamente el plano es nulo

    for(int i = 0; i < DIMENSION; ++i)
    {
        free(p->board2D[i]); // libera cada fila
    }

    free(p->board2D); // libera el arreglo de punteros a filas
    free(p);          // libera el plano
}

//Limpia la torre y los datos de cada jugador
void reset(void)
{
	freeTower(tower);
	for (int i = 0; i < PLAYERS; i++)
        free(players[i]);
    free(players);
}
