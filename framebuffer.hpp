#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <linux/fb.h>

// Constantes globales
#define PI 3.14159265358979323846
#define TAMANO_FIGURA 100

// Variables globales para manejar el framebuffer
extern char *mapa_framebuffer;
extern struct fb_var_screeninfo *informacion_variable;
extern struct fb_fix_screeninfo *informacion_fija;
extern long int tamano_pantalla;

// Funciones de inicializacion y limpieza del framebuffer
int inicializar_framebuffer();
void cerrar_framebuffer(int descriptor_framebuffer);

#endif
