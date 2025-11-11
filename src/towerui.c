#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <pthread.h>
#include "towerengine.h"
#include "towerui.h"

//Función que muestra el menú de la torre
void displayTower(int n,int* axis,int* plano,char* c)
{
    system("cls");
	printf("Mostrando plano fijo ");

	if(*axis==X)
		printf("X");
	else if(*axis == Y)
		printf("Y");
	else if(*axis == Z)
		printf("Z");
	else if (*axis == D1)
		printf("Primera diagonal");
	else
		printf("Segunda diagonal");

	if(*axis != D1 && *axis != D2)
		printf("\n Plano: %d\n", *plano+1 );
	else
		printf("\n");

	displayStructure(*plano, *axis);

	printf("\nPulse las teclas para realizar:"
			"\nArriba:    Cambiar eje arriba"
			"\nAbajo:     Cambiar eje abajo"
			"\nIzquierda: Mover plano izquierdo"
			"\nDerecha:   Mover plano derecho");
    if (n)
    {
     printf("\ne:         Pasar turno (Jugador %d)",n); //Si n == 0 no se despliega esta parte porque no hay opción de jugar
    }
     printf("\nq:         Salir\n");
	*c = readKeyboard();

	updateMenu(c,axis,plano);
}

//FUnción que permite actualizar y recibir las entradas del teclado del usuario
void updateMenu(char *c, int *axis, int *plano)
{
	switch (*c)
	{
	case 'u': //arriba
		*axis = (*axis + 1) % 5;
		break;

	case 'd': //abajo
        *axis = (*axis + 4) % 5;
		break;

	case 'l': //izquierda
        if (*axis < 3) *plano = (*plano - 1 + DIMENSION) % DIMENSION;
		break;

	case 'r': //derecha
        if (*axis < 3) *plano = (*plano + 1) % DIMENSION;
		break;
	}
}

//Función que manda a llamar cada una de los distintos posibles planos
void displayStructure(int n,  int axis)
{
	switch(axis)
	{
	case X:
	    printf("\nCapas horizontales del cubo. Siendo Plano 1 la parte superior, y Plano 4 la parte inferior.\n");
		displayX(n); // Para ajustar la vista a lo que se ve, se empieza desde la cara frontal
		break;
	case Y: //Muestra con respecto al eje Y
        printf("\nCara lateral derecha del cubo. Donde Plano 1 es la parte frontal, y Plano 4 es la parte trasera.\n");
		displayY(DIMENSION-n-1);
		break;
	case Z: //Con respecto al eje Z
        printf("\nCara lateral izquierda del cubo. Donde Plano 1 es la parte izquierda, y Plano 4 es la parte derecha.\n");
		displayZ(n);
		break;
	case D1: //Así como las diagonales
		printf("\nDiagonal principal que cruza la torre\n");
		displayD1();
		break;
	case D2:
		printf("\nAnti-diagonal que cruza la torre\n");
		displayD2();
		break;
	}
}
//Función que imprime un tablero horizontal

void displayX(int n)
{
    // 1. Obtener el plano

    Plane* plane = obtainPlane(n, X);

    // 2. Mostrarlo en formato "horizontal"

    //Final inicial de asteriscos
    for (int s = 0; s < DIMENSION; ++s)
        {
            printChar('*');
            printf(" ");
        }
    	printf("\n");

    for (int i = 0; i < DIMENSION; ++i)
    {
        //Espacio en blanco que da efecto de horizontalidad

    	for (int s = 0; s < i; ++s)
        {
    		printf("  ");
        }
    	printChar('*');
    	printf(" ");

        // imprimir cada columna
        for (int j = 0; j < DIMENSION; ++j)
        {
            printChar(plane->board2D[i][j]);
            printChar(' ');
        }

        // terminar la fila con '*'
        printChar('*');
        printChar('\n');
    }

    // fila final de asteriscos
    for (int s = 0; s < DIMENSION; ++s)
    {
        printChar(' ');
        printChar(' ');
    }
    for (int j = 0; j < DIMENSION; ++j)
    {
        printChar('*');
        printChar(' ');
    }
    printChar('\n');

    free(plane);
}

