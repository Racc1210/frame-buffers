#include "figuras.hpp"
#include "framebuffer.hpp"
#include "utilidades.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void dibujar_cuadrado(int posicion_x, int posicion_y, int color) {
    int ancho = TAMANO_FIGURA;
    int alto = TAMANO_FIGURA;

    for (int fila = 0; fila < alto; fila++) {
        for (int columna = 0; columna < ancho; columna++) {
            int pixel_x = posicion_x + columna;
            int pixel_y = posicion_y + fila;

            if ((unsigned int)pixel_x >= informacion_variable->xres || (unsigned int)pixel_y >= informacion_variable->yres)
                continue;

            long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
                                 (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);

            if (ubicacion < tamano_pantalla)
                *((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
        }
    }
}

void dibujar_figuras_aux(int posicion_x, int posicion_y, int color, int numero_lados) {
    int centro_x = posicion_x + TAMANO_FIGURA / 2;
    int centro_y = posicion_y + TAMANO_FIGURA / 2;
    int radio = TAMANO_FIGURA / 2;

    int vertices_x[10];
    int vertices_y[10];

    for (int indice = 0; indice < numero_lados; indice++) {
        double angulo = (2.0 * PI * indice / numero_lados) - (PI / 2.0);
        vertices_x[indice] = centro_x + (int)(radio * cos(angulo));
        vertices_y[indice] = centro_y + (int)(radio * sin(angulo));
    }

    for (int fila = 0; fila < TAMANO_FIGURA; fila++) {
        for (int columna = 0; columna < TAMANO_FIGURA; columna++) {
            int pixel_x = posicion_x + columna;
            int pixel_y = posicion_y + fila;

            int dentro = 1;
            for (int indice = 0; indice < numero_lados; indice++) {
                int siguiente = (indice + 1) % numero_lados;
                int vector_x = vertices_x[siguiente] - vertices_x[indice];
                int vector_y = vertices_y[siguiente] - vertices_y[indice];
                int punto_x = pixel_x - vertices_x[indice];
                int punto_y = pixel_y - vertices_y[indice];

                if (vector_x * punto_y - vector_y * punto_x < 0) {
                    dentro = 0;
                    break;
                }
            }

            if (dentro) {
                if ((unsigned int)pixel_x >= informacion_variable->xres || (unsigned int)pixel_y >= informacion_variable->yres)
                    continue;

                long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
                                     (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);

                if (ubicacion < tamano_pantalla)
                    *((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
            }
        }
    }
}



void dibujar_figuras(int tipo_figura) {
    memset(mapa_framebuffer, 0, tamano_pantalla);

    int cantidad_figuras = (rand() % 4) + 2;

    unsigned short colores[] = {
        0x07fc,
        0x005f,
        0xffe0,
        0x37e0,
        0xf800,
        0xfd60,
        0x781f,
        0xf81b
    };
    int cantidad_colores = sizeof(colores) / sizeof(colores[0]);

    if (cantidad_figuras > cantidad_colores) cantidad_figuras = cantidad_colores;

    printf("\nDibujando %d figuras...", cantidad_figuras);

    int centro_pantalla_x = informacion_variable->xres / 2;
    int centro_pantalla_y = informacion_variable->yres / 2;
    int radio_posicion = (informacion_variable->xres < informacion_variable->yres ? informacion_variable->xres : informacion_variable->yres) / 3;

    for (int indice = 0; indice < cantidad_figuras; indice++) {
        double angulo = (2.0 * PI * indice / cantidad_figuras) - (PI / 2.0);

        int posicion_x = centro_pantalla_x + (int)(radio_posicion * cos(angulo)) - (TAMANO_FIGURA / 2);
        int posicion_y = centro_pantalla_y + (int)(radio_posicion * sin(angulo)) - (TAMANO_FIGURA / 2);

        int color = colores[rand() % cantidad_colores];

        switch (tipo_figura) {
        case 1:
            dibujar_cuadrado(posicion_x, posicion_y, color);
            break;
        case 2:
            dibujar_figuras_aux(posicion_x, posicion_y, color, 5);
            break;
        case 3:
            dibujar_figuras_aux(posicion_x, posicion_y, color, 7);
            break;
        case 4:
            dibujar_figuras_aux(posicion_x, posicion_y, color, 10);
            break;
        }
    }
    printf("¡Listo!\nToque cualquier tecla para continuar...\n");
    obtener_tecla();
}
