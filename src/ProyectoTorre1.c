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
	int id; //1 //2
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

//Prototipos de las funciones


void PlayerTurn( Player* player, Point3D *p); //Función que permite a determinado jugador realizar su turno
int useTicket( Player* player, enum TURN turn); //Función que permite al usuario cambiar de plano
int verifyWin(Player* player, Point3D *p); //Función que checa si ganó el jugador en su turno.
int verifyPlane(Plane* plane, Point2D* p);
void checkAllTower(void); //Función que checa todos los planos para validar antes de realizar el giro
Tower* copyTower(Tower* tower); //Función que genera una copia temporal de la torre antes de realizar el cambio



Plane* obtainPlane(int number, enum AXIS axis);
Point2D* obtainPoint2D(Point3D* p, enum AXIS axis);
//funciones incializadoras
Tower* initTower(void);
Plane* initPlane(void);
Point3D* initPoint3D(void);
Point2D* initPoint2D(void);


//Funciones GUI
void menuTower(void); //Función que muestra el estado actual de la torre
void displayStructure(int num,  enum AXIS axis);
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

//Funciones de limpieza
void freeTower(Tower* t);
void freePlane(Plane* P);


//Funciones matematicas





//-------------------------------------------
void dummyData(void); //Función para testear llenando la torre de canicas




int main(void) {

	//inicializar torre
	tower = initTower();


	dummyData();

	menuTower();
	free(tower); //liberar memoria
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
			memset(t->board3D[i][j], '1', sizeof(COLUMNS));
		}
	}

	 //inicializar en 0
	return t;
}

Point3D* initPoint3D(void)
{
	Point3D* p;
	p = (Point3D*)malloc(sizeof(Point3D));
			memset(p, '0', sizeof(Point3D)); //inicializar en 0
			return p;

}
Point2D* initPoint2D(void)
{
	Point2D* p;
	p = (Point2D*)malloc(sizeof(Point2D));
		memset(p, '0', sizeof(Point2D)); //inicializar en 0
		return p;

}

//--------------------------------
//Cuerpo de las funciones
//--------------------------------
Plane* obtainPlane(int number, enum AXIS axis)
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
Point2D* obtainPoint2D(Point3D* p, enum AXIS axis)
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


void menuTower(void)
{
	int menu = 0;
	enum AXIS axis = 0;
	int plano = 0;
	char c = '0';
	int turn = 0; // 0 => jugador 1 \\ 1 => jugador 2

	do
	{
	printf("=========================================\n");
	printf("JUEGO CUYO NOMBRE NO RECUERDO!!!\n");
	printf("=========================================\n");

	printf("Menu: \n1.Jugar \n0.Salir \n2.Ver creditos\n\n");
	scanf("%d", &menu);
	switch(menu)
	{
	case 1:
		while(1) //Bucle infinito, en señal que se estará iterando todo el tiempo
		{

			do
			{
			system("cls");
			printf("Mostrando plano fijo ");
			if(axis==X)
				printf("X");
			else if(axis == Y)
				printf("Y");
			else
				printf("Z");
			printf("\n Plano: %d\n", plano );

			displayStructure(plano, axis);
			printf("\n\n");



			//	clearBuffer(); //método para limpiar el buffer
			printf("\n\nAcciones: Pulse las teclas para realizar: \nArriba. Cambiar eje arriba \nAbajo. Cambiar eje abajo \nIzquierda: mover plano izquierdo \n Derecha. Mover plano derecho");
			printf("\nEnter.Jugar Jugador %c\n q.Salir\n", (turn == 0)?'1':'2');
			c = readKeyboard();
			switch (c)
			{
			case 'u': //arriba

					if(axis+1 > 2) axis = 0;
					else ++axis;
				break;
			case 'd':
					if(axis-1 < 0) axis = 2;
					else --axis;
				break;

			case 'l':
					if( plano - 1 < 0) plano = 3;
					else plano--;
				break;

			case 'r':
					if(plano + 1 > 3) plano = 0;
					else plano++;
				break;

			case 'e': //Jugar
				turn = !turn; //cambia jugador
				//Implementación de los turnos
				break;

			case 'q':
				printf("\nTerminando partida. Regresando a menu. Pulse Enter para continuar...");
				enterKey();
				break;


			}

			}while(c != 'q');



			if(c == 'q')
			{
				system("cls"); //limpiar pantalla

				break;
			}

		}


		break;

	case 0:
		printf("Hasta luego. Pulse Enter para salir\n");
		enterKey();
		system("cls");
		break;


	case 2:
		//	displayCredits();
			enterKey();
		break;

	default:
		printf("Tecla incorrecta. Pulse Enter para continuar...\n ");
	}



	}while(menu != 0);


}




void displayStructure(int num,  enum AXIS axis)
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




void PlayerTurn(Player* player, Point3D *p) //Función que permite a determinado jugador realizar su turno
{




}


int useTicket( Player* player,  enum TURN turn)
{

	if(player->ticket == 0) //Si ya no tiene tiradas
		return -1; //Regresar un error


	//Solución ingenua
	//Realizar una copia de la torre
	Tower* copy = copyTower(tower);




	return 0;
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

	 while (1) {
	        c = _getch();
	        if (c == 0 || c == 224) {   // teclas especiales
	            c = _getch();
	            switch (c) {
	                case 72: return 'u';
	                case 80: return 'd';
	                case 75: return 'l';
	                case 77: return 'r';
	                case 13: return 'e';
	            }
	        } else if (c == 'q') {
	            return 'q';
	        }
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



//funciones de limpieza

//Función que limpia la memoria de la torre
void freeTower(Tower* t)
{

	for(int i=0; i < FLOORS; ++i)
	{
		for(int j=0; j < ROWS; ++j)
		{
			free(t->board3D[i][j]); //libera la matriz
		}
		free(t->board3D[i]); //Libera la capa
	}
	free(t->board3D);
	free(t);
}


//Función que libera la memoria del plano
void freePlane(Plane* p)
{

	for(int i=0; i < ROWS; ++i)
	{
		free(p->board2D[i]);
	}
	free(p->board2D);
	free(p);


}



