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
#include <string.h>
#include <conio.h>
#include <pthread.h>

//Variables globales
int DIMENSION,PLAYERS;

Tower* tower;
Player** players;

//Prototipos de las funciones

//Funciones de pantalla
void gameStart(int *menu); //Función para el menu principal
void menuTower(int *menu,int *resultado,int *turn,int *axis,int *plano); //Función para mostrar el menu del juego
int quitDialog(void); //Menu para salir
void endScreen(int *resultado,int *turn,int *menu); //Menu de final del juego
void playerTurn(Player* player,int *resultado,int *turn,int *axis,int *plano); //Función que permite a determinado jugador realizar su turno
void turnDialog(Player* player,int *input); //Función que da el diálogo principal para las acciones del jugador
void placeMarble(Player *player,int *resultado, int *validTurn); //Da ingreso de colocación de canica
void ticket(Player* player,int *resultado,int *validTurn); //Función que permite al usuario cambiar de plano

//Función auxiliar
void enterKey(void);

//-------------------------------------------

int main(void) {
	//inicializar torre y jugadores
	int var,menu;

	do {
		system("cls");
		printf("=========================================\n");
		printf("           TAKE THE TOWER IN C           \n");
		printf("=========================================\n");

		printf("Menu:\n "
				"\nJugar        (1)"
				"\nVer creditos (2)"
				"\nSalir        (0)\n");
		var = scanf("%d", &menu);
		if (var != 1) setbuf(stdin,NULL);

		switch(menu)
		{
		case 1: //Entrar al juego
            while (menu == 1)
                gameStart(&menu);
			break;
		case 2: //Mostrar créditos
		    system("cls");
            printf("\n(c) 2025.\n"
                   "Victor Emiliano Rodriguez Aguila\n"
                   "& Joshua David DeBono Rios\n\n"
                   "Presiona una tecla para regresar.");
            enterKey();
			break;
        case 0:
            printf("\nHasta luego!\n");
		}
	} while (var != 1 || menu != 0);
}

//Empiezo de GUI
void gameStart(int *menu)
{
    int var;
    int resultado = 0; //0 => Juego en transcurrencia \\ 1 - 4 => ganó el jugador correspondiente \\ 5 => Se salió de la partida/empate
    int turn = 0; // turn corresponde al número del jugador respectivo
    int axis = 0, plano = 0; //Colocar posición y plano de vista

    //Solicitando datos preliminarios del juego
    system("cls");
    printf("------EMPEZANDO JUEGO TAKE THE TOWER----\n\n");
    do {
        printf("De cuantas dimensiones es su tabla? (3-5) ");
        var = scanf("%d",&DIMENSION);
        if (var != 1) setbuf(stdin,NULL);
    } while (var != 1 || DIMENSION < 3 || DIMENSION > 5);

    do {
        printf("Cuantos jugadores quieren jugar? (2-4) ");
        var = scanf("%d",&PLAYERS);
        if (var != 1) setbuf(stdin,NULL);
    } while (var != 1 || PLAYERS < 2 || PLAYERS > 4);

    //Inicializando estructuras globales
    players = (Player**)malloc(sizeof(Player*)*PLAYERS);
    for (int i = 0; i < PLAYERS; i++)
        players[i] = initPlayer(i+'1');

    tower = initTower();

    //Empezando juego (ciclo de juego controlado por el valor de resultado)
    while (resultado == 0)
        menuTower(menu,&resultado,&turn,&axis,&plano);
}

void menuTower(int *menu,int *resultado,int *turn,int *axis, int *plano)
{
	char c;

	do { //Bucle para desplegar la torre mientras no se presiona 'q' o 'e'
        displayTower(*turn+1,axis,plano,&c); //Pasar turn + 1 para desplegar el número del jugador que le toca
	} while (c != 'q' && c != 'e');

	if (c == 'q' && quitDialog()) //Si se presiona 'q' y quitDialog() == true, se sale del juego y se reinicia
	{
        *resultado = 5;
		*menu = 3;
	}
	else if (c == 'e') //Si se presiona 'e', se pasa a la pantalla de turnos.
	{
		//Jugar
        playerTurn(players[*turn],resultado,turn,axis,plano);

		if (tower->spacesLeft == 0)
            *resultado = 5;

		if (*resultado)
			endScreen(resultado,turn,menu);
	}
}

