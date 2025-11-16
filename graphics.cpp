#include <fcntl.h>
#include <linux/fb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

void sqrPrnt (char *fbmap, struct fb_var_screeninfo *vinfo, struct fb_fix_screeninfo *finfo,
	      int x, int y, int w, int h, int base) {
	long int l = 0;
	int ty = 0;
	while (ty < h) {
		int tx = 0;
		while (tx < w) {
			l = ((y + ty + vinfo->yoffset) * finfo->line_length) +
			    ((x + tx + vinfo->xoffset) * (vinfo->bits_per_pixel / 8));
			*((unsigned short int*) (fbmap + l)) = base;
			tx++;
		}
		ty++;
	}
}

int main () {
	int fbfd = 0;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize = 0;
	char *fbmap = 0;

	// Open the file for reading and writing
	fbfd = open ("/dev/fb0", O_RDWR);
	if (fbfd < 0) {
		perror ("Error: cannot open framebuffer device file...");
		exit (1);
	}
	printf ("Framebuffer device file opened successfully.\n");

	// Get fixed and variable screen information
	if (ioctl (fbfd, FBIOGET_FSCREENINFO, &finfo) < 0) {
		perror ("Error reading fixed information");
		exit (2);
	}
	if (ioctl (fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
		perror ("Error reading variable information");
		exit (3);
	}

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;
	// Map the device to memory
	fbmap = (char*) mmap (0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	if (fbmap == MAP_FAILED) {
		perror ("ERROR: failed to map framebuffer device file to memory");
		exit (4);
	}

	// Print screen data
	printf ("The framebuffer device was mapped to memory successfully.\n");
	printf ("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	//memset (fbmap, 0, screensize);
	sqrPrnt (fbmap, &vinfo, &finfo, 100, 100, 200, 300, 0xf00f);
	sqrPrnt (fbmap, &vinfo, &finfo, 200, 350, 50,  75,  0x0f00);
	sqrPrnt (fbmap, &vinfo, &finfo, 0,   10,  90,  85,  0x00ff);
	sqrPrnt (fbmap, &vinfo, &finfo, 50,  50,  300, 300, 0x0777);

	munmap (fbmap, screensize);
	close (fbfd);
	getchar ();
	return 0;
}

