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
	int x;
	int y;
	int z;
} Point3D;


typedef struct
{
	int x;
	int y;
} Point2D;





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








//Variables globalbes
Tower* tower;
Player* player1;
Player* player2;

//Prototipos de las funciones

//Funciones de juego
void playerTurn(Player* player, Point3D *p,int *resultado); //Función que permite a determinado jugador realizar su turno
int ticket( Player* player); //Función que permite al usuario cambiar de plano
void placeMarble(Point3D *p, int dim); //Da ingreso de colocación de canica
void verifyWin(Point3D *p, int *resultado); //Función que checa si ganó el jugador en su turno.
void verifyPlane(Plane* plane, int *resultado, int line, int n);
int checkAllTower(Tower* t); //Función que checa todos los planos para validar antes de realizar el giro
Tower* copyTower(Tower* tower); //Función que genera una copia temporal de la torre antes de realizar el cambio
Plane* obtainPlane(int n, int axis);
Point2D* obtainPoint2D(Point3D* p, int axis);


//funciones incializadoras
Tower* initTower(void);
Player* initPlayer(char id);
Plane* initPlane(void);
Point3D* initPoint3D(void);
Point2D* initPoint2D(void);


//Funciones GUI
void menu(void); //Función para el menu principal
void menuTower(int *resultado,int *turn, int *menu); //Función para mostrar el estado de la torre
void updateMenu(char *c,int *axis,int *plano);
void quitDialog(int *resultado);
void winScreen(int *resultado,int *turn,int *menu);
void displayStructure(int n,int axis);
void displayZ(int n);
void displayY(int n);
void displayX(int n);
void displayD1(void);
void displayD2(void);
//void displayD(int n); //Solo dos planos lol
void displayCredits(void);
void printChar(char c);
char readKeyboard(void);
void clearBuffer(void);
void enterKey(void);
void reset(void);

void freeTower(Tower* t);
void freePlane(Plane* p);


//Funciones de giro

Plane* turn90Left(Plane* p);
Plane* turn90Right(Plane* p);


//-------------------------------------------

int main(void) {

	//inicializar torre y jugadores
	tower = initTower();
	player1 = initPlayer('1');
	player2 = initPlayer('2');

	menu();

	freeTower(tower);
	free(player1);
	free(player2); //Liberar memoria
}



//Funciones inicializadoras