//Función que muestra pantalla de confirmación
int quitDialog(void)
{
	int input,var;

	do {
		system("cls");
		printf("Realmente quiere salir?\n"
				"\nSi (1)"
				"\nNo (0)\n");
		var = scanf("%d",&input);
		if (var != 1) setbuf(stdin,NULL);
	} while (var != 1 || (input != 0 && input != 1));

    if (input) reset();

    return input;
}

//Pantalla final del juego. Permite visualizar el tablero al finalizar
void endScreen(int *resultado,int *turn,int *menu)
{
	int input,var;

	do {
		system("cls");
		if (*resultado == 5)
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
        int axis = 0, plano = 0;
	    //Uso de la función de despliegue de la torre para mostrar la tabla final sin funcionalidad real del juego
	    while (1) //Ciclo para regresar en caso de que no quiera salir en el dialogo de salida
        {
            char c;

            do {
                displayTower(0,&axis,&plano,&c); //Función de despliegue
            } while (c != 'q');

            if (c == 'q' && quitDialog()) break; //Si se quiere salir se rompe el ciclo y se va al menú principal
        }
        *menu = 3;
		break;
	case 2:
	    reset();
		*turn = 0;
		break;
	case 3:
	    reset();
	    *menu = 3;
	}
}

//Inicializar formalmente el turno del jugador
void playerTurn(Player *player,int *resultado,int *turn,int *axis,int *plano) //Función que permite a determinado jugador realizar su turno
{
	int input,validTurn;

    do {
        validTurn = 0;
        turnDialog(player,&input);

        switch (input) {
        case 1:
            placeMarble(player,resultado,&validTurn);
            break;
        case 2:
            ticket(player,resultado,&validTurn);
            break;
        case 0:
            validTurn = 1;
        }
    } while (!validTurn); //Si no se ejecutó un turno correctamente se da otra vuelta al diálogo principal

    if (input) {
        *turn = (*turn + 1) % PLAYERS;
        *axis = 0;
        *plano = 0;
    }
}

//Pantalla de diálogo para el turno
void turnDialog(Player* player,int *input)
{
    int var;

    do {
		system("cls");
		printf("JUGADOR %c - TICKETS: %d            \n\n"
				"Selecciona una accion:               \n"
				"Colocar una canica                (1)\n"
				"Girar la tabla (Costo: 1 ticket)  (2)\n"
				"Regresar a ver el tablero         (0)\n",player->id,player->ticket);
		var = scanf("%d",input);

		if (var != 1) setbuf(stdin,NULL);
		else if (*input == 2 && player->ticket == 0)
		{
			printf("\nNo tienes suficientes tickets para girar la tabla. Presione una tecla y selecciona otra opción.\n");
			*input = -1;
			enterKey();
		}
	} while (var != 1 || *input < 0 || *input > 2);
}

