#include "utilidades.hpp"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

int obtener_tecla() {
    int caracter;
    struct termios configuracion_anterior;
    struct termios configuracion_nueva;

    tcgetattr(STDIN_FILENO, &configuracion_anterior);
    configuracion_nueva = configuracion_anterior;
    configuracion_nueva.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &configuracion_nueva);
    caracter = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &configuracion_anterior);

    return caracter;
}

void limpiar_pantalla() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

void mostrar_menu() {
    limpiar_pantalla();
    printf("=========================================\n");
    printf("  DIBUJADOR DE FIGURAS GEOMETRICAS\n");
    printf("=========================================\n\n");
    printf("Seleccione la figura a dibujar:\n\n");
    printf("  1. Cuadrados (4 lados)\n");
    printf("  2. Pentagonos (5 lados)\n");
    printf("  3. Heptagonos (7 lados)\n");
    printf("  4. Decagonos (10 lados)\n");
    printf("  5. Salir\n\n");
    printf("Opcion: ");
    fflush(stdout);
}
