/*
 ============================================================================
 Name        : Proyecto.c
 Author      : Emi & Josh
 Version     : 2.0
 Copyright   : Your copyright notice
 Description : Take The Tower in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <pthread.h>

//Mracos
#define ROWS 4
#define COLUMNS 4
#define FLOORS 4

//Estructuras

typedef struct
{
	char id; //1 //2
	int ticket;
	int marble;
} Player; //Representa la información de un jugador

typedef struct
{
	char*** board3D;
} Tower; //Representa la información de la torre


typedef struct
{
	char** board2D;
} Plane; //Representa la informacióin de cada uno de los planos de la torrre


typedef struct
{
	int x;
	int y;
	int z;
} Point3D; //Representa un punto 3D



typedef struct
{
	int x;
	int y;
} Point2D; //Representa un punto 2D


enum AXIS{
	X,
	Y,
	Z,
	D1,
	D2
};


enum TURN {

	LEFT,
	RIGHT
};

//Variables globales
Tower* tower;
Player* player1;
Player* player2;

//Prototipos de las funciones

//Funciones GUI
void menu(void); //Función para el menu principal
void menuTower(int *resultado,int *turn, int *menu,int *axis,int *plano); //Función para mostrar el menu del juego
void displayTower(int n,int *axis,int *plano,char *c);
void updateMenu(char *c,int *axis,int *plano); //Función para actualizar el plano y eje de la vista de la torre
int quitDialog(void); //Menu para salir
void endScreen(int *resultado,int *turn,int *menu); //Menu de final del juego
void displayStructure(int n,int axis); //Función para imprimir el plano y eje
void displayZ(int n); //Función para imprimir el eje Z
void displayY(int n); //Función para imprimir el eje Y
void displayX(int n); //Función para imprimir el eje X
void displayD1(void); //Función para imprimir diagonal 3D
void displayD2(void); //Función para imprimir antidiagonal 3D
void printChar(char c); //Impresión de carácteres con colores
char readKeyboard(void); //Lectura de teclas para el menu de la torre
void displayCredits(void); //Desplegar créditos
void enterKey(void);

//Funciones de juego
void playerTurn(Player* player,int *resultado,int *turn,int *axis,int *plano); //Función que permite a determinado jugador realizar su turno
void turnDialog(Player* player,int *x); //Función que da el diálogo principal para las acciones del jugador
void placeMarble(Player *player,int *resultado, int *validTurn); //Da ingreso de colocación de canica
void ticket(Player* player,int *resultado,int *validTurn); //Función que permite al usuario cambiar de plano
void verifyDiagonals(int axis,int *resultado); //Función que verifica una diagonal de tercera dimensión del tablero
void verifyWin(Point3D *p, int *resultado); //Función que checa si ganó el jugador en su turno.
void verifyPlane(Plane* plane, int *resultado, int line, int n);
void checkAllTower(Tower* t, Plane* p, int index, int* resultado); //Función que checa todos los planos para validar antes de realizar el giro
Tower* copyTower(Tower* tower); //Función que genera una copia temporal de la torre antes de realizar el cambio
Plane* obtainPlane(int n, int axis); //Función que obtiene el plano a analizar
Point2D* obtainPoint2D(Point3D* p, int axis); //Función que transforma un punto 3D a 2D

//funciones incializadoras
Tower* initTower(void);
Player* initPlayer(char id);
Plane* initPlane(void);
Point3D* initPoint3D(void);
Point2D* initPoint2D(void);

//Funciones para limpieza de memoria
void freeTower(Tower* t);
void freePlane(Plane* p);
void reset(void);

//Funciones de giro
Plane* turn90Left(Plane* p);
Plane* turn90Right(Plane* p);

//-------------------------------------------

int main(void) {

	//inicializar torre y jugadores
	tower = initTower();
	player1 = initPlayer('1');
	player2 = initPlayer('2');

    tower->board3D[0][0][0] = '1'; tower->board3D[1][1][1] = '1'; tower->board3D[2][2][2] = '1';
    tower->board3D[0][1][0] = '2'; tower->board3D[1][2][1] = '2'; tower->board3D[2][3][2] = '2';
	menu();

	freeTower(tower);
	free(player1);
	free(player2); //Liberar memoria
}

//Funciones inicializadoras

Plane* initPlane(void)
{
    Plane* p = (Plane*)malloc(sizeof(Plane));
    if(!p)
    {
        printf("No se inicializo la memoria del plano\n");
        exit(-1);
    }

    p->board2D = (char**)malloc(sizeof(char*)*ROWS);
    if(!(p->board2D))
    {
        printf("No se inicializo la memoria del tablero del plano\n");
        exit(-2);
    }

    for(int i = 0; i < ROWS; ++i)
    {
        p->board2D[i] = (char*)malloc(sizeof(char)*COLUMNS);
        if(!(p->board2D[i]))
        {
            printf("No se inicializo la memoria de la fila %d del plano\n", i);
            exit(-3);
        }

        memset(p->board2D[i], '0', COLUMNS);  // inicializar en '0'
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
	p->marble = 32;

	return p;
}


//Función que inicializa en memoria a la torre
Tower* initTower(void) {
    Tower *t = (Tower*)malloc(sizeof(Tower));
    if(!t) {
        printf("No se inicializo adecuadamente la torre\n");
        exit(-1);
    }

    t->board3D  = (char***)malloc(sizeof(char**)*FLOORS);
    if(!(t->board3D)) {
        printf("No se inicializo la memoria del tablero 3D\n");
        exit(-2);
    }

    for(int i = 0; i < FLOORS; ++i) {
        t->board3D[i] = (char**)malloc(sizeof(char*)*ROWS);
        if(!(t->board3D[i])) {
            printf("No se inicializo adecuadamente la memoria para las filas del piso %d\n", i);
            exit(-3);
        }

        for(int j = 0; j < ROWS; ++j) {
            t->board3D[i][j] = (char*)malloc(sizeof(char)*COLUMNS);
            if(!(t->board3D[i][j])) {
                printf("No se inicializo adecuadamente la memoria de la fila %d del piso %d\n", j, i);
                exit(-4);
            }

            memset(t->board3D[i][j], '0', COLUMNS);
        }
    }

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
//Cuerpo de las funciones
//-------------------------------
Plane* obtainPlane(int n, int axis)
{
	Plane* plane =  initPlane();
	int x,y,z;

	//Se pasan dos argumentos.
	//El numero del plano cuando el eje Axis es fijo. De esa forma se diferencian
	//Entonces:

	for(int i=0; i <COLUMNS; i++) {
		for(int j=0; j < FLOORS; j++) {
			switch (axis)
			{
			case X:
				x = n; y = i; z = j; break;
			case Y:
				x = i; y = n; z = j; break;
			case Z:
				x = i; y = j; z = n; break;
			case D1:
				x = i; y = i; z = j; break;
			case D2:
				x = i; z = j; y = (COLUMNS - 1) - j; break;
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

	for(int x=0; x < FLOORS; x++)
	{
		for(int y=0; y < ROWS; y++)
		{
			for(int z = 0;   z < COLUMNS  ; z++)
			{
				t->board3D[x][y][z] = tower->board3D[x][y][z];
			}
		}
	}
	return t;
}

//Función de giro
Plane* turn90Left(Plane* p)
{

    Plane* ptr = initPlane();

    if(!ptr) return NULL;
    for(int i=0; i < ROWS; ++i)
        for(int j=0; j < COLUMNS; ++j)
            ptr->board2D[COLUMNS-j-1][i] = p->board2D[i][j]; //Se realiza el intercambio
    //Si todo sale bien

    return ptr; //Se regresa un 0, en señal de que salio bie
}

Plane* turn90Right(Plane* p)
{

	Plane* ptr = initPlane();
     if(!ptr) return NULL;

    //Hacer la rotacióm
    for(int i=0; i < ROWS; ++i)
        for(int j=0; j < COLUMNS; ++j)
                ptr->board2D[j][ROWS-i-1] = p->board2D[i][j];

    return ptr;
}

//Empiezo de GUI
void menu(void)
{
	int var,menu;

	do {
		int resultado = 0; //0 => Juego en transcurrencia \\ 1 => Gana jugador 1 \\ 2 => Gana jugador 2 \\ 3 => Se salió de la partida
		int turn = 0; // 0 => jugador 1 \\ 1 => jugador 2
		int axis = 0,plano = 0; //Colocar posición y plano de vista
		system("cls");
		printf("=========================================\n");
		printf("           TAKE THE TOWER IN C           \n");
		printf("=========================================\n");

		printf("Menu:\n "
				"\nJugar        (1)"
				"\nVer creditos (2)"
				"\nSalir        (0)\n");
		var = scanf("%d", &menu);
		if (var != 1)
			setbuf(stdin,NULL);

		switch(menu)
		{
		case 1: //Entrar al juego
			while (resultado == 0)
                menuTower(&resultado,&turn,&menu,&axis,&plano);
			break;
		case 2: //Mostrar créditos
			displayCredits();
			break;
		case 0:
			printf("\nHasta luego!\n");
		}
	} while (var != 1 || menu != 0);
}

void menuTower(int *resultado,int *turn, int *menu,int *axis, int *plano)
{
	char c;

	int playerid = (*turn == 0) ? 1 : 2;

	do { //Bucle para desplegar la torre mientras no se presiona 'q' o 'e'
        displayTower(playerid,axis,plano,&c);
	} while (c != 'q' && c != 'e');

	if (c == 'q' && quitDialog()) //Si se presiona 'q' y quitDialog() == true, se sale del juego y se reinicia
		*resultado = 3;
	else if (c == 'e') //Si se presiona 'e', se pasa a la pantalla de turnos.
	{
		//Jugar
		if (playerid == 1)
			playerTurn(player1,resultado,turn,axis,plano);
		else
			playerTurn(player2,resultado,turn,axis,plano);

		if (player1->marble == 0 && player2->marble == 0)
			*resultado = 3;
		if (*resultado != 0)
			endScreen(resultado,turn,menu);
	}
}

//Función que muestra los metadatos del plano a visualizar
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
     printf("\ne:         Pasar turno (Jugador %d)",n); //Si playerid == 1 || 2, si no, es menu final y no deberá dejar pasar un turno
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
		if(*axis+1 > 4)
			*axis = 0;
		else
			(*axis)++;
		break;

	case 'd': //abajo
		if(*axis-1 < 0)
			*axis = 4;
		else
			(*axis)--;
		break;

	case 'l': //izquierda
		if(*plano-1 < 0)
			*plano = 3;
		else
			(*plano)--;
		break;

	case 'r': //derecha
		if(*plano + 1 > 3)
			*plano = 0;
		else
			(*plano)++;
		break;
	}
}

//Función que muestra pantalla de confirmación
int quitDialog(void)
{
	int input,var;

	do {
		system("cls");
		printf("\nRealmente quiere salir?"
				"\nSi (1)"
				"\nNo (0)\n");
		var = scanf("%d",&input);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || (input != 0 && input != 1));

    switch (input){
    case 1:
        reset();
        return 1;
    case 0:
        return 0;
    }

    return 0;
}


//Pantalla final del juego. Permite visualizar el tablero al finalizar
void endScreen(int *resultado,int *turn,int *menu)
{
	int input,var;

	do {
		system("cls");
		if (*resultado == 3)
			printf("\nEMPATE - SE HAN AGOTADO LAS CANICAS\n");
		else
			printf("\nFELICIDADES - JUGADOR %d HA GANADO!\n",*resultado);
		printf("\nVer tabla final     (1)"
			   "\nJugar de nuevo      (2)"
			   "\nIr al menu          (3)\n");
		var = scanf("%d",&input);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || input < 1 || input > 3);

	switch (input) {
	case 1:
	    //Uso de la función de despliegue de la torre para mostrar la tabla final sin funcionalidad real del juego
	    while (1) //Ciclo para regresar en caso de que no quiera salir en el dialogo de salida
        {
            int axis = 0, plano = 0;
            char c;

            do {
                displayTower(0,&axis,&plano,&c); //Función de despliegue
            } while (c != 'q');

            if (c == 'q' && quitDialog()) break; //Si se quiere salir se rompe el ciclo y se va al menú principal
        }

		break;
	case 2:
		reset(); //Libera la memoria de torre y jugadores para inicializarlos de nuevo
		*resultado = 0;
		*turn = 0;
		break;
	case 3:
	    reset();
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
		displayY(ROWS-n-1);
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
    for (int s = 0; s < COLUMNS; ++s)
        {
            printChar('*');
            printf(" ");
        }
    	printf("\n");

    for (int i = 0; i < ROWS; ++i)
    {
        //Espacio en blanco que da efecto de horizontalidad

    	for (int s = 0; s < i; ++s)
        {
    		printf("  ");
        }
    	printChar('*');
    	printf(" ");

        // imprimir cada columna
        for (int j = 0; j < COLUMNS; ++j)
        {
            printChar(plane->board2D[i][j]);
            printChar(' ');
        }

        // terminar la fila con '*'
        printChar('*');
        printChar('\n');
    }

    // fila final de asteriscos
    for (int s = 0; s < ROWS; ++s)
    {
        printChar(' ');
        printChar(' ');
    }
    for (int j = 0; j < COLUMNS; ++j)
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
    for(int i=0; i < COLUMNS; ++i)
    {
    	printChar('*');
    	printf(" ");
    }
    printf("\n");

    for(int i=0; i < ROWS; ++i)
    {

    	//Imprimir espacios
    	for(int s= i; ROWS-s > 0; ++s )
    	{
    		printf(" ");
    	}

    	printChar('*');
    	printf(" ");
    	for(int j=0; j < COLUMNS; ++j)
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
    for(int i=0; i < COLUMNS; ++i)
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
	    for (int s = 0; s < COLUMNS + 2; ++s)
        {
	        printChar('*');
	        printf(" ");
	    }
	    printf("\n");

	    for (int i = 0; i < ROWS; ++i)
	    {
	        //Espacio en blanco que da efecto de horizontalidad

	    	for (int s = 0; s < i; ++s)
	        {
	    		printf(" ");
	        }
	    	printChar('*');
	    	printf(" ");

	        // imprimir cada columna
	        for (int j = 0; j < COLUMNS; ++j)
	        {
	            printChar(plane->board2D[i][j]);
	            printf(" ");
	        }

	        // terminar la fila con '*'
	        printChar('*');
	        printChar('\n');
	    }

	    // fila final de asteriscos
	    for (int s = 0; s < ROWS - 2; ++s)
	    {
	        printChar(' ');
	        printChar(' ');
	    }
	    for (int j = 0; j < COLUMNS + 2; ++j)
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

	for(int i=0; i < COLUMNS+2; ++i)
	{
		printChar('*');
		printf(" ");
	}
	printf("\n");


	for(int i=0; i < ROWS; ++i)
	{
		printChar('*');
		printf(" ");
		for(int j=0; j < COLUMNS; ++j)
		{
			printChar(p->board2D[i][j]);
			printf(" ");
		}
		printChar('*');
		printf("\n");
	}

	for(int i=0; i < COLUMNS+2; ++i)
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


	for(int i=0; i < COLUMNS+2; ++i)
	{
		printChar('*');
		printf(" ");
	}
	printf("\n");


	for(int i=0; i < ROWS; ++i)
	{
		printChar('*');
		printf(" ");
		for(int j=0; j < COLUMNS; ++j)
		{
			printChar(p->board2D[i][j]);
			printf(" ");
		}
		printChar('*');
		printf("\n");
	}

	for(int i=0; i < COLUMNS+2; ++i)
			{
				printChar('*');
				printf(" ");
			}
			printf("\n");



	freePlane(p);
}

void displayCredits(void)
{
    system("cls");
	printf("\n(c) 2025.\n"
           "Victor Emiliano Rodriguez Aguila\n"
           "& Joshua David DeBono Rios\n\n"
           "Presiona una tecla para regresar.");
	enterKey();
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
        case '0':
            printf("*"); // solo un asterizco
            break;

        case '*':
        	printf("\033[33m*\033[0m");
        	break;

        default:
            printf("%c", c);
            break;
    }
}

//Inicializar formalmente el turno del jugador
void playerTurn(Player *player,int *resultado,int *turn,int *axis,int *plano) //Función que permite a determinado jugador realizar su turno
{
	int input,validTurn;

    do {
        validTurn = 1;
        turnDialog(player,&input);

        switch (input) {
        case 1:
            placeMarble(player,resultado,&validTurn);
            break;
        case 2:
            ticket(player,resultado,&validTurn);
            if(validTurn) player->ticket-=1;
        }
    } while (!validTurn); //Si se intenta girar, se da chance en caso que sea un giro inválido para que se regrese al diálogo principal.

    if (input) {
        *turn = !(*turn); //cambia de jugador si se terminó de ejecutar un turno
        *axis = 0;
        *plano = 0;
    }
}

void turnDialog(Player* player,int *x)
{
    int var;

    do {
		system("cls");
		printf("JUGADOR %c - TICKETS: %d            \n\n"
				"Selecciona una accion:               \n"
				"Colocar una canica                (1)\n"
				"Girar la tabla (Costo: 1 ticket)  (2)\n"
				"Regresar a ver el tablero         (0)\n",player->id,player->ticket);
		var = scanf("%d",x);

		if (var != 1)
			setbuf(stdin,NULL);
		else if (*x == 2 && player->ticket == 0)
		{
			printf("No tienes suficientes tickets, pasando a colocar una canica.\n");
			*x = 1;
		}
	} while (var != 1 || *x < 0 || *x > 2);
}

//Solicitar posición en donde el jugador quiera poner su canica
void placeMarble(Player *player,int* resultado,int* validTurn)
{
    Point3D *p = initPoint3D();

    for (int i = 0; i < 3; i++) //Input Dialog (se hace 3 veces, para cada nivel de coordenada)
    {
        int input,var;
        do {
            switch (i) {
            case X: printf("\nDeclare la capa en el cual quieres posicionar tu canica (1-4): "); break;
            case Y: printf("\nDeclare la fila en el cual quieres posicionar tu canica (1-4): "); break;
            case Z: printf("\nDeclare la columna en el cual quieres posicionar tu canica (1-4): ");
            }
            var = scanf("%d",&input);
            if (var != 1)
                setbuf(stdin,NULL);
        } while (var != 1 || input < 0 || input > 4);

        switch (i) {
        case X: p->x = input-1; break;
        case Y: p->y = input-1; break;
        case Z: p->z = input-1;
        }
    }

	if (tower->board3D[p->x][p->y][p->z] != '0')
    {
        *validTurn = 0;
		printf("\nYa hay una canica en ese espacio, presiona una tecla e intenta de nuevo.\n");
		enterKey();
	} else {
		tower->board3D[p->x][p->y][p->z] = player->id;
		//Verificar tabla en ese punto
		verifyDiagonals(D1,resultado);
		if (!(*resultado)) verifyDiagonals(D2,resultado);
		if (!(*resultado)) verifyWin(p,resultado);

		player->marble -= 1;
		printf("\nCanica colocada con exito! Presiona una tecla para continuar.\n");
		enterKey();
	}

	free(p);
}

//Solicitar información del giro y validarlo
void ticket(Player* player,int *resultado,int *validTurn)
{
    int index, direction, var;

    // Pedir al jugador el plano a girar
    do {
        printf("Jugador %c, elige el plano (X fijo, 1-4): ", player->id);
        var = scanf("%d", &index);
        if (var != 1)
            setbuf(stdin, NULL);
    } while (var != 1 || index < 1 || index > 4);

    // Pedir dirección de giro
    do {
        printf("Elige la direccion:\n"
               "Izquierda (1)\n"
               "Derecha   (2)\n");
        var = scanf("%d", &direction);
        if (var != 1)
            setbuf(stdin, NULL);
    } while (var != 1 || (direction != 1 && direction != 2));

    // Crear copia de la torre como respaldo
    Tower* backup = copyTower(tower);

    // Obtener el plano fijo en X
    Plane* plane = obtainPlane(index - 1, X);

    // Rotar el plano
    Plane* rotated = (direction == 1) ? turn90Left(plane) : turn90Right(plane);

    // Aplicar el plano rotado en la torre real
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
            tower->board3D[index-1][i][j] = rotated->board2D[i][j];

    // Verificar si alguien ganó tras el giro
    checkAllTower(tower, plane, index-1, resultado);

    if (!(*resultado)) { //Verifica si el giro fue valido
        printf("\nEl plano fue girado con exito! Presiona una tecla para continuar.\n");
        enterKey();
    } else {
        // Restaurar desde el respaldo
        for (int x = 0; x < FLOORS; x++)
            for (int y = 0; y < ROWS; y++)
                for (int z = 0; z < COLUMNS; z++)
                    tower->board3D[x][y][z] = backup->board3D[x][y][z];
        *validTurn = 0;
        *resultado = 0;
        printf("\nMovimiento invalido: alguien ganaria con este giro. Presiona una tecla e intenta de nuevo.\n");
        enterKey();
    }

    // Liberar memoria
    freePlane(plane);
    freePlane(rotated);
    freeTower(backup);
}

/*
 * PRIMERA Y PRINCIPAL FUNCIÓN DE LÓGICA
 * Toma las coordenadas de la canica puesta/que se quiere validar,
 * organiza la verificación en los planos que intersecta ese punto
 * y se hace la validación de que si forma una linea de 4 en lo horizontal,
 * vertical o diagonal.
 */