//Solicitar posición en donde el jugador quiera poner su canica
void placeMarble(Player *player,int* resultado,int* validTurn)
{
    Point3D *p = initPoint3D();
    system("cls");
    printf("-------------------------------- COLOCANDO UNA CANICA --------------------------------\n\n");

    for (int i = 0; i < 3; i++) //Input Dialog (se hace 3 veces, para cada nivel de coordenada)
    {
        int input,var;
        do {
            switch (i) {
            case X: printf("\nDeclare la capa en el cual quieres posicionar tu canica (1-%d, 0 para regresar): ", DIMENSION); break;
            case Y: printf("\nDeclare la fila en el cual quieres posicionar tu canica (1-%d, 0 para regresar): ", DIMENSION); break;
            case Z: printf("\nDeclare la columna en el cual quieres posicionar tu canica (1-%d, 0 para regresar): ", DIMENSION);
            }
            var = scanf("%d",&input);
            if (var != 1) setbuf(stdin,NULL);
        } while (var != 1 || input < 0 || input > DIMENSION);

        if (input == 0) return;

        switch (i) {
        case X: p->x = input-1; break;
        case Y: p->y = input-1; break;
        case Z: p->z = input-1;
        }
    }

	if (tower->board3D[p->x][p->y][p->z] == '0')
    {
		tower->board3D[p->x][p->y][p->z] = player->id;

		//Verificar tabla en ese punto
        verifyDiagonals(D1,resultado);
		if (!(*resultado)) verifyDiagonals(D2,resultado);
		if (!(*resultado)) verifyWin(p,resultado);

        *validTurn = 1;
        tower->spacesLeft -= 1;
		printf("\nCanica colocada con exito! Presiona una tecla para continuar.\n");
		enterKey();
	} else {
	    printf("\nYa hay una canica en ese espacio, presiona una tecla e intenta de nuevo.\n");
		enterKey();
	}

	free(p);
}

//Solicitar información del giro y validarlo
void ticket(Player* player,int *resultado,int *validTurn)
{
    int index, direction, var;

    system("cls");
    printf("-------------------------------- GIRANDO LA TABLA --------------------------------\n\n");
    //Pedir eje de rotación

    // Pedir al jugador el plano a girar
    do {
        //printf("\nDeclara el plano que quieres girar (%c fijo, 1-%d, 0 para regresar): ",axis+'W',DIMENSION);
        printf("\nDeclara el plano que quieres girar (X fijo, 1-%d, 0 para regresar): ",DIMENSION);
        var = scanf("%d", &index);
        if (var != 1) setbuf(stdin, NULL);
    } while (var != 1 || index < 0 || index > DIMENSION);

    if (index == 0) return;

    // Pedir dirección de giro
    do {
        printf("\nElige la direccion a la cual la quieres girar:\n"
               "Izquierda (1)\n"
               "Derecha   (2)\n"
               "Regresar  (0)\n");
        var = scanf("%d", &direction);
        if (var != 1) setbuf(stdin, NULL);
    } while (var != 1 || direction < 0 || direction > 2);

    if (direction == 0) return;

    // Crear copia de la torre como respaldo
    Tower* backup = copyTower(tower);

    // Obtener el plano fijo en X
    //Plane* plane = obtainPlane(index-1, axis-1);
    Plane* plane = obtainPlane(index - 1, X);

    // Rotar el plano
    Plane* rotated = (direction == 1) ? turn90Left(plane) : turn90Right(plane);

    // Aplicar el plano rotado en la torre real
    for (int i = 0; i < DIMENSION; i++)
        for (int j = 0; j < DIMENSION; j++)
            tower->board3D[index-1][i][j] = rotated->board2D[i][j];

    // Verificar si alguien ganó tras el giro
    checkAllTower(tower, plane, index-1, resultado);

    if (!(*resultado)) { //Verifica si el giro fue valido
        *validTurn = 1;
        player->ticket -= 1;
        printf("\nEl plano fue girado con exito! Presiona una tecla para continuar.\n");
        enterKey();
    } else {
        // Restaurar desde el respaldo
        for (int x = 0; x < DIMENSION; x++)
            for (int y = 0; y < DIMENSION; y++)
                for (int z = 0; z < DIMENSION; z++)
                    tower->board3D[x][y][z] = backup->board3D[x][y][z];
        *resultado = 0;
        printf("\nMovimiento invalido: alguien ganaria con este giro. Presiona una tecla e intenta de nuevo.\n");
        enterKey();
    }

    // Liberar memoria
    freePlane(plane);
    freePlane(rotated);
    freeTower(backup);
}

//-------------------------
//Fúnciones auxiliares

//Limpia el buffer y hace que se presione una tecla para continuar
void enterKey(void)
{
    setbuf(stdin,NULL);
	getchar();
}
