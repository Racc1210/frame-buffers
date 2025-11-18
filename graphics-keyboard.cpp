#include <fcntl.h>
#include <linux/fb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <unistd.h>

// Constants
#define SIZE 128

// Global structures
char *fbmap;
struct fb_var_screeninfo *vinfo;
struct fb_fix_screeninfo *finfo;
long int screensize = 0;

// Function prototypes
int getch();
int getnumber();
void clearscreen();
void sqrPrnt(int, int, int, int, int);
void handlecommand(char *);
void handlekeys();

// Function implementation
void sqrPrnt(int x, int y, int w, int h, int base)
{
	long int l = 0;

	int ty = 0;
	while (ty < h)
	{
		int tx = 0;
		while (tx < w)
		{
			int px = x + tx;
			if (px >= vinfo->xres)
				break;
			l = ((y + ty + vinfo->yoffset) * finfo->line_length) +
				((px + vinfo->xoffset) * (vinfo->bits_per_pixel / 8));

			if (l < screensize)
				*((unsigned short int *)(fbmap + l)) = base;
			else
				return;

			tx++;
		}
		ty++;
	}
}

void clearscreen()
{
	system("clear");
	if (!cur_term)
	{
		int result;
		setupterm(nullptr, STDOUT_FILENO, &result);
		if (result <= 0)
			return;
	}
	putp(tigetstr("clear"));
}

void handlecommand(char *command)
{
	char c = command[0];
	switch (c)
	{
	case 'T': // Triangle (pending)
		break;
	case 'S': // Square
		sqrPrnt(getnumber(), getnumber(), getnumber(), getnumber(), getnumber());
		break;
	case 'P': // Pentagon (pending)
		break;
	case 'H': // Hexagon (pending)
		break;
	case 'C': // Circle (pending)
		break;
	case '#': // Blank screen
		clearscreen();
		break;
	default:
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

int getnumber()
{
	printf("\r#: ");
	int n = 0;
	char c;
	do
	{
		c = getch();
		if (c > 47 && c < 58)
		{
			printf("%c", c);
			n = (n * 10) + (c - 48);
		}
	} while (c != 10);
	printf("!");
	return n;
}

void handlekeys()
{
	char input = 0;
	char buffer[SIZE + 1];
	int p = 0;
	memset(buffer, 0, SIZE + 1);
	clearscreen();
	printf("Type a command: ");
	do
	{
		input = getch();
		if (input == 10)
		{
			printf("\r");
			for (int i = 0; i < p + 16; i++)
				printf(" ");
			handlecommand(buffer);
			memset(buffer, 32, p);
			printf("\rType a command: %s", buffer);
			memset(buffer, 0, p + 1);
			p = 0;
		}
		else if ((input == 127 || input == 8) && p > 0)
		{ // Backspace
			p--;
			buffer[p] = 0;
			// Limpia la línea antes de imprimir el buffer
			printf("\rType a command: ");
			for (int i = 0; i < SIZE; i++)
				printf(" ");
			printf("\rType a command: %s", buffer);
		}
		else if (input >= 32 && input < 127 && p < SIZE)
		{ // Caracteres imprimibles
			buffer[p++] = input;
			buffer[p] = 0;
			printf("\rType a command: ");
			for (int i = 0; i < SIZE; i++)
				printf(" ");
			printf("\rType a command: %s", buffer);
		}
	} while (!(input == 'x' && p == 1));
}

int main()
{
	int fbfd = 0;

	vinfo = new struct fb_var_screeninfo;
	finfo = new struct fb_fix_screeninfo;

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

	// Figure out the size of the screen in bytes
	screensize = vinfo->xres * vinfo->yres * vinfo->bits_per_pixel / 8;

	// Map the device to memory
	fbmap = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (fbmap == MAP_FAILED)
	{
		perror("ERROR: failed to map framebuffer device file to memory");
		exit(4);
	}

	// Print screen data
	printf("%dx%d, %dbpp\n", vinfo->xres, vinfo->yres, vinfo->bits_per_pixel);
	printf("The framebuffer device was mapped to memory successfully.\n");
	printf("Hit any key to begin...");
	getch();

	memset(fbmap, 0, screensize);
	handlekeys();

	munmap(fbmap, screensize);
	close(fbfd);
	return 0;
}
