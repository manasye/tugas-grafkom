#include "framebuffer.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"

int charToInt(char n)
{
    return ((int)(n - '0'));
}

int main()
{
    char temp;
    FBUFFER fb;
    char intBuffer[5];
    int curX, curY;

    init(&fb);

    printf("Width : %d\nHeight : %d\n", fb.xres, fb.yres);
    scanf("%c", &temp);

    // Turn off the cursor
    system("setterm -cursor off");

    clear(&fb);

    drawLine(fb, 10, 10, 300, 400);

    // Pause
    scanf("%c", &temp);

    clear(&fb);

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}