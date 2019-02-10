#include "framebuffer.h"
#include "line.h"
#include "polygon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define FILENAME "test.txt"

// https://www.geeksforgeeks.org/time-delay-c/
void delay(int ms)
{
    // Stroing start time
    clock_t start_time = clock();
    long micro = ms * 1000;

    // looping till required time is not acheived
    while (clock() < start_time + micro)
        ;
}

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
int seekToEOL(FILE * file)
{
    char c = getc(file);
    while ((c != '\n') && (c != EOF))
    {
        c = getc(file);
    }
    return c;
}

// Reads one entry of polygon
Polygon readPolygon(FILE * file)
{
    int x, y;
    uint32_t rgb;
    Polygon poly;
    initPolygon(&poly);

    char temp = getc(file);
    if (temp == '|') {
        temp = getc(file);
    }

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
        addPointPolygon(&poly,x,y,rgb);

        if (temp != '|') {
            temp = getc(file);
        }
    }
    // Finishing drawing the polygon
    // drawPolygon(fb,poly);

    return poly;
}

// Reads one entry of circle
Circle readCircle(FILE * file)
{
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

    return makeCircle(xc,yc,r,rgb);
}

// Reads one entry of line
Line readLine(FILE * file)
{
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

    return makeLine(x1,y1,x2,y2,rgb);
}

void animateAllObject(FBUFFER *fb, Line* listOfLine, Circle* listOfCircle, Polygon* listOfPolygon)
{
    int tankSpeed = 5;
    int planeSpeed = 10;
    int bulletSpeed = 25;
    int frames = 200;
    int bulletDelay1 = 25;
    int bulletDelay2 = 5;
    while (frames > 0) {
        // Tank 1 -> Polygon 1 - 3, Circle 1 - 4, Line 1-5
        for (int i = 0; i < 3; i++){
            drawPolygon(fb, listOfPolygon[i]);
            drawCircleObject(fb, listOfCircle[i]);
        }
        // Plane -> Polygon 7 - 11; Bullet -> Circle 9, Line 11 - 13
        for (int i = 6; i < 11; i++) {
            drawPolygon(fb, listOfPolygon[i]);
        }
        // Bullet delay for tank
        if (bulletDelay1 <= 0) {
            drawCircleObject(fb, listOfCircle[3]);
        } else {
            bulletDelay1--;
        }
        // Bullet delay for plane
        if (bulletDelay2 <= 0) {
            drawCircleObject(fb, listOfCircle[8]);
        } else {
            bulletDelay2--;
        }

        delay(33);
        swapBuffer(fb);

        // Move tank
        for (int i = 0; i < 3; i++){
            movePolygon(&listOfPolygon[i], tankSpeed, 0);
            moveCircle(&listOfCircle[i], tankSpeed, 0);
        }
        // Move plane
        for (int i = 6; i < 11; i++) {
            movePolygon(&listOfPolygon[i], -planeSpeed, 0);
        }
        // Move tank bullet
        if (bulletDelay1 <= 0) {
            moveCircle(&listOfCircle[3], bulletSpeed, 0);
        } else {
            moveCircle(&listOfCircle[3], tankSpeed, 0);
        }
        // Move plane bullet
        if (bulletDelay2 <= 0) {
            moveCircle(&listOfCircle[8], 0, bulletSpeed);
        } else {
            moveCircle(&listOfCircle[8], -planeSpeed, 0);
        }
        frames--;
    };
}

void draw(FBUFFER *fb, Line *listOfLine, Circle* listOfCircle, Polygon *listOfPolygon, int numOfLine, int numOfCircle, int numOfPolygon)
{
    int i;
    for (i = 0; i < numOfPolygon; i++) {
        drawPolygon(fb,listOfPolygon[i]);
    }
    for (i = 0; i < numOfCircle; i++) {
        drawCircleObject(fb,listOfCircle[i]);
    }
    for (i = 0; i < numOfLine; i++) {
        drawLineObject(fb,listOfLine[i]);
    }
    swapBuffer(fb);
}

int main()
{
    char temp;
    FBUFFER fb;
    Polygon * listOfPolygon;
    Circle * listOfCircle;
    Line * listOfLine;
    int numOfPolygon, numOfCircle, numOfLine;
    uint32_t rgb;

    listOfPolygon = (Polygon *) malloc(sizeof(Polygon));
    numOfPolygon = 0;

    listOfCircle = (Circle *) malloc(sizeof(Circle));
    numOfCircle = 0;

    listOfLine = (Line *) malloc(sizeof(Line));
    numOfLine = 0;

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
                listOfPolygon[numOfPolygon] = readPolygon(inputFile);
                numOfPolygon++;
                listOfPolygon = (Polygon *) realloc (listOfPolygon, (numOfPolygon + 1) * sizeof(Polygon));
                temp = getc(inputFile);
            }
            // Circles
            else if ((temp == 'C') || (temp == 'c')) {
                listOfCircle[numOfCircle] = readCircle(inputFile);
                numOfCircle++;
                listOfCircle = (Circle *) realloc (listOfCircle, (numOfCircle + 1) * sizeof(Circle));
                temp = getc(inputFile);
            }
            // Lines
            else if ((temp == 'L') || (temp == 'l')) {
                listOfLine[numOfLine] = readLine(inputFile);
                numOfLine++;
                listOfLine = (Line *) realloc (listOfLine, (numOfLine + 1) * sizeof(Line));
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

    // Draw all Polygon, Circle, and Line
    // draw(&fb, listOfLine, listOfCircle, listOfPolygon, numOfLine, numOfCircle, numOfPolygon);

    animateAllObject(&fb, listOfLine, listOfCircle, listOfPolygon);

    // Pause
    //scanf("%c", &temp);

    // Turn the cursor back on
    system("setterm -cursor on");

    clear(&fb);
    swapBuffer(&fb);
    destroy(&fb);

    return 0;
}
