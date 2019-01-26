#include "framebuffer.h"
#include "line.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int curX0, curY0, curX1, curY1;
    char colorHex[9];

    init(&fb);

    printf("Width : %d\nHeight : %d\n", fb.xres, fb.yres);
    scanf("%c", &temp);

    // Turn off the cursor
    system("setterm -cursor off");

    clear(&fb);

    // Open file
    FILE *inputFile = fopen(FILENAME, "r");
    temp = getc(inputFile);
    while (temp != EOF)
    {
        while ((temp != '\n') && (temp != EOF))
        {
            curX0 = 0;
            curY0 = 0;
            curX1 = 0;
            curY1 = 0;

            // Fetch X0 coordinate
            while (temp != ',')
            {
                curX0 = (curX0 * 10) + charToInt(temp);
                temp = getc(inputFile);
            }

            temp = getc(inputFile);

            // Fetch Y0 coordinate
            while (temp != ',')
            {
                curY0 = (curY0 * 10) + charToInt(temp);
                temp = getc(inputFile);
            }

            temp = getc(inputFile);

            // Fetch X1 coordinate
            while (temp != ',')
            {
                curX1 = (curX1 * 10) + charToInt(temp);
                temp = getc(inputFile);
            }

            temp = getc(inputFile);

            // Fetch Y1 coordinate
            while (temp != ',')
            {
                curY1 = (curY1 * 10) + charToInt(temp);
                temp = getc(inputFile);
            }

            temp = getc(inputFile);

            // Get the hexcode color
            while ((temp != '\n') && (temp != EOF))
            {
                curY0 = (curY0 * 10) + charToInt(temp);
                temp = getc(inputFile);
            }

            // Draw that line
            drawLine(fb, curX0, curY0, curX1, curY1, WHITE);

            if (temp != EOF)
            {
                temp = getc(inputFile);
            }
        }
    }
    fclose(inputFile);

    // Pause
    scanf("%c", &temp);

    clear(&fb);

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}