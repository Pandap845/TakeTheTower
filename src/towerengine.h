#ifndef TOWERENGINE_H_INCLUDED
#define TOWERENGINE_H_INCLUDED

#ifdef BUILD_TOWERENGINE_DLL
    // Cuando compilamos la DLL, exportamos las funciones
    #define DLLEXPORT __declspec(dllexport)
#else
    // Cuando otros programas usan la DLL, las importamos
    #define DLLEXPORT __declspec(dllimport)
#endif

typedef struct
{
	char id;
	int ticket;
} Player; //Representa la información de un jugador

typedef struct
{
	char*** board3D;
	int spacesLeft;
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

//funciones incializadoras
DLLEXPORT Tower* initTower(int DIMENSION);
DLLEXPORT Player* initPlayer(char id);
DLLEXPORT Plane* initPlane(int DIMENSION);
DLLEXPORT Point3D* initPoint3D(void);
DLLEXPORT Point2D* initPoint2D(void);

//Funciones de mantenimiento
DLLEXPORT Tower* copyTower(Tower* tower, int DIMENSION); //Función que genera una copia temporal de la torre antes de realizar el cambio
DLLEXPORT Plane* obtainPlane(Tower* t, int n, int axis, int DIMENSION); //Función que obtiene el plano a analizar
DLLEXPORT Point2D* obtainPoint2D(Point3D* p, int axis); //Función que transforma un punto 3D a 2D

//Funciones de giro
DLLEXPORT Plane* turn90Left(Plane* p, int DIMENSION);
DLLEXPORT Plane* turn90Right(Plane* p, int DIMENSION);

//funciones de validación
DLLEXPORT void verifyWin(Tower* t,Point3D *p, int *resultado, int DIMENSION, int PLAYERS); //Función que checa si ganó el jugador en su turno.
DLLEXPORT void verifyPlane(Plane* plane, int *resultado, int line, int n, int DIMENSION, int PLAYERS);
DLLEXPORT void verifyDiagonals(Tower* t, int axis,int *resultado, int DIMENSION, int PLAYERS); //Función que verifica una diagonal de tercera dimensión del tablero
DLLEXPORT void checkAllTower(Tower* t, int index, int* resultado, int DIMENSION, int PLAYERS); //Función que checa todos los planos para validar antes de realizar el giro

//Funciones para limpieza de memoria
DLLEXPORT void freeTower(Tower* t, int DIMENSION);
DLLEXPORT void freePlane(Plane* p, int DIMENSION);
DLLEXPORT void reset(Tower* t, Player** players, int DIMENSION, int PLAYERS);

#endif // TOWERENGINE_H_INCLUDED