void displayY(int n)
{
    // 1. Obtener el plano

    Plane* plane = obtainPlane(n, Y);

    // 2. Mostrarlo en formato "horizontal"
    	//Fila de cobertura de asteriscos
    printf("      ");
    for(int i=0; i < DIMENSION; ++i)
    {
    	printChar('*');
    	printf(" ");
    }
    printf("\n");

    for(int i=0; i < DIMENSION; ++i)
    {

    	//Imprimir espacios
    	for(int s= i; DIMENSION-s > 0; ++s )
    	{
    		printf(" ");
    	}

    	printChar('*');
    	printf(" ");
    	for(int j=0; j < DIMENSION; ++j)
    	{
    		//Imprimir la información
    		printChar(plane->board2D[i][j]);
    		printf(" ");

    	}
    	printChar('*');
    	printf("\n");
    }

    printf("   ");
    //Asteriscos finales
    for(int i=0; i < DIMENSION; ++i)
    {
    	printChar('*');
    	printf(" ");
    }
    printf("\n");

    free(plane);
}


//Función para imprimir torre con Eje X fijo

void displayZ(int n)
{
	 // 1. Obtener el plano

	    Plane* plane = obtainPlane(n, Z);
	    // 2. Mostrarlo en formato "horizontal"

	    //Final inicial de asteriscos
	    for (int s = 0; s < DIMENSION + 2; ++s)
        {
	        printChar('*');
	        printf(" ");
	    }
	    printf("\n");

	    for (int i = 0; i < DIMENSION; ++i)
	    {
	        //Espacio en blanco que da efecto de horizontalidad

	    	for (int s = 0; s < i; ++s)
	        {
	    		printf(" ");
	        }
	    	printChar('*');
	    	printf(" ");

	        // imprimir cada columna
	        for (int j = 0; j < DIMENSION; ++j)
	        {
	            printChar(plane->board2D[i][j]);
	            printf(" ");
	        }

	        // terminar la fila con '*'
	        printChar('*');
	        printChar('\n');
	    }

	    // fila final de asteriscos
	    for (int s = 0; s < DIMENSION - 2; ++s)
	    {
	        printChar(' ');
	        printChar(' ');
	    }
	    for (int j = 0; j < DIMENSION + 2; ++j)
	    {
	        printChar('*');
	        printChar(' ');
	    }
	    printChar('\n');

	    free(plane);
}

//Función que muestra la primera diagonal
void displayD1(void)
{
	Plane* p = obtainPlane(0, D1);

	for(int i=0; i < DIMENSION+2; ++i)
	{
		printChar('*');
		printf(" ");
	}
	printf("\n");


	for(int i=0; i < DIMENSION; ++i)
	{
		printChar('*');
		printf(" ");
		for(int j=0; j < DIMENSION; ++j)
		{
			printChar(p->board2D[i][j]);
			printf(" ");
		}
		printChar('*');
		printf("\n");
	}

	for(int i=0; i < DIMENSION+2; ++i)
		{
			printChar('*');
			printf(" ");
		}
		printf("\n");

	freePlane(p);
}

//Función que muestra la segunda diagonal
void displayD2(void)
{

	Plane* p = obtainPlane(0, D2);


	for(int i=0; i < DIMENSION+2; ++i)
	{
		printChar('*');
		printf(" ");
	}
	printf("\n");


	for(int i=0; i < DIMENSION; ++i)
	{
		printChar('*');
		printf(" ");
		for(int j=0; j < DIMENSION; ++j)
		{
			printChar(p->board2D[i][j]);
			printf(" ");
		}
		printChar('*');
		printf("\n");
	}

	for(int i=0; i < DIMENSION+2; ++i)
			{
				printChar('*');
				printf(" ");
			}
			printf("\n");



	freePlane(p);
}

void printChar(char c)
{
    switch (c) {
        case '1':
            printf("\033[31m1\033[0m"); // imprimir en rojo
            break;
        case '2':
            printf("\033[34m2\033[0m"); // imprimir en azul
            break;
        case '3':
            printf("\033[38;5;226m3\033[0m");
            break;
        case '4':
            printf("\033[32m4\033[0m");
            break;
        case '0':
            printf("*"); // solo un asterisco
            break;

        case '*':
        	printf("\033[33m*\033[0m");
        	break;

        default:
            printf("%c", c);
            break;
    }
}
