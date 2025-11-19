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

    // Colores RGB565 brillantes y de alto contraste
    unsigned short colores[] = {
        0xFFFF, // Blanco
        0xF800, // Rojo puro
        0x07E0, // Verde puro
        0x001F, // Azul puro
        0xFFE0, // Amarillo
        0xF81F, // Magenta
        0x07FF, // Cyan
        0xFD20, // Naranja (Rojo+Verde)
        0x841F, // Violeta (Rojo+Azul)
        0x07FF  // Turquesa brillante
    };

    int cantidad_colores = sizeof(colores) / sizeof(colores[0]);

    if (cantidad_figuras > cantidad_colores) cantidad_figuras = cantidad_colores;

    // Crear array temporal con los indices disponibles
    int indices_disponibles[10];
    for (int i = 0; i < cantidad_colores; i++) { 
        indices_disponibles[i] = i;
    }

    int indices_restantes = cantidad_colores;
    printf("\nDibujando %d figuras...", cantidad_figuras);

    int centro_pantalla_x = informacion_variable->xres / 2;
    int centro_pantalla_y = informacion_variable->yres / 2;
    int radio_posicion = (informacion_variable->xres < informacion_variable->yres ? informacion_variable->xres : informacion_variable->yres) / 3;

    for (int indice = 0; indice < cantidad_figuras; indice++) {
        double angulo = (2.0 * PI * indice / cantidad_figuras) - (PI / 2.0);

        int posicion_x = centro_pantalla_x + (int)(radio_posicion * cos(angulo)) - (TAMANO_FIGURA / 2);
        int posicion_y = centro_pantalla_y + (int)(radio_posicion * sin(angulo)) - (TAMANO_FIGURA / 2);

        int pos_aleatoria = rand() % indices_restantes;
        int indice_color = indices_disponibles[pos_aleatoria];
        int color = colores[indice_color];
        
        for (int k = pos_aleatoria; k < indices_restantes - 1; k++) {
            indices_disponibles[k] = indices_disponibles[k + 1];
        }
        indices_restantes--;

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