Plane* initPlane(void)
{
	Plane* p;
	p = (Plane*)malloc(sizeof(Plane));


	//Médidas de seguridad
	if(!p) return NULL;

	p->board2D = (char**)malloc(sizeof(char**)*ROWS);

	if(!(p->board2D))
	{
		free(p);
		return NULL;
	}


	for(int i=0; i< ROWS; ++i)
	{
		p->board2D[i] = (char*)malloc(sizeof(char*)*COLUMNS);

		 if (!(p->board2D[i])) {
		            for (int j = 0; j < i; ++j) {
		                free(p->board2D[j]);
		            }
		            free(p->board2D);
		            free(p);
		            return NULL;
		        }
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
	p = (Player*)malloc(sizeof(Player));


	if(!p) return NULL; //Por si no se inicializa
	p->id = id;
	p->ticket = 4;
	p->marble = 32;

	return p;
}

Tower* initTower(void)
{
	Tower *t;
	t = (Tower*)malloc(sizeof(Tower));

	t->board3D  = (char***)malloc(sizeof(char**)*FLOORS);

	for(int i=0; i < FLOORS; ++i)
	{
		t->board3D[i] = (char**)malloc(sizeof(char*)*ROWS);
		for(int j=0; j < ROWS; ++j)
		{
			t->board3D[i][j] = (char*)malloc(sizeof(char)*COLUMNS);
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


//Funciones de liberación de memoria
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


//--------------------------------
//Cuerpo de las funciones
//--------------------------------
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
				x = n; y = j; z = i; break;
			case Y:
				x = j; y = n; z = i; break;
			case Z:
				x = i; y = j; z = n; break;
			case D1:
				x = i; y = i; z = j;
									 break;
			case D2:
				x = i; z = j; y = (COLUMNS - 1) - j;
								break;
			}
			plane->board2D[i][j] = tower->board3D[x][y][z];
		}
	}
	/*
	switch(axis)
	{

	case X:
		for(int y=0; y <COLUMNS; y++)
			for(int z=0; z < FLOORS; z++)
				plane->board2D[y][z] = tower->board3D[n][z][y];
		break;

	case Y:
		for(int x=0; x <ROWS; x++)
			for(int z=0; z < FLOORS; z++)
				plane->board2D[x][z] = tower->board3D[z][n][x];
		break;

	case Z:
		for(int x=0; x < ROWS; x++)
			for(int y=0; y < COLUMNS; y++)
				plane->board2D[x][y] = tower->board3D[x][y][n];
		break;
	}
	*/
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

	case D1:
	case D2:
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

	do {
		int resultado = 0; //0 => Juego en transcurrencia \\ 1 => Gana jugador 1 \\ 2 => Gana jugador 2 \\ 3 => Se salió de la partida
		int turn = 0; // 0 => jugador 1 \\ 1 => jugador 2
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
		case 1:
			while (resultado == 0)
				menuTower(&resultado,&turn,&menu);
			break;

		case 2:
			displayCredits();
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

void menuTower(int *resultado,int *turn, int *menu)
{
	int axis = 0;
	int plano = 0;
	char c;

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
			else if(axis == Z)
				printf("Z");
			else if (axis == D1)
				printf("Primera diagonal");
			else
				printf("Segunda diagonal");


			if(axis != D1 && axis != D2)
			printf("\n Plano: %d\n", plano+1 );
			else
			printf("\n");

				displayStructure(plano, axis);

			//	clearBuffer(); //método para limpiar el buffer
			printf("\nPulse las teclas para realizar:"
					"\nArriba:    Cambiar eje arriba"
					"\nAbajo:     Cambiar eje abajo"
					"\nIzquierda: Mover plano izquierdo"
					"\nDerecha:   Mover plano derecho"
					"\ne:         Pasar turno (Jugador %d)"
					"\nq:         Salir\n",playerid);
			c = readKeyboard();

			updateMenu(&c,&axis,&plano);

		} while (c != 'q' && c != 'e');

		break;
	}

	if (c == 'q')
		//Dar diálogo de salida para asegurar que se termine el juego a propósito
		quitDialog(resultado);


	else if (c == 'e')
	{
		//Jugar
		Point3D *p = initPoint3D();
		if (playerid == 1)
			playerTurn(player1,p,resultado);
		else
			playerTurn(player2,p,resultado);

		if (*resultado != 0)
			winScreen(resultado,turn,menu);
		*turn = !(*turn); //cambia jugador
	}
}

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

	if (input == 1)
		{*resultado = 3;

			freeTower(tower);
			tower = initTower();
		}
}

void winScreen(int *resultado,int *turn,int *menu)
{
	int input,var;

	do {
		system("cls");
		printf("\nFELICIDADES - JUGADOR %d HA GANADO!\n"
				"\nJugar de nuevo     (1)"
				"\nIr al menu         (2)"
				"\nSalir del programa (3)\n",*resultado);
		var = scanf("%d",&input);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || input < 1 || input > 3);

	switch (input) {
	case 1:
		reset(); //Libera la memoria de torre y jugadores para inicializarlos de nuevo
		*resultado = 0;
		*turn = 1; //Al terminar este bloque se pasará el turno, entonces al inicializar el juego empezará con jugador 1 (turno == 0)
		break;
	case 2: reset(); break;
	case 3: *menu = 0;
	}
}

void displayStructure(int n,  int axis)
{
	//Seleccionar que pantalla mostrar
	switch(axis)
	{
	case X:
		displayX(n);
		break;

	case Y:
		displayY(n);
		break;
	case Z:
		displayZ(n);
		break;
	case D1:
		displayD1();
		break;
	case D2:
		displayD2();
		break;
	}

}
//Función que imprime un tablero horizontal

