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

void dibujar_pentagono(int posicion_x, int posicion_y, int color) {
    int centro_x = posicion_x + TAMANO_FIGURA / 2;
    int centro_y = posicion_y + TAMANO_FIGURA / 2;
    int radio = TAMANO_FIGURA / 2;
    int numero_lados = 5;

    int vertices_x[5];
    int vertices_y[5];

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

void dibujar_heptagono(int posicion_x, int posicion_y, int color) {
    int centro_x = posicion_x + TAMANO_FIGURA / 2;
    int centro_y = posicion_y + TAMANO_FIGURA / 2;
    int radio = TAMANO_FIGURA / 2;
    int numero_lados = 7;

    int vertices_x[7];
    int vertices_y[7];

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

void dibujar_decagono(int posicion_x, int posicion_y, int color) {
    int centro_x = posicion_x + TAMANO_FIGURA / 2;
    int centro_y = posicion_y + TAMANO_FIGURA / 2;
    int radio = TAMANO_FIGURA / 2;
    int numero_lados = 10;

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

    unsigned short colores[] = {0xFFFF, 0xF800, 0x07E0, 0x001F,
        0xFFE0, 0xF81F, 0x07FF, 0xFD20, 0xAFE5, 0xF7DE  };

    int cantidad_colores = sizeof(colores) / sizeof(colores[0]);

    unsigned short colores_usados[10];

    for (int i = 0; i < cantidad_colores; i++) {
        colores_usados[i] = colores[i];
    }

    for (int i = cantidad_colores - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        unsigned short temp = colores_usados[i];
        colores_usados[i] = colores_usados[j];
        colores_usados[j] = temp;
    }

    printf("\nDibujando %d figuras...", cantidad_figuras);

    int centro_pantalla_x = informacion_variable->xres / 2;
    int centro_pantalla_y = informacion_variable->yres / 2;

    int radio_posicion = (informacion_variable->xres < informacion_variable->yres ? informacion_variable->xres : informacion_variable->yres) / 3;

    for (int indice = 0; indice < cantidad_figuras; indice++) {
        double angulo = (2.0 * PI * indice / cantidad_figuras) - (PI / 2.0);

        int posicion_x = centro_pantalla_x + (int)(radio_posicion * cos(angulo)) - (TAMANO_FIGURA / 2);
        int posicion_y = centro_pantalla_y + (int)(radio_posicion * sin(angulo)) - (TAMANO_FIGURA / 2);

        int color = colores_usados[indice];

        switch (tipo_figura) {
        case 1:
            dibujar_cuadrado(posicion_x, posicion_y, color);
            break;
        case 2:
            dibujar_pentagono(posicion_x, posicion_y, color);
            break;
        case 3:
            dibujar_heptagono(posicion_x, posicion_y, color);
            break;
        case 4:
            dibujar_decagono(posicion_x, posicion_y, color);
            break;
        }
    }
    printf("¡Listo!\nToque cualquier tecla para continuar...\n");
    obtener_tecla();
}
