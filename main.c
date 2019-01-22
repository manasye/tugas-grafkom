#include "framebuffer.h"
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "test.txt"

int charToInt(char n) {
    return ((int) (n - '0'));
}

int main() {
    char temp;
    FBUFFER fb;
    char intBuffer[5];
    int curX, curY;

    init(&fb);

    printf("Width : %d\nHeight : %d\n",fb.xres,fb.yres);
    scanf("%c",&temp);

    // Turn off the cursor
    system("setterm -cursor off");

    clear(&fb);

    // Open file
    FILE *inputFile = fopen(FILENAME,"r");
    temp = getc(inputFile);
    while(temp != EOF) {
        while((temp != '\n') && (temp != EOF)) {
            curX = 0;
            curY = 0;
            // Fetch X coordinate
            while(temp != ',') {
                curX = (curX * 10) + charToInt(temp);
                temp = getc(inputFile);
            }
            temp = getc(inputFile);
            // Fetch Y coordinate
            while((temp != '\n') && (temp != EOF)) {
                curY = (curY * 10) + charToInt(temp);
                temp = getc(inputFile);
            }
            // Draw pixel
            colorPixel(&fb,curX,curY,WHITE);
            if (temp != EOF) {
                temp = getc(inputFile);
            }
        }
    }
    fclose(inputFile);

    // Pause
    scanf("%c",&temp);
    
    clear(&fb);

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}