void displayZ(int n)
{
    // 1. Obtener el plano

    Plane* plane = obtainPlane(n, Z);


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

    freePlane(plane);
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


    freePlane(plane);

}


//Función para imprimir torre con Eje X fijo

void displayX(int n)
{
	 // 1. Obtener el plano

	    Plane* plane = obtainPlane(n, X);
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

	    freePlane(plane);

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
	printf("\n(c) 2025. Victor Emiliano Rodriguez Aguila\n");
	printf("      &\n         Joshua David DeBono Rios\n");
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
            printf("%c", c); // basura lol
            break;
    }
}

void playerTurn(Player *player, Point3D *p,int *resultado) //Función que permite a determinado jugador realizar su turno
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
			placeMarble(p,X);
			placeMarble(p,Y);
			placeMarble(p,Z);
			if (tower->board3D[p->x][p->y][p->z] != '0')
			{
				printf("\nYa hay una canica en ese espacio, intenta de nuevo.\n");
				validTurn = 0;
			} else {
				tower->board3D[p->x][p->y][p->z] = player->id;
				verifyWin(p,resultado);
			}

		}
		else
		{
			if(!ticket(player)) continue;
			player->ticket -= 1;


		}

	} while (validTurn == 0);

	player->marble -= 1;
}

void placeMarble(Point3D *p,int dim)
{
	int x,var;
	do {
		switch (dim) { //Sino, es X, Y , Z
		case X: printf("Declare la capa en el cual quieres posicionar tu canica (1-4):"); break;
		case Y: printf("Declare la fila en el cual quieres posicionar tu canica (1-4):"); break;
		case Z: printf("Declare la columna en el cual quieres posicionar tu canica (1-4):");
		}
		var = scanf("%d",&x);
		if (var != 1)
			setbuf(stdin,NULL);
	} while (var != 1 || x < 0 || x > 4);

	switch (dim) {
	case X: p->z = x-1; break;
	case Y: p->x = x-1; break;
	case Z: p->y = x-1;
	}
}


int ticket(Player* player)
{
    int index, direction, var;

    // Pedir al jugador el plano a girar
    do {
        printf("Jugador %c, elige el plano (Z fijo, 1-4): ", player->id);
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

    // Obtener el plano fijo en Z
    Plane* plane = obtainPlane(index - 1, Z);

    // Rotar el plano
    Plane* rotated;
    if (direction == 1)
        rotated = turn90Left(plane);
    else
        rotated = turn90Right(plane);

    // Aplicar el plano rotado en la torre real
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLUMNS; j++)
            tower->board3D[i][j][index - 1] = rotated->board2D[i][j];

    // Verificar si alguien ganó tras el giro
    int valid = checkAllTower(tower);

    if (valid) {
        printf("\nEl plano fue girado con exito!\n");
    } else {
        printf("\nMovimiento invalido: alguien ganaria con este giro. Revirtiendo...\n");

        // Restaurar desde el respaldo
        for (int x = 0; x < FLOORS; x++)
            for (int y = 0; y < ROWS; y++)
                for (int z = 0; z < COLUMNS; z++)
                    tower->board3D[x][y][z] = backup->board3D[x][y][z];
    }

    enterKey();
    enterKey();

    // Liberar memoria
    freePlane(plane);
    freePlane(rotated);
    freeTower(backup);

    return valid;
}


void verifyWin(Point3D *p, int *resultado)
{
	//Función que checa si ganó el jugador en su turno.
	//Nos basamos en la lógica de que, siemmpre un punto hace intersección en 3 planos
	Plane* plane;
	Point2D* p2d;


	for(int i=0; i < 5; i++)
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

		case D1:
			  if (p->x != p->y) continue; // no pertenece al plano
			plane = obtainPlane(0, D1);
			p2d = obtainPoint2D(p, D1);
			break;
		case D2:
			  if (p->x + p->z != 3) continue; // no pertenece a la diagonal
			plane = obtainPlane(0, D2);
			p2d = obtainPoint2D(p, D2);
			break;


		}

		//proceso de validación
		verifyPlane(plane,resultado,0,p2d->x); //Checar horizontalmente
		if (*resultado != 0) return;
		verifyPlane(plane,resultado,1,p2d->y); //Checar verticalmente
		if (*resultado != 0) return;
		verifyPlane(plane,resultado,2,0); //Checar primera diagonal
		if (*resultado != 0) return;
		verifyPlane(plane,resultado,3,0); //Checar segunda diagonal
		if (*resultado != 0) return;

		//Estar liberando la memoria del plano
		freePlane(plane);
		free(p2d);
	}
}

