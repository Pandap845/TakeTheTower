cmake_minimum_required(VERSION 3.15)

project(TakeTheTower VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

#ruta del src
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#Crear libreria towerengine
add_library(towerengine SHARED
    ${SRC_DIR}/towerengine.c
)

#Cargar argumento
target_compile_definitions(towerengine
    PRIVATE BUILD_TOWERENGINE_DLL
)


#towerui
add_library(towerui SHARED
    ${SRC_DIR}/towerui.c
)

#Argumento
target_compile_definitions(towerui
    PRIVATE BUILD_TOWERUI_DLL
)

target_link_libraries(towerui
    PRIVATE towerengine
)

# ====================
# Main
# ====================
add_executable(DED_Proyecto2
    ${SRC_DIR}/DED_Proyecto2.c
)

target_link_libraries(DED_Proyecto2
    PRIVATE towerui towerengine
)
