#include <assert.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define FILENAME "test.txt"

uint32_t *buf;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int xres;
int yres;

void init() {
    // Getting the framebuffer
    int fb = open("/dev/fb0", O_RDWR);
    assert(fb > 0);
    // Getting framebuffer information - width, height
    assert(ioctl(fb, FBIOGET_VSCREENINFO, &vinfo) == 0);
    assert(ioctl(fb, FBIOGET_FSCREENINFO, &finfo) == 0);
    xres = finfo.line_length / 4;
    yres = vinfo.yres_virtual;
    // Getting the framebuffer memory
    buf = mmap(NULL, finfo.line_length * yres, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);
    assert(buf != MAP_FAILED);
}

void colorPixel(int x, int y, uint32_t rgb) {
    buf[y * xres + x] = rgb;
}

int charToInt(char n) {
    return ((int) (n - '0'));
}

void clear() {
    int i, j;
    for (j = 0;j < yres; j++) {
        for (i = 0;i < xres; i++) {
            colorPixel(i,j,BLACK);
        }
    }
}

int main() {
    char temp;
    char intBuffer[5];
    int curX, curY;
    // Turn off the cursor
    system("setterm -cursor off");

    init();

    printf("Width : %d\nHeight : %d\n",xres,yres);
    scanf("%c",&temp);

    clear();

    // Open file
    FILE *inputFile = fopen(FILENAME,"r");
    temp = getc(inputFile);
    while(temp != EOF) {
        while((temp != '\n') && (temp != EOF)) {
            curX = 0;
            curY = 0;
            while(temp != ',') {
                curX = (curX * 10) + charToInt(temp);
                temp = getc(inputFile);
            }
            temp = getc(inputFile);
            while((temp != '\n') && (temp != EOF)) {
                curY = (curY * 10) + charToInt(temp);
                temp = getc(inputFile);
            }
            colorPixel(curX,curY,WHITE);
            if (temp != EOF) {
                temp = getc(inputFile);
            }
        }
    }
    fclose(inputFile);

    // Pause
    scanf("%c",&temp);
    
    clear();

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}