//Función que verifica si ALGUIEN ganó
void verifyPlane(Plane* plane, int *resultado,int line, int n)
{
	int p1=0, p2=0;
	int x=0,y=0;
	for(int i=0; i < 4; i++)
	{
		switch (line) {
		case 0: x = n; y = i; break;
		case 1: x = i; y = n; break;
		case 2: x = i; y = i; break;
		case 3: x = i; y = 3-i;
		}
		if (plane->board2D[x][y] == '1') p1++;
		if (plane->board2D[x][y] == '2') p2++;
	}

	*resultado = (p1 == 4) ? 1 : (p2 == 4) ? 2 : 0;
}


//Esta función no debería siquiera existir. Pero permite verificar TODA la torre para ver si hay algún movimiento invalido al girar la torre
int checkAllTower(Tower* t)
{
    Plane* plane;
    int resultado = 0;

    // Revisar todos los planos en todos los ejes
    for (int n = 0; n < 4; n++) {
        // Eje X
        plane = obtainPlane(n, X);
        for (int i = 0; i < 4 && resultado == 0; i++) {
            verifyPlane(plane, &resultado, 0, i);
            if(resultado!=0) return 0;

            verifyPlane(plane, &resultado, 1, i);
            if(resultado!=0) return 0;
        }
        verifyPlane(plane, &resultado, 2, 0);
        if(resultado!=0) return 0;
        verifyPlane(plane, &resultado, 3, 0);
        if(resultado!=0) return 0;
        freePlane(plane);

        // Eje Y
        plane = obtainPlane(n, Y);
        for (int i = 0; i < 4 && resultado == 0; i++) {
            verifyPlane(plane, &resultado, 0, i);
            if(resultado!=0) return 0;
            verifyPlane(plane, &resultado, 1, i);
            if(resultado!=0) return 0;
        }
        verifyPlane(plane, &resultado, 2, 0);
        if(resultado!=0) return 0;
        verifyPlane(plane, &resultado, 3, 0);
        if(resultado!=0) return 0;
        freePlane(plane);

        // Eje Z
        plane = obtainPlane(n, Z);
        for (int i = 0; i < 4 && resultado == 0; i++) {
            verifyPlane(plane, &resultado, 0, i);
            if(resultado!=0) return 0;
            verifyPlane(plane, &resultado, 1, i);
            if(resultado!=0) return 0;
        }
        verifyPlane(plane, &resultado, 2, 0);
        if(resultado!=0) return 0;
        verifyPlane(plane, &resultado, 3, 0);
        if(resultado!=0) return 0;
        freePlane(plane);
    }

    // Revisar diagonales principales
    plane = obtainPlane(0, D1);
    verifyPlane(plane, &resultado, 2, 0);
    if(resultado!=0) return 0;
    verifyPlane(plane, &resultado, 3, 0);
    if(resultado!=0) return 0;
    freePlane(plane);

    plane = obtainPlane(0, D2);
    verifyPlane(plane, &resultado, 2, 0);
    if(resultado!=0) return 0;
    verifyPlane(plane, &resultado, 3, 0);
    freePlane(plane);

    return (resultado == 0); // 1 si es válido, 0 si alguien ganó
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

void reset(void)
{
	freeTower(tower); free(player1); free(player2);
	tower = initTower();
	//Verificar por ubo una inicialización invalida

	player1 = initPlayer('1');
	player2 = initPlayer('2');
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
