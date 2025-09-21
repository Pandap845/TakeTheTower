/*
 ============================================================================
 Name        : Proyecto.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

//Mracos
#define ROWS 4
#define COLUMNS 4
#define FLOORS 4

//Estructuras

typedef struct
{
	char id; //1 //2
	char ticket;
	char marble;
} Player;


typedef struct
{
	char*** board3D;
} Tower;


typedef struct
{
	char** board2D;
} Plane;


typedef struct
{
	short x;
	short y;
	short z;
} Point3D;


typedef struct
{
	short x;
	short y;
} Point2D;





enum AXIS{
	X,
	Y,
	Z
};


enum TURN {

	LEFT,
	RIGHT
};








//Variables globalbes
Tower* tower;
Player* player1;
Player* player2;

//Prototipos de las funciones

//Funciones de juego
void playerTurn(Player* player, Point3D *p); //Función que permite a determinado jugador realizar su turno
void ticket( Player* player, int turn); //Función que permite al usuario cambiar de plano
void placeMarble(Point3D *p); //Da ingreso de colocación de canica
int verifyWin(Player* player, Point3D *p); //Función que checa si ganó el jugador en su turno.
int verifyPlane(Plane* plane, Point2D *p);
void checkAllTower(void); //Función que checa todos los planos para validar antes de realizar el giro
Tower* copyTower(Tower* tower); //Función que genera una copia temporal de la torre antes de realizar el cambio
Plane* obtainPlane(int number, int axis);
Point2D* obtainPoint2D(Point3D* p, int axis);

//funciones incializadoras
Tower* initTower(void);
Player* initPlayer(char id);
Plane* initPlane(void);
Point3D* initPoint3D(void);
Point2D* initPoint2D(void);


//Funciones GUI
void menu(void); //Función para el menu principal
void menuTower(int *resultado,int *turn, char *c); //Función para mostrar el estado de la torre
void updateMenu(char *c,int *axis,int *plano);
void quitDialog(int *resultado);
void displayStructure(int num,int axis);
void displayZ(int num);
void displayY(int num);
void displayX(int num);
void displayD(int num); //Solo dos planos lol
void displayCredits(void);
void selectPlaneToShow();
void printChar(char c);
char readKeyboard(void);
void clearBuffer(void);
void enterKey(void);

/* Lista de caras (cada cara tiene 4 puntos en 2D: x,y) */



//-------------------------------------------
void dummyData(void); //Función para testear llenando la torre de canicas

int main(void) {

	//inicializar torre y jugadores
	tower = initTower();
	player1 = initPlayer('1');
	player2 = initPlayer('2');

	menu();

	free(tower); free(player1); free(player2); //Liberar memoria
}

//Funciones de testing

void dummyData(void)
{
	int z=0;
	int x=0;
	int y=0;

	 for (z = 0; z < 4; z++) {
	        char value;
	        if (z % 3 == 0) value = '0';
	        else if (z % 3 == 1) value = '1';
	        else value = '2';
	        for (x = 0; x <4 ; x++) {
	            for (y = 0; y < 4; y++) {
	                tower->board3D[x][y][z] = value;
	            }
	        }
	    }
}



//Funciones inicializadoras

Plane* initPlane(void)
{
	Plane* p;
	p = (Plane*)malloc(sizeof(Plane));
	p->board2D = (char**)malloc(sizeof(char**)*ROWS);

	for(int i=0; i< ROWS; ++i)
	{
		p->board2D[i] = (char*)malloc(sizeof(char*)*COLUMNS);
		 memset(p->board2D[i], '0', COLUMNS);  //inicializar en 0
	}


	return p;
}


void clearPlane(Plane* plane)
{

	for(int i=0; i< ROWS; ++i)
	{
		 memset(plane->board2D[i], '0', COLUMNS);
	}

}

Player* initPlayer(char id)
{
	Player *p;
	p = (Player*)malloc(sizeof(p));

	p->id = id;
	p->ticket = 4;
	p->marble = 32;

	return p;
}

Tower* initTower(void)
{
	Tower *t;
	t = (Tower*)malloc(sizeof(Tower));

	t->board3D  = (char***)malloc(sizeof(char***)*FLOORS);
	memset(t->board3D, '0', sizeof(FLOORS));

	for(int i=0; i < FLOORS; ++i)
	{
		t->board3D[i] = (char**)malloc(sizeof(char**)*ROWS);
		memset(t->board3D[i], '0', sizeof(ROWS));
		for(int j=0; j < ROWS; ++j)
		{
			t->board3D[i][j] = (char*)malloc(sizeof(char*)*COLUMNS);
			memset(t->board3D[i][j], '0', sizeof(COLUMNS));
		}
	}

	 //inicializar en 0
	return t;
}

Point3D* initPoint3D(void)
{
	Point3D* p;
	p = (Point3D*)malloc(sizeof(Point3D));
	return p;

}
Point2D* initPoint2D(void)
{
	Point2D* p;
	p = (Point2D*)malloc(sizeof(Point2D));
	return p;

}

