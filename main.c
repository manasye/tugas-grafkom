#include "framebuffer.h"
#include "line.h"
#include "polygon.h"
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

// Moves file pointer towards end-of-line (EOL)
// It will also stop on EOF, so check on it's return value
int seekToEOL(FILE * file) {
    char c = getc(file);
    while ((c != '\n') && (c != EOF))
    {
        c = getc(file);
    }
    return c;
}

// Reads one entry of polygon
int readPolygon(FILE * file, FBUFFER * fb) {
    int x, y;
    uint32_t rgb;

    char temp = getc(file);
    if (temp == '|') {
        temp = getc(file);
    }
    drawStart(fb);
    while (temp != '|') {
        x = 0;
        y = 0;
        rgb = 0;
        // Read X
        while (temp != ',') {
            x = (x * 10) + charToInt(temp);
            temp = getc(file);
        }
        temp = getc(file);
        // Read Y
        while (temp != ',') {
            y = (y * 10) + charToInt(temp);
            temp = getc(file);
        }
        temp = getc(file);
        // Read hex color
        while ((temp != ';') && (temp != '|')) {
            rgb = (rgb * 16) + singleHexToInt(temp);
            temp = getc(file);
        }
        // Add point
        drawAddPoint(x,y,rgb);

        if (temp != '|') {
            temp = getc(file);
        }
    }
    // Finishing drawing the polygon
    drawEnd();

    return 0;
}

// Reads one entry of circle
int readCircle(FILE * file, FBUFFER * fb) {
    char temp = getc(file);
    if (temp == '|') {
        temp = getc(file);
    }
    int xc = 0;
    int yc = 0;
    int r = 0;
    uint32_t rgb = 0;
    // Read X center point
    while (temp != ',') {
        xc = (xc * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read P2
    while (temp != ',') {
        yc = (yc * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read radius
    while (temp != ',') {
        r = (r * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read hex color
    while (temp != '|') {
        rgb = (rgb * 16) + singleHexToInt(temp);
        temp = getc(file);
    }
    // Draw the circle
    drawCircle(*fb,xc,yc,r,rgb);

    return 0;
}

// Reads one entry of line
int readLine(FILE * file, FBUFFER * fb) {
    char temp = getc(file);
    if (temp == '|') {
        temp = getc(file);
    }
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    uint32_t rgb = 0;
    // Read X1
    while (temp != ',') {
        x1 = (x1 * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read Y1
    while (temp != ',') {
        y1 = (y1 * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read X2
    while (temp != ',') {
        x2 = (x2 * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read Y2
    while (temp != ',') {
        y2 = (y2 * 10) + charToInt(temp);
        temp = getc(file);
    }
    temp = getc(file);
    // Read hexcolor
    while (temp != '|') {
        rgb = (rgb * 16) + singleHexToInt(temp);
        temp = getc(file);
    }
    // Draw the line
    drawLine(*fb,x1,y1,x2,y2,rgb);
    return 0;
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
    swapBuffer(&fb);

    // Open file
    FILE *inputFile = fopen(FILENAME, "r");
    temp = getc(inputFile);

    while (temp != EOF)
    {
        while ((temp != '\n') && (temp != EOF))
        {
            // Comments
            if (temp == '#') {
                temp = seekToEOL(inputFile);  
            }
            // Polygons 
            else if ((temp == 'P') || (temp == 'p')) {
                readPolygon(inputFile,&fb);
                temp = getc(inputFile);
            } 
            // Circles
            else if ((temp == 'C') || (temp == 'c')) {
                readCircle(inputFile,&fb);
                temp = getc(inputFile);
            } 
            // Lines
            else if ((temp == 'L') || (temp == 'l')) {
                readLine(inputFile, &fb);
                temp = getc(inputFile);
            }
            // Other, just advance the file pointer
            else {
                temp = getc(inputFile);
            }
            
        }

        if (temp != EOF) {
            temp = getc(inputFile);
        }
    }
    fclose(inputFile);
    swapBuffer(&fb);

    // Pause
    scanf("%c", &temp);

    clear(&fb);
    swapBuffer(&fb);
    destroy(&fb);

    // Turn the cursor back on
    system("setterm -cursor on");
    return 0;
}
