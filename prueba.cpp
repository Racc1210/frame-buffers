#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

// Constants
#define SIZE 128

// Global structures
char *fbmap;
struct fb_var_screeninfo *vinfo;
struct fb_fix_screeninfo *finfo;
long int screensize = 0;

// Function prototypes
int getch();
void clearscreen();
void clearfb();
void handlecommand(char *);
void handlekeys();

// Drawing helpers
void putPixel(int x, int y, unsigned int argb);
unsigned int ARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
void drawSquare(int x, int y, int w, int h, unsigned int color);
void fillPolygon(int cx, int cy, int radius, int sides, float rotation, unsigned int color);
void drawRandomShapes(int sides);

// ================= IMPLEMENTACIÓN =================

unsigned int ARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
    return ((unsigned int)a << 24) | ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

void putPixel(int x, int y, unsigned int argb)
{
    if (!vinfo || !finfo || !fbmap)
        return;
    if (x < 0 || y < 0 || x >= (int)vinfo->xres || y >= (int)vinfo->yres)
        return;

    int bpp = vinfo->bits_per_pixel;
    long int offset = (y + vinfo->yoffset) * finfo->line_length + (x + vinfo->xoffset) * (bpp / 8);
    if (offset < 0 || offset + (bpp / 8) > screensize)
        return;

    if (bpp == 32)
    {
        *((unsigned int *)(fbmap + offset)) = argb;
    }
    else if (bpp == 16)
    {
        unsigned char r = (argb >> 16) & 0xFF;
        unsigned char g = (argb >> 8) & 0xFF;
        unsigned char b = (argb) & 0xFF;
        unsigned short rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((unsigned short *)(fbmap + offset)) = rgb565;
    }
}

void drawSquare(int x, int y, int w, int h, unsigned int color)
{
    for (int ty = 0; ty < h; ++ty)
    {
        for (int tx = 0; tx < w; ++tx)
        {
            putPixel(x + tx, y + ty, color);
        }
    }
}

void fillPolygon(int cx, int cy, int radius, int sides, float rotation, unsigned int color)
{
    if (sides < 3)
        return;

    // Calcular vértices
    int vx[sides], vy[sides];
    for (int i = 0; i < sides; ++i)
    {
        float angle = rotation + 2.0f * M_PI * i / sides;
        vx[i] = cx + (int)(cosf(angle) * radius);
        vy[i] = cy + (int)(sinf(angle) * radius);
    }

    // Límites verticales
    int ymin = vy[0], ymax = vy[0];
    for (int i = 1; i < sides; ++i)
    {
        if (vy[i] < ymin)
            ymin = vy[i];
        if (vy[i] > ymax)
            ymax = vy[i];
    }

    // Scanline
    for (int y = ymin; y <= ymax; ++y)
    {
        int interx[sides];
        int count = 0;

        for (int i = 0; i < sides; ++i)
        {
            int j = (i + 1) % sides;
            int x1 = vx[i], y1 = vy[i];
            int x2 = vx[j], y2 = vy[j];

            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y))
            {
                int x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
                interx[count++] = x;
            }
        }

        // Ordenar intersecciones
        for (int i = 0; i < count - 1; ++i)
        {
            for (int j = i + 1; j < count; ++j)
            {
                if (interx[i] > interx[j])
                {
                    int tmp = interx[i];
                    interx[i] = interx[j];
                    interx[j] = tmp;
                }
            }
        }

        // Pintar entre pares
        for (int i = 0; i < count; i += 2)
        {
            if (i + 1 < count)
            {
                for (int x = interx[i]; x <= interx[i + 1]; ++x)
                {
                    putPixel(x, y, color);
                }
            }
        }
    }
}

void drawRandomShapes(int sides)
{
    int count = 2 + rand() % 4; // entre 2 y 5

    unsigned int colors[] = {
        ARGB(0xFF, 0xFF, 0xFF, 0xFF),
        ARGB(0xFF, 0xFF, 0x40, 0x40),
        ARGB(0xFF, 0x40, 0xFF, 0x40),
        ARGB(0xFF, 0x40, 0x40, 0xFF),
        ARGB(0xFF, 0xFF, 0xD7, 0x00)};
    int paletteSize = sizeof(colors) / sizeof(colors[0]);

    int w = (int)vinfo->xres;
    int h = (int)vinfo->yres;

    for (int i = 0; i < count; ++i)
    {
        unsigned int color = colors[rand() % paletteSize];
        int margin = (w < h ? w : h) / 20;
        int cx = margin + rand() % (w - 2 * margin);
        int cy = margin + rand() % (h - 2 * margin);
        int maxRadius = (w < h ? w : h) / 6;
        int minRadius = (w < h ? w : h) / 12;
        int radius = minRadius + rand() % (maxRadius - minRadius + 1);
        float rotation = (float)(rand() % 628) / 100.0f;

        if (sides == 4)
        {
            drawSquare(cx - radius, cy - radius, radius * 2, radius * 2, color);
        }
        else
        {
            fillPolygon(cx, cy, radius, sides, rotation, color);
        }
    }
}

