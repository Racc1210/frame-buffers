#include "framebuffer.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *mapa_framebuffer;
struct fb_var_screeninfo *informacion_variable;
struct fb_fix_screeninfo *informacion_fija;
long int tamano_pantalla = 0;

int inicializar_framebuffer() {
    int descriptor_framebuffer = 0;

    informacion_variable = new struct fb_var_screeninfo;
    informacion_fija = new struct fb_fix_screeninfo;

    descriptor_framebuffer = open("/dev/fb0", O_RDWR);
    if (descriptor_framebuffer < 0) {
        perror("Error: no se puede abrir el framebuffer");
        exit(1);
    }
    printf("Framebuffer abierto exitosamente.\n");

    if (ioctl(descriptor_framebuffer, FBIOGET_FSCREENINFO, informacion_fija) < 0) {
        perror("Error obteniendo informacion fija");
        exit(2);
    }

    if (ioctl(descriptor_framebuffer, FBIOGET_VSCREENINFO, informacion_variable) < 0) {
        perror("Error obteniendo informacion variable");
        exit(3);
    }
    tamano_pantalla = informacion_variable->xres * informacion_variable->yres *
                      informacion_variable->bits_per_pixel / 8;

    mapa_framebuffer = (char *)mmap(0, tamano_pantalla, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, descriptor_framebuffer, 0);

    if (mapa_framebuffer == MAP_FAILED) {
        perror("Error mapeando el framebuffer a memoria");
        exit(4);
    }

    printf("Resolucion: %dx%d, %d bits por pixel\n",
           informacion_variable->xres, informacion_variable->yres,
           informacion_variable->bits_per_pixel);
    printf("Framebuffer mapeado exitosamente.\n");

    return descriptor_framebuffer;
}

void cerrar_framebuffer(int descriptor_framebuffer) {
    memset(mapa_framebuffer, 0, tamano_pantalla);
    munmap(mapa_framebuffer, tamano_pantalla);
    close(descriptor_framebuffer);
    delete informacion_variable;
    delete informacion_fija;
}