void verifyDiagonals(int axis,int *resultado)
{
    Plane* plane = obtainPlane(0, axis);
    verifyPlane(plane,resultado,2,0); //Checar primera diagonal
	if (*resultado) {free(plane); return;}
	verifyPlane(plane,resultado,3,0); //Checar segunda diagonal
	if (*resultado) {free(plane); return;}

	free(plane);
}

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
	int p1=0, p2=0;
	int x=0,y=0;
	for(int i=0; i < 4; i++)
	{
		switch (line) {
		case 0: x = i; y = n; break;
		case 1: x = n; y = i; break;
		case 2: x = i; y = i; break;
		case 3: x = i; y = 3-i;
		}
		if (plane->board2D[x][y] == '1') p1++;
		if (plane->board2D[x][y] == '2') p2++;
	}
    //mutex
    //for i en cuantos jugadores hayan - si p[i] == 4 resultado = i+1, si no 0
    //si resultado != 0 parar los hilos y destruirlos
    //unlock mutex
	*resultado = (p1 == 4) ? 1 : (p2 == 4) ? 2 : 0;
}


void checkAllTower(Tower* t, Plane* p, int index, int* resultado)
{
	Point3D* point = initPoint3D(); //Se crea un punto 3D que se va a estar iterando para verificar todo el plano
	point->x = index; //Se determina el plano

    verifyDiagonals(D1,resultado);
    if (*resultado) {free(point); return;}
    verifyDiagonals(D2,resultado);
    if (*resultado) {free(point); return;}

    for (point->y = 0; point->y < ROWS; ++(point->y)) //Eje y
        for(point->z=0; point->z < COLUMNS; ++(point->z)) //Eje z
        {
            verifyWin(point, resultado);
            if(*resultado) {free(point); return;} //Salirse automaticamente si se identificó que alguien va a ganar
        }

     free(point); //liberar memoria
}

