#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <linux/fb.h>

#define PI 3.14159265358979323846
#define TAMANO_FIGURA 80

extern char *mapa_framebuffer;
extern struct fb_var_screeninfo *informacion_variable;
extern struct fb_fix_screeninfo *informacion_fija;
extern long int tamano_pantalla;

int inicializar_framebuffer();
void cerrar_framebuffer(int descriptor_framebuffer);

#endif
