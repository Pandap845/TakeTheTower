#ifndef TOWERUI_H_INCLUDED
#define TOWERUI_H_INCLUDED

#include <conio.h>
#include <pthread.h>
#include "towerengine.h"

#ifdef BUILD_TOWERUI_DLL
    // Cuando compilamos la DLL, exportamos las funciones
    #define DLLEXPORT __declspec(dllexport)
#else
    // Cuando otros programas usan la DLL, las importamos
    #define DLLEXPORT __declspec(dllimport)
#endif

//Despliegue de la torre
DLLEXPORT void displayTower(int n,int *axis,int *plano,char *c);
DLLEXPORT void updateMenu(char *c,int *axis,int *plano); //Función para actualizar el plano y eje de la vista de la torre
DLLEXPORT void displayStructure(int n,int axis); //Función para imprimir el plano y eje
DLLEXPORT void displayZ(int n); //Función para imprimir el eje Z
DLLEXPORT void displayY(int n); //Función para imprimir el eje Y
DLLEXPORT void displayX(int n); //Función para imprimir el eje X
DLLEXPORT void displayD1(void); //Función para imprimir diagonal 3D
DLLEXPORT void displayD2(void); //Función para imprimir antidiagonal 3D

//Funciones auxiliares
DLLEXPORT void printChar(char c); //Impresión de carácteres con colores
DLLEXPORT char readKeyboard(void); //Lectura de teclas para el menu de la torre

#endif // TOWERUI_H_INCLUDED
