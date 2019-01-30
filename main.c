#include "framebuffer.h"
#include "line.h"
// #include "polygon.h" //read README.md for compile
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "test.txt"

int charToInt(char n)
{
    return ((int)(n - '0'));
}

int singleHexToInt(char n)
{
    if (n == 'a' || n == 'A')
    {
        return 10;
    }
    else if (n == 'b' || n == 'B')
    {
        return 11;
    }
    else if (n == 'c' || n == 'C')
    {
        return 12;
    }
    else if (n == 'd' || n == 'D')
    {
        return 13;
    }
    else if (n == 'e' || n == 'E')
    {
        return 14;
    }
    else if (n == 'f' || n == 'F')
    {
        return 15;
    }
    else
    {
        return charToInt(n);
    }
}

int main()
{
    char temp;
    FBUFFER fb;
    char intBuffer[5];
    int curX0, curY0, curX1, curY1;
    uint32_t rgb;

    init(&fb);

    printf("Width : %d\nHeight : %d\n", fb.xres, fb.yres);
    scanf("%c", &temp);

    // Turn off the cursor
    system("setterm -cursor off");

    clear(&fb);

    // Open file
    // FILE *inputFile = fopen(FILENAME, "r");
    // temp = getc(inputFile);
    // while (temp != EOF)
    // {
    //     while ((temp != '\n') && (temp != EOF))
    //     {
    //         curX0 = 0, curY0 = 0, curX1 = 0, curY1 = 0;
    //         rgb = 0;

    //         // Fetch X0 coordinate
    //         while (temp != ',')
    //         {
    //             curX0 = (curX0 * 10) + charToInt(temp);
    //             temp = getc(inputFile);
    //         }

    //         temp = getc(inputFile);

    //         // Fetch Y0 coordinate
    //         while (temp != ',')
    //         {
    //             curY0 = (curY0 * 10) + charToInt(temp);
    //             temp = getc(inputFile);
    //         }

    //         temp = getc(inputFile);

    //         // Fetch X1 coordinate
    //         while (temp != ',')
    //         {
    //             curX1 = (curX1 * 10) + charToInt(temp);
    //             temp = getc(inputFile);
    //         }

    //         temp = getc(inputFile);

    //         // Fetch Y1 coordinate
    //         while (temp != ',')
    //         {
    //             curY1 = (curY1 * 10) + charToInt(temp);
    //             temp = getc(inputFile);
    //         }

    //         temp = getc(inputFile);

    //         // Fetch RGB hex value
    //         while ((temp != '\n') && (temp != EOF))
    //         {
    //             rgb = (rgb << 4) + singleHexToInt(temp);
    //             temp = getc(inputFile);
    //         }

    //         // Draw that line
    //         drawLine(fb, curX0, curY0, curX1, curY1, rgb);

    //         if (temp != EOF)
    //         {
    //             temp = getc(inputFile);
    //         }
    //     }
    // }
    // fclose(inputFile);

    //drawOctagon, drawSquare, drawHome, drawTree, & drawTriangle bisa di delete hanya membantu menentukan titik dan menulis ke file
    // drawOctagon(fb, 550, 100, 30, YELLOW); //draw Sun
    // drawTriangle(fb, 400, 0, 500, BLUE); //draw Mountain

    // Draw Home and trees
    // drawHome(fb, 705, 330);
    // drawHome(fb, 825, 330);
    // drawTree(fb, 100, 350, 30, 80);

    drawLine(fb, 100, 100, 200, 150, WHITE);
    drawLine(fb, 200, 150, 200, 200, WHITE);
    drawLine(fb, 200, 200, 100, 300, WHITE);
    drawLine(fb, 100, 300, 0, 200, WHITE);
    drawLine(fb, 0, 200, 0, 150, WHITE);
    drawLine(fb, 0, 150, 100, 100, WHITE);

    // Pause
    scanf("%c", &temp);

    clear(&fb);

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}
