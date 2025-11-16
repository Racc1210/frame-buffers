#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>
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
	/*if (!cur_term) {
		int result;
		setupterm (nullptr, STDOUT_FILENO, &result);
		if (result <= 0)
			return;
	}
	putp (tigetstr ("clear"));*/
}

void handlecommand(char *command)
{
	char c = command[0];
	switch (c)
	{
	case 'T':
		break;
	case 'S':
		sqrPrnt(getnumber(), getnumber(), getnumber(), getnumber(), getnumber());
		break;
	case 'P':
		break;
	case 'H':
		break;
	case 'C':
		break;
	case '#':
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
		else if (p < SIZE)
		{
			buffer[p++] = input;
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

	// Print screen data
	printf("%dx%d, %dbpp\n", vinfo->xres, vinfo->yres, vinfo->bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo->xres * vinfo->yres * vinfo->bits_per_pixel / 8;

	// Map the device to memory
	fbmap = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if ((int)fbmap == -1)
	{
		perror("ERROR: failed to map framebuffer device file to memory");
		exit(4);
	}
	printf("The framebuffer device was mapped to memory successfully.\n");
	printf("Hit any key to begin...");
	getch();

	memset(fbmap, 0, screensize);
	// sqrPrnt (fbmap, &vinfo, &finfo, 100, 100, 200, 300, 0xf00f);
	// sqrPrnt (fbmap, &vinfo, &finfo, 200, 350, 50,  75,  0x0f00);
	// sqrPrnt (fbmap, &vinfo, &finfo, 0,   10,  90,  85,  0x00ff);
	// sqrPrnt (fbmap, &vinfo, &finfo, 50,  50,  300, 300, 0x0777);

	handlekeys();

	munmap(fbmap, screensize);
	close(fbfd);
	return 0;
}