void handlecommand(char *command)
{
    char c = command[0];
    switch (c)
    {
    case 'S':
        drawRandomShapes(4);
        break;
    case 'P':
        drawRandomShapes(5);
        break;
    case 'H':
        drawRandomShapes(7);
        break;
    case 'D':
        drawRandomShapes(10);
        break;
    case '#':
        clearfb();
        clearscreen();
        break;
    default:
        printf("\nComandos:\n");
        printf("  S = Cuadrados\n");
        printf("  P = Pentágonos\n");
        printf("  H = Heptágonos\n");
        printf("  D = Decágonos\n");
        printf("  # = Limpiar pantalla\n");
        printf("  x = Salir\n\n");
        break;
    }
}

int getch()
{
    int c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

void clearscreen()
{
    printf("\033[2J\033[H");
    fflush(stdout);
}

void clearfb()
{
    if (fbmap && screensize > 0)
    {
        memset(fbmap, 0, screensize);
    }
}

unsigned int ARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
{
    return ((unsigned int)a << 24) | ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b;
}

void putPixel(int x, int y, unsigned int argb)
{
    if (!vinfo || !finfo || !fbmap)
        return;
    if (x < 0 || y < 0 || x >= (int)vinfo->xres || y >= (int)vinfo->yres)
        return;

    int bpp = vinfo->bits_per_pixel;
    long int offset = (y + vinfo->yoffset) * finfo->line_length + (x + vinfo->xoffset) * (bpp / 8);
    if (offset < 0 || offset + (bpp / 8) > screensize)
        return;

    if (bpp == 32)
    {
        // Escribir ARGB8888 (ajusta si tu framebuffer usa BGRA)
        *((unsigned int *)(fbmap + offset)) = argb;
    }
    else if (bpp == 16)
    {
        // Convertir ARGB8888 a RGB565
        unsigned char r = (argb >> 16) & 0xFF;
        unsigned char g = (argb >> 8) & 0xFF;
        unsigned char b = (argb) & 0xFF;
        unsigned short rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
        *((unsigned short *)(fbmap + offset)) = rgb565;
    }
    else
    {
        // Otros formatos no soportados: escribir negro
        memset(fbmap + offset, 0, bpp / 8);
    }
}

void drawLine(int x0, int y0, int x1, int y1, unsigned int color)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1)
    {
        putPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void drawPolygon(int cx, int cy, int radius, int sides, float rotation, unsigned int color)
{
    if (sides < 3)
        return;
    int prevx = 0, prevy = 0;
    for (int i = 0; i <= sides; ++i)
    {
        float angle = rotation + 2.0f * M_PI * (float)i / (float)sides;
        int x = cx + (int)(cosf(angle) * radius);
        int y = cy + (int)(sinf(angle) * radius);
        if (i > 0)
        {
            drawLine(prevx, prevy, x, y, color);
        }
        prevx = x;
        prevy = y;
    }
}

void drawRandomShapes(int sides)
{
    // Entre 2 y 5 figuras
    int count = 2 + rand() % 4;

    // Paleta simple
    unsigned int colors[] = {
        ARGB(0xFF, 0xFF, 0xFF, 0xFF), // blanco
        ARGB(0xFF, 0xFF, 0x40, 0x40), // rojo suave
        ARGB(0xFF, 0x40, 0xFF, 0x40), // verde suave
        ARGB(0xFF, 0x40, 0x40, 0xFF), // azul suave
        ARGB(0xFF, 0xFF, 0xD7, 0x00)  // goldenrod
    };
    int paletteSize = sizeof(colors) / sizeof(colors[0]);

    int w = (int)vinfo->xres;
    int h = (int)vinfo->yres;

    for (int i = 0; i < count; ++i)
    {
        unsigned int color = colors[rand() % paletteSize];

        // Margen para que no salga de pantalla
        int margin = (w < h ? w : h) / 20;
        int cx = margin + rand() % (w - 2 * margin);
        int cy = margin + rand() % (h - 2 * margin);

        int maxRadius = (w < h ? w : h) / 6;
        int minRadius = (w < h ? w : h) / 12;
        int radius = minRadius + rand() % (maxRadius - minRadius + 1);
        float rotation = (float)(rand() % 628) / 100.0f; // ~0..6.28

        if (sides == 4)
        {
            // Cuadrado relleno: inscribir en un cuadrado de lado 2*radius
            int side = radius * 2;
            int x = cx - side / 2;
            int y = cy - side / 2;
            drawSquare(x, y, side, side, color);
        }
        else
        {
            // Polígono regular dibujado con líneas
            drawPolygon(cx, cy, radius, sides, rotation, color);
        }
    }
}

void handlecommand(char *command)
{
    char c = command[0];
    switch (c)
    {
    case 'S': // Square (random, 2-5)
        drawRandomShapes(4);
        break;
    case 'P': // Pentagon (random, 2-5)
        drawRandomShapes(5);
        break;
    case 'H': // Heptagon (random, 2-5)
        drawRandomShapes(7);
        break;
    case 'D': // Decagon (random, 2-5)
        drawRandomShapes(10);
        break;
    case '#': // Blank framebuffer + consola
        clearfb();
        clearscreen();
        break;
    default:
        // Mostrar ayuda rápida
        printf("\nComandos:\n");
        printf("  S = Cuadrados aleatorios (2-5)\n");
        printf("  P = Pentágonos aleatorios (2-5)\n");
        printf("  H = Heptágonos aleatorios (2-5)\n");
        printf("  D = Decágonos aleatorios (2-5)\n");
        printf("  # = Limpiar pantalla\n");
        printf("  x = Salir\n\n");
        break;
    }
}

int getch()
{
    int c;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}

void handlekeys()
{
    char input = 0;
    char buffer[SIZE + 1];
    int p = 0;
    memset(buffer, 0, SIZE + 1);
    clearscreen();
    printf("Comandos: S (cuadrados), P (pentágonos), H (heptágonos), D (decágonos), # (limpiar), x (salir)\n");
    printf("Type a command: ");
    fflush(stdout);
    do
    {
        input = getch();
        if (input == 10) // Enter
        {
            printf("\r\033[K");
            handlecommand(buffer);
            memset(buffer, 0, SIZE + 1);
            p = 0;
            printf("Type a command: ");
            fflush(stdout);
        }
        else if ((input == 127 || input == 8) && p > 0) // Backspace
        {
            p--;
            buffer[p] = 0;
            printf("\r\033[KType a command: %s", buffer);
            fflush(stdout);
        }
        else if (input >= 32 && input < 127 && p < SIZE) // Caracteres imprimibles
        {
            buffer[p++] = input;
            buffer[p] = 0;
            printf("\r\033[KType a command: %s", buffer);
            fflush(stdout);
        }
    } while (!(input == 'x' && p == 1));
}

int main()
{
    int fbfd = 0;

    vinfo = (struct fb_var_screeninfo *)malloc(sizeof(struct fb_var_screeninfo));
    finfo = (struct fb_fix_screeninfo *)malloc(sizeof(struct fb_fix_screeninfo));
    if (!vinfo || !finfo)
    {
        fprintf(stderr, "Error: memoria insuficiente\n");
        return 1;
    }

    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd < 0)
    {
        perror("Error: cannot open framebuffer device file...");
        exit(1);
    }
    printf("Framebuffer device file opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, finfo) < 0)
    {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen informacion
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, vinfo) < 0)
    {
        perror("Error reading variable information");
        exit(3);
    }

    // Figure out the size of the mapped region in bytes (usar line_length)
    screensize = finfo->line_length * vinfo->yres;

    // Map the device to memory
    fbmap = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbmap == MAP_FAILED)
    {
        perror("ERROR: failed to map framebuffer device file to memory");
        exit(4);
    }

    // Seed de aleatoriedad
    srand((unsigned int)time(NULL));

    // Print screen data
    printf("%dx%d, %dbpp, line_length=%d\n", vinfo->xres, vinfo->yres, vinfo->bits_per_pixel, finfo->line_length);
    printf("The framebuffer device was mapped to memory successfully.\n");
    printf("Hit any key to begin...");
    getch();

    clearfb();
    handlekeys();

    munmap(fbmap, screensize);
    close(fbfd);
    free(vinfo);
    free(finfo);
    return 0;
}