//--------------------------------
//Cuerpo de las funciones
//--------------------------------
Plane* obtainPlane(int number, int axis)
{


	Plane* plane =  initPlane();

	//Se pasan dos argumentos.
	//El numero del plano cuando el eje Axis es fijo. De esa forma se diferencian
	//Entonces:
	switch(axis)
	{

	case X:
		for(int y=0; y <COLUMNS; y++)
			for(int z=0; z < FLOORS; z++)
				plane->board2D[y][z] = tower->board3D[number][z][y];
		break;

	case Y:
		for(int x=0; x <ROWS; x++)
					for(int z=0; z < FLOORS; z++)
						plane->board2D[x][z] = tower->board3D[z][number][x];
		break;

	case Z:
		for(int x=0; x < ROWS; x++)
					for(int y=0; y < COLUMNS; y++)
						plane->board2D[x][y] = tower->board3D[x][y][number];
		break;

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


void menu(void)
{
	int var,menu;
	char c;

	do {
		int resultado = 0; //0 => Juego en transcurrencia \\ 1 => Gana jugador 1 \\ 2 => Gana jugador 2 \\ 3 => Se salió de la partida
		int turn = 0; // 0 => jugador 1 \\ 1 => jugador 2
		system("cls");
		printf("=========================================\n");
		printf("JUEGO CUYO NOMBRE NO RECUERDO!!!\n");
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
		case 1:
			while (resultado == 0)
				menuTower(&resultado,&turn,&c);
			break;

		case 2:
			//	displayCredits();
			enterKey();
			break;

		case 0:
			printf("\nHasta luego!\n");
			break;

		default:
			printf("\nTecla incorrecta. Intente de nuevo\n\n");
		}

	} while (var != 1 || menu != 0);
}

void menuTower(int *resultado,int *turn, char *c)
{
	int axis = 0;
	int plano = 0;

	int playerid = (*turn == 0) ? 1 : 2;

	while(1) //Bucle infinito, en señal que se estará iterando todo el tiempo
	{
		do {
			system("cls");
			printf("Mostrando plano fijo ");
			if(axis==X)
				printf("X");
			else if(axis == Y)
				printf("Y");
			else
				printf("Z");
			printf("\n Plano: %d\n", plano+1 );
				displayStructure(plano, axis);

			//	clearBuffer(); //método para limpiar el buffer
			printf("\nPulse las teclas para realizar:"
					"\nArriba:    Cambiar eje arriba"
					"\nAbajo:     Cambiar eje abajo"
					"\nIzquierda: Mover plano izquierdo"
					"\nDerecha:   Mover plano derecho"
					"\ne:         Pasar turno (Jugador %d)"
					"\nq:         Salir\n",playerid);
			*c = readKeyboard();

			updateMenu(c,&axis,&plano);

		} while (*c != 'q' && *c != 'e');

		break;
	}

	if (*c == 'q')
		//Dar diálogo de salida para asegurar que se termine el juego a propósito
		quitDialog(resultado);
	else if (*c == 'e')
	{
		//Jugar
		Point3D *p = initPoint3D();
		if (playerid == 1)
			playerTurn(player1,p);
		else
			playerTurn(player2,p);
		//Implementación de los turnos
		*turn = !(*turn); //cambia jugador
	}
}

void updateMenu(char *c, int *axis, int *plano)
{
	switch (*c)
	{
		case 'u': //arriba
			if(*axis+1 > 2) *axis = 0;
			else (*axis)++;

			break;

		case 'd':
			if(*axis-1 < 0) *axis = 2;
			else (*axis)--;

			break;

		case 'l':
			if(*plano-1 < 0) *plano = 3;
			else (*plano)--;

			break;

		case 'r':
			if(*plano + 1 > 3) *plano = 0;
			else (*plano)++;

			break;
		}
}

void quitDialog(int *resultado)
{
	int input,var;

	do {
		system("cls");
		printf("\nRealmente quiere salir?"
				"\nSi (1)"
				"\nNo (2)\n");
		var = scanf("%d",&input);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 && input != 1 && input != 2);

	if (input == 1) *resultado = 3;
}

void displayStructure(int num,  int axis)
{
	//Seleccionar que pantalla mostrar
	switch(axis)
	{
	case X:
		displayX(num);
		break;

	case Y:
		displayY(num);
		break;
	case Z:
		displayZ(num);
		break;




	}

}


//Función que imprime un tablero horizontal

void displayZ(int num)
{
    // 1. Obtener el plano

    Plane* plane = obtainPlane(num, Z);


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


void displayY(int num)
{
    // 1. Obtener el plano

    Plane* plane = obtainPlane(num, Y);


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


    free(plane);

}


//Función para imprimir torre con Eje X fijo

void displayX(int num)
{
	 // 1. Obtener el plano

	    Plane* plane = obtainPlane(num, X);
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

void diplayCredits(void)
{
	printf("\n(c) 2025. Victor Emiliano Rodriguez Aguila\n");
	printf("Joshua\n");

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
            printf("%c", c); // basura lol
            break;
    }
}




void playerTurn(Player *player, Point3D *p) //Función que permite a determinado jugador realizar su turno
{
	int x,var,validTurn;

	do {
		system("cls");
		printf("JUGADOR %c - TICKETS: %d            \n\n"
				"Selecciona una accion:               \n"
				"Colocar una canica                (1)\n"
				"Girar la tabla (Costo: 1 ticket)  (2)\n",player->id,player->ticket);
		var = scanf("%d",&x);
		if (var != 1)
			setbuf(stdin,NULL);
		else if (x == 1 && player->ticket == 0)
		{
			printf("\nNo tienes suficientes tickets, pasando a colocar una canica.\n");
			x = 2;
		}
	} while (var != 1 && (x < 0 || x > 2));

	do {
		validTurn = 1;
		if (x == 1)
		{
			placeMarble(p);
			if (tower->board3D[p->x][p->y][p->z] != '0')
			{
				printf("\nYa hay una canica en ese espacio, intenta de nuevo.\n");
				validTurn = 0;
			} else
				tower->board3D[p->x][p->y][p->z] = player->id;
		}
		//else
			//Ticket();
			//player->ticket -= 1;

	} while (validTurn == 0);

	player->marble -= 1;
}

void placeMarble(Point3D *p)
{
	int x,y,z,var;
	do {
		printf("Declare la capa en el cual quieres posicionar tu canica (1-4):");
		var = scanf("%d",&x);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || x < 0 || x > 4);
	p->x = x-1;

	do {
		printf("Declare la fila en el cual quieres posicionar tu canica (1-4):");
		var = scanf("%d",&y);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || y < 0 || y > 4);
	p->y = y-1;

	do {
		printf("Declare la columna en el cual quieres posicionar tu canica (1-4):");
		var = scanf("%d",&z);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || z < 0 || z > 4);
	p->z = z-1;
}

void ticket( Player* player, int turn)
{
	//Función que permite al usuario cambiar de plano
}



//Función que verifica si ALGUIEN ganó
int verifyPlane(Plane* plane, Point2D* p2d)
{
	int p1=0, p2=0;
	int result;

	//Verificar verticalmente
	for(int x=0; x < 4; ++x)
	{
		if(plane->board2D[x][p2d->y] == '1') ++p1;
		if(plane->board2D[x][p2d->y] == '2') ++p2;
	}

	result = p1==4? 1: p2 ==4? 2: 0; //indica si alguien ganó o nada

	if(result!=0) return result; //Ya se habrá identificado un ganador

	//limpiar variables
	p1=0, p2=0;

	//verificar horizontalmente
	for(int y=0; y < 4; ++y)
		{
			if(plane->board2D[p2d->x][y] == '1') ++p1;
			if(plane->board2D[p2d->x][y] == '2') ++p2;
	}

	result = p1==4? 1: p2 ==4? 2: 0; //indica si alguien ganó o nada

	if(result!=0) return result; //Ya se habrá identificado un ganador

	//Verificar diagonalmente
	//debido a que solo existen dos diagonales, basta con comprobar ambas
	p1=0, p2=0;

	//Primera diagonal
	for(int i=0; i < 4; ++i)
	{
		if(plane->board2D[i][i] == '1') ++p1;
		if(plane->board2D[i][i] == '2') ++p2;
	}

	result = p1==4? 1: p2 ==4? 2: 0; //indica si alguien ganó o nada
	if(result!=0) return result; //Ya se habrá identificado un ganador

	p1=0, p2=0;
	//Segunda diagonal
	for(int i=0; i < 4; ++i)
	{
		if(plane->board2D[i][3-i] == '1') ++p1;
		if(plane->board2D[i][3-i] == '2') ++p2;
	}
		result = p1==4? 1: p2 ==4? 2: 0; //indica si alguien ganó o nada

		if(result!=0) return result; //Ya se habrá identificado un ganador

	return result; //Al final de todo se retorna lo obtenido
}

int VerifyWin(Player* player, Point3D *p)
{
	//Función que checa si ganó el jugador en su turno.
	//Nos basamos en la lógica de que, siemmpre un punto hace intersección en 3 planos
	Plane* plane;
	Point2D* p2d;
	int result=0;


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
		result = verifyPlane(plane, p2d);




		//Estar liberando la memoria del plano
		free(plane);
		if(result!=0) return result;


	}
	return result;
}




void checkAllTower(void)
{
	//Función que checa todos los planos para validar antes de realizar el giro
}


Tower* copyTower(Tower* tower)
{
	Tower* t = initTower();

	for(int x=0; x < 4; x++)
	{
		for(int y=0; y < 4; y++)
		{
			for(int z = 0; z < 4; z++)
			{
				t->board3D[x][y][z] = tower->board3D[x][y][z];
			}
		}
	}
	return t;
	//Función que genera una copia temporal de la torre antes de realizar el cambio
}


//-------------------------
//FUnciones auxiliares


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

void clearBuffer(void)
{
	int c;
	//Continua leyendo hasta que se limpió completamente
	while ((c = getchar()) != '\n' && c != EOF) { }
}

void enterKey(void)
{
	getchar();
}
