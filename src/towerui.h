#ifndef TOWERUI_H_INCLUDED
#define TOWERUI_H_INCLUDED

#ifdef BUILD_TOWERUI_DLL
    // Cuando compilamos la DLL, exportamos las funciones
    #define DLLEXPORT __declspec(dllexport)
#else
    // Cuando otros programas usan la DLL, las importamos
    #define DLLEXPORT __declspec(dllimport)
#endif

//Despliegue de la torre
DLLEXPORT void displayTower(Tower* t, int n,int *axis,int *plano,char *c, int DIMENSION);
DLLEXPORT void updateMenu(char *c,int *axis,int *plano, int DIMENSION); //Función para actualizar el plano y eje de la vista de la torre
DLLEXPORT void displayStructure(Tower* t, int n,int axis, int DIMENSION); //Función para imprimir el plano y eje
DLLEXPORT void displayZ(Tower* t, int n, int DIMENSION); //Función para imprimir el eje Z
DLLEXPORT void displayY(Tower* t, int n, int DIMENSION); //Función para imprimir el eje Y
DLLEXPORT void displayX(Tower* t, int n, int DIMENSION); //Función para imprimir el eje X
DLLEXPORT void displayD1(Tower* t, int DIMENSION); //Función para imprimir diagonal 3D
DLLEXPORT void displayD2(Tower* t, int DIMENSION); //Función para imprimir antidiagonal 3D

//Funciones auxiliares
DLLEXPORT void printChar(char c); //Impresión de carácteres con colores
DLLEXPORT char readKeyboard(void); //Lectura de teclas para el menu de la torre
DLLEXPORT void enterKey(void);

#endif // TOWERUI_H_INCLUDED
