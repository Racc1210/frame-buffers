#include "framebuffer.hpp"
#include "figuras.hpp"
#include "utilidades.hpp"
#include <cstdlib>
#include <ctime>
#include <stdio.h>

int main() {
    srand(time(NULL));

    int descriptor_framebuffer = inicializar_framebuffer();

    printf("Presiona cualquier tecla para comenzar...\n");
    obtener_tecla();

    int opcion = 0;
    while (opcion != 5) {
        mostrar_menu();
        opcion = obtener_tecla() - '0'; 

        if (opcion >= 1 && opcion <= 4) {
            limpiar_pantalla();
            dibujar_figuras(opcion);
        } else if (opcion == 5) {
            limpiar_pantalla();
            printf("Programa terminado\n");
        } else {
            limpiar_pantalla();
            printf("Opcion invalida. Presiona cualquier tecla...\n");
            obtener_tecla();
        }
    }
    cerrar_framebuffer(descriptor_framebuffer);
    return 0;
}