//-------------------------
//Fúnciones auxiliares

char readKeyboard(void)
{
	int c;

	while (1)
	{
		c = _getch();
		if (c == 0 || c == 224) {   // teclas especiales
			c = _getch();
			switch (c) {
			case 72: return 'u';
			case 80: return 'd';
			case 75: return 'l';
			case 77: return 'r';
			}
		} else if (c == 'q') {
			return 'q';
		} else if (c == 'e')
			return 'e';
	}

	return 0;

}

//Limpia el buffer y hace que se presione una tecla para continuar
void enterKey(void)
{
    setbuf(stdin,NULL);
	getchar();
}

//Funciones de liberación de memoria
//Función que limpia la memoria de la torre
//Función que limpia la memoria de la torre
void freeTower(Tower* t)
{
    if(!t) return;  // Si directamente se manda un NULL

    for(int i = 0; i < FLOORS; ++i)
    {
        for(int j = 0; j < ROWS; ++j)
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

    for(int i = 0; i < ROWS; ++i)
    {
        free(p->board2D[i]); // libera cada fila
    }

    free(p->board2D); // libera el arreglo de punteros a filas
    free(p);          // libera el plano
}


//Limpia la torre y los datos de cada jugador y los vuelve a inicializar para el próximo juego
void reset(void)
{
	freeTower(tower); free(player1); free(player2);
	tower = initTower();

	player1 = initPlayer('1');
	player2 = initPlayer('2');
}
