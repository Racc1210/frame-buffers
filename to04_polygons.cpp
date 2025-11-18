#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

// Constantes
#define PI 3.14159265358979323846

// Estructuras globales para manejar el framebuffer
char *mapa_framebuffer;
struct fb_var_screeninfo *informacion_variable;
struct fb_fix_screeninfo *informacion_fija;
long int tamano_pantalla = 0;

// Prototipos de funciones
int obtener_tecla();
void limpiar_pantalla();
void dibujar_pixel(int posicion_x, int posicion_y, int color);
void dibujar_linea(int posicion_x1, int posicion_y1, int posicion_x2, int posicion_y2, int color);
void dibujar_cuadrado(int centro_x, int centro_y, int tamano, int color);
void dibujar_pentagono(int centro_x, int centro_y, int tamano, int color);
void dibujar_heptagono(int centro_x, int centro_y, int tamano, int color);
void dibujar_decagono(int centro_x, int centro_y, int tamano, int color);
void dibujar_poligono(int centro_x, int centro_y, int tamano, int numero_lados, int color);
void mostrar_menu();
void dibujar_figuras_aleatorias(int tipo_figura);

// Funcion auxiliar: Dibuja un solo pixel en la pantalla
void dibujar_pixel(int posicion_x, int posicion_y, int color)
{
    // Verificar que las coordenadas esten dentro de los limites
    if (posicion_x < 0 || posicion_x >= (int)informacion_variable->xres ||
        posicion_y < 0 || posicion_y >= (int)informacion_variable->yres)
        return;

    // Calcular la posicion en el framebuffer
    long int ubicacion = (posicion_y + informacion_variable->yoffset) * informacion_fija->line_length +
                         (posicion_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);

    if (ubicacion < tamano_pantalla)
        *((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
}

// Funcion auxiliar: Dibuja una linea usando el algoritmo de Bresenham
void dibujar_linea(int posicion_x1, int posicion_y1, int posicion_x2, int posicion_y2, int color)
{
    int diferencia_x = abs(posicion_x2 - posicion_x1);
    int diferencia_y = abs(posicion_y2 - posicion_y1);
    int paso_x = (posicion_x1 < posicion_x2) ? 1 : -1;
    int paso_y = (posicion_y1 < posicion_y2) ? 1 : -1;
    int error = diferencia_x - diferencia_y;

    while (1)
    {
        dibujar_pixel(posicion_x1, posicion_y1, color);

        if (posicion_x1 == posicion_x2 && posicion_y1 == posicion_y2)
            break;

        int error_doble = 2 * error;
        if (error_doble > -diferencia_y)
        {
            error -= diferencia_y;
            posicion_x1 += paso_x;
        }
        if (error_doble < diferencia_x)
        {
            error += diferencia_x;
            posicion_y1 += paso_y;
        }
    }
}

// Funcion generica para dibujar cualquier poligono regular
void dibujar_poligono(int centro_x, int centro_y, int tamano, int numero_lados, int color)
{
    // Arreglos para almacenar las coordenadas de los vertices
    int vertices_posicion_x[20]; // Maximo 20 lados
    int vertices_posicion_y[20];

    // Calcular la posicion de cada vertice del poligono
    for (int indice = 0; indice < numero_lados; indice++)
    {
        // Calcular el angulo para este vertice (empezar desde arriba, rotar en sentido horario)
        double angulo = (2.0 * PI * indice / numero_lados) - (PI / 2.0);
        vertices_posicion_x[indice] = centro_x + (int)(tamano * cos(angulo));
        vertices_posicion_y[indice] = centro_y + (int)(tamano * sin(angulo));
    }

    // Dibujar lineas conectando todos los vertices consecutivos
    for (int indice = 0; indice < numero_lados; indice++)
    {
        int siguiente = (indice + 1) % numero_lados;
        dibujar_linea(vertices_posicion_x[indice], vertices_posicion_y[indice],
                      vertices_posicion_x[siguiente], vertices_posicion_y[siguiente], color);
    }
}

// Dibuja un cuadrado (4 lados)
void dibujar_cuadrado(int centro_x, int centro_y, int tamano, int color)
{
    dibujar_poligono(centro_x, centro_y, tamano, 4, color);
}

// Dibuja un pentagono (5 lados)
void dibujar_pentagono(int centro_x, int centro_y, int tamano, int color)
{
    dibujar_poligono(centro_x, centro_y, tamano, 5, color);
}

// Dibuja un heptagono (7 lados)
void dibujar_heptagono(int centro_x, int centro_y, int tamano, int color)
{
    dibujar_poligono(centro_x, centro_y, tamano, 7, color);
}

// Dibuja un decagono (10 lados)
void dibujar_decagono(int centro_x, int centro_y, int tamano, int color)
{
    dibujar_poligono(centro_x, centro_y, tamano, 10, color);
}

// Limpia la pantalla usando secuencias ANSI
void limpiar_pantalla()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

// Obtiene un caracter sin mostrar eco en pantalla
int obtener_tecla()
{
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

// Muestra el menu principal y obtiene la seleccion del usuario
void mostrar_menu()
{
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

// Dibuja entre 2 y 5 figuras aleatorias del tipo seleccionado
void dibujar_figuras_aleatorias(int tipo_figura)
{
    // Limpiar el framebuffer (pintarlo de negro)
    memset(mapa_framebuffer, 0, tamano_pantalla);

    // Numero aleatorio de figuras (entre 2 y 5)
    int cantidad_figuras = (rand() % 4) + 2; // rand() % 4 da 0-3, +2 da 2-5

    // Arreglo de colores aleatorios (colores en formato RGB565)
    int colores[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xFFFF};
    int cantidad_colores = 7;

    printf("\nDibujando %d figuras...\n", cantidad_figuras);

    for (int indice = 0; indice < cantidad_figuras; indice++)
    {
        // Posicion aleatoria (evitar los bordes)
        int centro_x = (rand() % (informacion_variable->xres - 200)) + 100;
        int centro_y = (rand() % (informacion_variable->yres - 200)) + 100;

        // Tamano aleatorio (entre 30 y 80 pixeles)
        int tamano = (rand() % 51) + 30;

        // Color aleatorio
        int color = colores[rand() % cantidad_colores];

        // Dibujar el tipo de figura seleccionado
        switch (tipo_figura)
        {
        case 1:
            dibujar_cuadrado(centro_x, centro_y, tamano, color);
            break;
        case 2:
            dibujar_pentagono(centro_x, centro_y, tamano, color);
            break;
        case 3:
            dibujar_heptagono(centro_x, centro_y, tamano, color);
            break;
        case 4:
            dibujar_decagono(centro_x, centro_y, tamano, color);
            break;
        }
    }

    printf("¡Listo! Presiona cualquier tecla para continuar...\n");
    obtener_tecla();
}

int main()
{
    int descriptor_framebuffer = 0;

    // Inicializar la semilla aleatoria
    srand(time(NULL));

    informacion_variable = new struct fb_var_screeninfo;
    informacion_fija = new struct fb_fix_screeninfo;

    // Abrir el dispositivo framebuffer
    descriptor_framebuffer = open("/dev/fb0", O_RDWR);
    if (descriptor_framebuffer < 0)
    {
        perror("Error: no se puede abrir el framebuffer");
        exit(1);
    }
    printf("Framebuffer abierto exitosamente.\n");

    // Obtener informacion fija de la pantalla
    if (ioctl(descriptor_framebuffer, FBIOGET_FSCREENINFO, informacion_fija) < 0)
    {
        perror("Error obteniendo informacion fija");
        exit(2);
    }

    // Obtener informacion variable de la pantalla
    if (ioctl(descriptor_framebuffer, FBIOGET_VSCREENINFO, informacion_variable) < 0)
    {
        perror("Error obteniendo informacion variable");
        exit(3);
    }

    // Calcular el tamano de la pantalla en bytes
    tamano_pantalla = informacion_variable->xres * informacion_variable->yres * informacion_variable->bits_per_pixel / 8;

    // Mapear el framebuffer a memoria
    mapa_framebuffer = (char *)mmap(0, tamano_pantalla, PROT_READ | PROT_WRITE, MAP_SHARED, descriptor_framebuffer, 0);
    if (mapa_framebuffer == MAP_FAILED)
    {
        perror("Error mapeando el framebuffer a memoria");
        exit(4);
    }

    printf("Resolucion: %dx%d, %d bits por pixel\n", informacion_variable->xres, informacion_variable->yres, informacion_variable->bits_per_pixel);
    printf("Framebuffer mapeado exitosamente.\n");
    printf("Presiona cualquier tecla para comenzar...\n");
    obtener_tecla();

    // Ciclo principal del programa
    int opcion = 0;
    while (opcion != 5)
    {
        mostrar_menu();
        opcion = obtener_tecla() - '0'; // Convertir caracter a numero ('1' -> 1)

        if (opcion >= 1 && opcion <= 4)
        {
            limpiar_pantalla();
            dibujar_figuras_aleatorias(opcion);
        }
        else if (opcion == 5)
        {
            limpiar_pantalla();
            printf("¡Hasta luego!\n");
        }
        else
        {
            limpiar_pantalla();
            printf("Opcion invalida. Presiona cualquier tecla...\n");
            obtener_tecla();
        }
    }

    // Limpieza y cierre
    memset(mapa_framebuffer, 0, tamano_pantalla); // Limpiar pantalla antes de salir
    munmap(mapa_framebuffer, tamano_pantalla);
    close(descriptor_framebuffer);

    delete informacion_variable;
    delete informacion_fija;

    return 0;
}
