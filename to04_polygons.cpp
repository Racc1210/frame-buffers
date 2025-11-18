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
#define TAMANO_FIGURA 50  // Tamano fijo para todas las figuras

// Estructuras globales para manejar el framebuffer
char *mapa_framebuffer;
struct fb_var_screeninfo *informacion_variable;
struct fb_fix_screeninfo *informacion_fija;
long int tamano_pantalla = 0;

// Prototipos de funciones
int obtener_tecla();
void limpiar_pantalla();
void dibujar_cuadrado(int posicion_x, int posicion_y, int color);
void dibujar_pentagono(int posicion_x, int posicion_y, int color);
void dibujar_heptagono(int posicion_x, int posicion_y, int color);
void dibujar_decagono(int posicion_x, int posicion_y, int color);
void mostrar_menu();
void dibujar_figuras_aleatorias(int tipo_figura);

// Dibuja un cuadrado de tamano fijo (similar a sqrPrnt)
void dibujar_cuadrado(int posicion_x, int posicion_y, int color)
{
	int ancho = TAMANO_FIGURA;
	int alto = TAMANO_FIGURA;
	
	for (int fila = 0; fila < alto; fila++)
	{
		for (int columna = 0; columna < ancho; columna++)
		{
			int pixel_x = posicion_x + columna;
			int pixel_y = posicion_y + fila;
			
			// Verificar limites
			if (pixel_x >= informacion_variable->xres || pixel_y >= informacion_variable->yres)
				continue;
				
			// Calcular posicion en el framebuffer
			long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
								 (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);
			
			if (ubicacion < tamano_pantalla)
				*((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
		}
	}
}

// Dibuja un pentagono usando puntos calculados
void dibujar_pentagono(int posicion_x, int posicion_y, int color)
{
	int centro_x = posicion_x + TAMANO_FIGURA / 2;
	int centro_y = posicion_y + TAMANO_FIGURA / 2;
	int radio = TAMANO_FIGURA / 2;
	
	// Dibujar el pentagono rellenando con puntos
	for (int fila = 0; fila < TAMANO_FIGURA; fila++)
	{
		for (int columna = 0; columna < TAMANO_FIGURA; columna++)
		{
			int pixel_x = posicion_x + columna;
			int pixel_y = posicion_y + fila;
			
			// Calcular distancia desde el centro
			int distancia_x = pixel_x - centro_x;
			int distancia_y = pixel_y - centro_y;
			double distancia = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
			
			// Si esta dentro del radio, dibujarlo
			if (distancia <= radio)
			{
				if (pixel_x >= informacion_variable->xres || pixel_y >= informacion_variable->yres)
					continue;
					
				long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
									 (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);
				
				if (ubicacion < tamano_pantalla)
					*((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
			}
		}
	}
}

// Dibuja un heptagono (simplificado como circulo)
void dibujar_heptagono(int posicion_x, int posicion_y, int color)
{
	int centro_x = posicion_x + TAMANO_FIGURA / 2;
	int centro_y = posicion_y + TAMANO_FIGURA / 2;
	int radio = TAMANO_FIGURA / 2;
	
	for (int fila = 0; fila < TAMANO_FIGURA; fila++)
	{
		for (int columna = 0; columna < TAMANO_FIGURA; columna++)
		{
			int pixel_x = posicion_x + columna;
			int pixel_y = posicion_y + fila;
			
			int distancia_x = pixel_x - centro_x;
			int distancia_y = pixel_y - centro_y;
			double distancia = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
			
			if (distancia <= radio)
			{
				if (pixel_x >= informacion_variable->xres || pixel_y >= informacion_variable->yres)
					continue;
					
				long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
									 (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);
				
				if (ubicacion < tamano_pantalla)
					*((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
			}
		}
	}
}

// Dibuja un decagono (simplificado como circulo)
void dibujar_decagono(int posicion_x, int posicion_y, int color)
{
	int centro_x = posicion_x + TAMANO_FIGURA / 2;
	int centro_y = posicion_y + TAMANO_FIGURA / 2;
	int radio = TAMANO_FIGURA / 2;
	
	for (int fila = 0; fila < TAMANO_FIGURA; fila++)
	{
		for (int columna = 0; columna < TAMANO_FIGURA; columna++)
		{
			int pixel_x = posicion_x + columna;
			int pixel_y = posicion_y + fila;
			
			int distancia_x = pixel_x - centro_x;
			int distancia_y = pixel_y - centro_y;
			double distancia = sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
			
			if (distancia <= radio)
			{
				if (pixel_x >= informacion_variable->xres || pixel_y >= informacion_variable->yres)
					continue;
					
				long int ubicacion = (pixel_y + informacion_variable->yoffset) * informacion_fija->line_length +
									 (pixel_x + informacion_variable->xoffset) * (informacion_variable->bits_per_pixel / 8);
				
				if (ubicacion < tamano_pantalla)
					*((unsigned short int *)(mapa_framebuffer + ubicacion)) = color;
			}
		}
	}
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
    int cantidad_figuras = (rand() % 4) + 2;

    // Arreglo de colores aleatorios (colores en formato RGB565)
    int colores[] = {0xF800, 0x07E0, 0x001F, 0xFFE0, 0xF81F, 0x07FF, 0xFFFF};
    int cantidad_colores = 7;

    printf("\nDibujando %d figuras...\n", cantidad_figuras);

    for (int indice = 0; indice < cantidad_figuras; indice++)
    {
        // Posicion aleatoria (evitar los bordes)
        int posicion_x = (rand() % (informacion_variable->xres - TAMANO_FIGURA - 100)) + 50;
        int posicion_y = (rand() % (informacion_variable->yres - TAMANO_FIGURA - 100)) + 50;

        // Color aleatorio
        int color = colores[rand() % cantidad_colores];

        // Dibujar el tipo de figura seleccionado
        switch (tipo_figura)
        {
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
