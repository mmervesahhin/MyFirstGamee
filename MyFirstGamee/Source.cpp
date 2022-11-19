/***********
 Merve Þahin
************/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD    500 // Period for the timer.
#define TIMER_PERIOD2   16 
#define TIMER_PERIOD3   100 
#define TIMER_PERIOD4   16 
#define TIMER_PERIODr0   40
#define TIMER_PERIODr1   25
#define TIMER_PERIODr2   16
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define TIMER_ON2        1 
#define TIMER_ON3        1 
#define TIMER_ON4        1 
#define TIMER_ONr0        1 
#define TIMER_ONr1        1 
#define TIMER_ONr2        1 

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

bool blink = false;
bool ufo = false;
int xUFO, yUFO;
int xAl = -600, yAl = 0;
bool mode = true; //False means autonomous, true means manuel
bool start = false;
int direction;
int S1 = 1, S2 = 1; //Variables for different stars
int xS1 = 20, xS2 = 600, yS1 = 300, yS2 = 300; //Variables for different stars
int xRoc0 = -600, yRoc0 = 220, xRoc1 = -600, yRoc1 = 40, xRoc2 = -600, yRoc2 = -140; //Rockets coordinates

float dist0, dist1, dist2, distNew0, distNew1, distNew2; //Variables to check if the UFO hit any rocket or not
bool hit = false; //true if the ufo hits any of the rockets
bool heart0 = true; //false if you lose your first heart
bool heart1 = true; //false if you lose your second heart
bool heart2 = true; //false if you lose your third heart
bool check = true;

int score = -1; //the number of times you collect the item
int xItem, yItem; //coordinates of the item
bool over = false; //boolean to start the game again

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}

void message()
{
    glColor3f(0.5, 1, 0.1);
    vprint(-590, -270, GLUT_BITMAP_TIMES_ROMAN_24, "Use                      to control the UFO and try to collect the items!");
    glColor3f(1, 0, 0);
    vprint(-590, -270, GLUT_BITMAP_TIMES_ROMAN_24, "        Arrow Keys                 ");
    glColor3f(1, 1, 0);
    vprint(170, -270, GLUT_BITMAP_TIMES_ROMAN_24, "Your Score: %d", score);
}

void background()
{
    if (blink)
    {
        glBegin(GL_LINES);

        int k, i;

        for (k = -550; k <= 550; k += 100)
            for (i = 250; i >= -250; i -= 100)
            {
                glColor3f(0.6, 0.6, 0.6);

                glVertex2f(k, i);
                glVertex2f(k + 8, i - 8);

                glVertex2f(k + 8, i);
                glVertex2f(k, i - 8);

                glColor3f(0.3, 0.3, 0.3);

                glVertex2f(k + 4, i);
                glVertex2f(k + 4, i - 8);

                glVertex2f(k, i - 4);
                glVertex2f(k + 8, i - 4);

                glColor3f(0.6, 0.6, 0.6);

                glVertex2f(k + 50, i - 50);
                glVertex2f(k + 50 + 8, i - 50 - 8);

                glVertex2f(k + 50 + 8, i - 50);
                glVertex2f(k + 50, i - 50 - 8);

                glColor3f(0.3, 0.3, 0.3);

                glVertex2f(k + 50 + 4, i - 50);
                glVertex2f(k + 50 + 4, i - 50 - 8);

                glVertex2f(k + 50, i - 50 - 4);
                glVertex2f(k + 50 + 8, i - 50 - 4);
            }

        glEnd();
    }

    else
    {
        glBegin(GL_LINES);

        int k, i;

        for (k = -575; k <= 575; k += 150)
            for (i = 275; i >= -275; i -= 125)
            {
                glColor3f(0.6, 0.6, 0.6);

                glVertex2f(k, i);
                glVertex2f(k + 8, i - 8);

                glVertex2f(k + 8, i);
                glVertex2f(k, i - 8);

                glColor3f(0.3, 0.3, 0.3);

                glVertex2f(k + 4, i);
                glVertex2f(k + 4, i - 8);

                glVertex2f(k, i - 4);
                glVertex2f(k + 8, i - 4);

                glColor3f(0.6, 0.6, 0.6);

                glVertex2f(k + 50, i - 50);
                glVertex2f(k + 50 + 8, i - 50 - 8);

                glVertex2f(k + 50 + 8, i - 50);
                glVertex2f(k + 50, i - 50 - 8);

                glColor3f(0.3, 0.3, 0.3);

                glVertex2f(k + 50 + 4, i - 50);
                glVertex2f(k + 50 + 4, i - 50 - 8);

                glVertex2f(k + 50, i - 50 - 4);
                glVertex2f(k + 50 + 8, i - 50 - 4);
            }

        glEnd();
    }

    glColor3f(0.3, 0.3, 0);

    glBegin(GL_POLYGON);

    glVertex2f(xS1 - 1, yS1);
    glVertex2f(xS1 + 10, yS1 + 27);
    glVertex2f(xS1 + 21, yS1);
    glVertex2f(xS1 + 50, yS1);
    glVertex2f(xS1 + 27, yS1 - 23);
    glVertex2f(xS1 + 35, yS1 - 55);
    glVertex2f(xS1 + 11, yS1 - 35);
    glVertex2f(xS1 - 15, yS1 - 55);
    glVertex2f(xS1 - 5, yS1 - 25);
    glVertex2f(xS1 - 25, yS1);
    glVertex2f(xS1, yS1);

    glEnd();

    glBegin(GL_POLYGON);

    glVertex2f(xS2, yS2 - 1);
    glVertex2f(xS2 + 9, yS2 + 21);
    glVertex2f(xS2 + 18, yS2 - 1);
    glVertex2f(xS2 + 44, yS2 - 1);
    glVertex2f(xS2 + 22, yS2 - 17);
    glVertex2f(xS2 + 29, yS2 - 50);
    glVertex2f(xS2 + 9, yS2 - 32);
    glVertex2f(xS2 - 10, yS2 - 50);
    glVertex2f(xS2, yS2 - 19);
    glVertex2f(xS2 - 23, yS2 - 1);
    glVertex2f(xS2, yS2 - 1);

    glEnd();
}

void firstUfo()
{

    glColor3f(1, 0, 0);
    circle(xUFO, yUFO, 60);

    glBegin(GL_TRIANGLES);
    glVertex2f(xUFO - 30, yUFO + 40);
    glVertex2f(xUFO + 30, yUFO + 40);
    glVertex2f(xUFO, yUFO + 100);
    glEnd();

    glColor3f(1, 1, 1);
    circle(xUFO, yUFO + 40, 45);

    glColor3f(1, 1, 1);
    circle(xUFO - 7, yUFO + 40, 4);
    circle(xUFO + 7, yUFO + 40, 4);
    circle(xUFO, yUFO + 50, 4);

    circle(xUFO, yUFO + 100, 5);

    glBegin(GL_LINES);
    glVertex2f(xUFO - 7, yUFO + 30);
    glVertex2f(xUFO + 7, yUFO + 30);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO - 50, yUFO - 80);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO, yUFO - 90);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO + 50, yUFO - 80);
    glEnd();

    glColor3f(0, 0, 0);
    circle(xUFO - 35, yUFO - 15, 10);
    circle(xUFO, yUFO - 35, 10);
    circle(xUFO + 35, yUFO - 15, 10);

    glColor3f(1, 0, 0);
    circle(xUFO, yUFO - 90, 5);
    circle(xUFO + 50, yUFO - 80, 5);
    circle(xUFO - 50, yUFO - 80, 5);
}

void alien()
{
    glColor3f(0, 1, 0);
    circle(xAl, yAl + 40, 20);
    glRectf(xAl - 12, yAl + 40, xAl + 12, yAl - 4.6);
    glBegin(GL_LINES);
    glVertex2f(xAl, yAl + 30);
    glVertex2f(xAl - 25, yAl + 10);
    glVertex2f(xAl, yAl + 30);
    glVertex2f(xAl + 25, yAl + 10);
    glEnd();
    glRectf(xAl - 12, yAl - 5, xAl - 5, yAl - 20);
    glRectf(xAl + 12, yAl - 5, xAl + 5, yAl - 20);

    glColor3f(0, 0, 0);
    circle(xAl - 7, yAl + 40, 2);
    circle(xAl + 7, yAl + 40, 2);
    circle(xAl, yAl + 50, 2);
    glRectf(xAl - 5, yAl + 30, xAl + 5, yAl + 32);

    glColor3f(1, 0, 0);
    circle(xAl, yAl + 7, 3);
}

void gamer()
{

    glColor3f(1, 0, 0);
    circle(xUFO, yUFO, 60);

    glBegin(GL_TRIANGLES);
    glVertex2f(xUFO - 30, yUFO + 40);
    glVertex2f(xUFO + 30, yUFO + 40);
    glVertex2f(xUFO, yUFO + 100);
    glEnd();

    glColor3f(1, 1, 1);
    circle(xUFO, yUFO + 40, 45);

    glColor3f(1, 1, 1);
    circle(xUFO - 7, yUFO + 40, 4);
    circle(xUFO + 7, yUFO + 40, 4);
    circle(xUFO, yUFO + 50, 4);

    circle(xUFO, yUFO + 100, 5);

    glBegin(GL_LINES);
    glVertex2f(xUFO - 7, yUFO + 30);
    glVertex2f(xUFO + 7, yUFO + 30);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO - 50, yUFO - 80);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO, yUFO - 90);
    glVertex2f(xUFO, yUFO - 10);
    glVertex2f(xUFO + 50, yUFO - 80);
    glEnd();

    glColor3f(0, 0, 0);
    circle(xUFO - 35, yUFO - 15, 10);
    circle(xUFO, yUFO - 35, 10);
    circle(xUFO + 35, yUFO - 15, 10);

    glColor3f(1, 0, 0);
    circle(xUFO, yUFO - 90, 5);
    circle(xUFO + 50, yUFO - 80, 5);
    circle(xUFO - 50, yUFO - 80, 5);

    glColor3f(0, 1, 0);
    circle(xUFO, yUFO + 40, 20);
    glRectf(xUFO - 12, yUFO + 40, xUFO + 12, yUFO - 4.6);
    glBegin(GL_LINES);
    glVertex2f(xUFO, yUFO + 30);
    glVertex2f(xUFO - 25, yUFO + 10);
    glVertex2f(xUFO, yUFO + 30);
    glVertex2f(xUFO + 25, yUFO + 10);
    glEnd();

    glColor3f(0, 0, 0);
    circle(xUFO - 7, yUFO + 40, 2);
    circle(xUFO + 7, yUFO + 40, 2);
    circle(xUFO, yUFO + 50, 2);
    glRectf(xUFO - 5, yUFO + 30, xUFO + 5, yUFO + 32);

    glColor3f(1, 0, 0);
    circle(xUFO, yUFO + 7, 3);
}

void rocket0()
{
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0, yRoc0);
    glVertex2f(xRoc0 - 50, yRoc0 + 40);
    glVertex2f(xRoc0 - 50, yRoc0 - 40);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glRectf(xRoc0 - 50, yRoc0 + 40, xRoc0 - 200, yRoc0 - 40);

    glColor3f(0, 0, 0);
    glRectf(xRoc0 - 195, yRoc0 + 40, xRoc0 - 200, yRoc0 - 40);
    glRectf(xRoc0 - 185, yRoc0 + 40, xRoc0 - 190, yRoc0 - 40);
    glRectf(xRoc0 - 50, yRoc0 + 40, xRoc0 - 55, yRoc0 - 40);

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0 - 200, yRoc0 + 35);
    glVertex2f(xRoc0 - 200, yRoc0 - 35);
    glVertex2f(xRoc0 - 240, yRoc0);
    glEnd();

    glColor3f(1, 0.5, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0 - 200, yRoc0 + 30);
    glVertex2f(xRoc0 - 200, yRoc0 - 30);
    glVertex2f(xRoc0 - 230, yRoc0);
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0 - 200, yRoc0 + 25);
    glVertex2f(xRoc0 - 200, yRoc0 - 25);
    glVertex2f(xRoc0 - 220, yRoc0);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0 - 200, yRoc0 + 75);
    glVertex2f(xRoc0 - 200, yRoc0 + 40);
    glVertex2f(xRoc0 - 175, yRoc0 + 40);
    glVertex2f(xRoc0 - 200, yRoc0 - 75);
    glVertex2f(xRoc0 - 200, yRoc0 - 40);
    glVertex2f(xRoc0 - 175, yRoc0 - 40);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc0 - 100, yRoc0 + 15);
    glVertex2f(xRoc0 - 100, yRoc0 - 15);
    glVertex2f(xRoc0 - 85, yRoc0);
    glVertex2f(xRoc0 - 100, yRoc0 + 15);
    glVertex2f(xRoc0 - 100, yRoc0 - 15);
    glVertex2f(xRoc0 - 115, yRoc0);
    glEnd();
}

void rocket1()
{
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1, yRoc1);
    glVertex2f(xRoc1 - 50, yRoc1 + 40);
    glVertex2f(xRoc1 - 50, yRoc1 - 40);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glRectf(xRoc1 - 50, yRoc1 + 40, xRoc1 - 200, yRoc1 - 40);

    glColor3f(0, 0, 0);
    glRectf(xRoc1 - 195, yRoc1 + 40, xRoc1 - 200, yRoc1 - 40);
    glRectf(xRoc1 - 185, yRoc1 + 40, xRoc1 - 190, yRoc1 - 40);
    glRectf(xRoc1 - 50, yRoc1 + 40, xRoc1 - 55, yRoc1 - 40);

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1 - 200, yRoc1 + 35);
    glVertex2f(xRoc1 - 200, yRoc1 - 35);
    glVertex2f(xRoc1 - 240, yRoc1);
    glEnd();

    glColor3f(1, 0.5, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1 - 200, yRoc1 + 30);
    glVertex2f(xRoc1 - 200, yRoc1 - 30);
    glVertex2f(xRoc1 - 230, yRoc1);
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1 - 200, yRoc1 + 25);
    glVertex2f(xRoc1 - 200, yRoc1 - 25);
    glVertex2f(xRoc1 - 220, yRoc1);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1 - 200, yRoc1 + 75);
    glVertex2f(xRoc1 - 200, yRoc1 + 40);
    glVertex2f(xRoc1 - 175, yRoc1 + 40);
    glVertex2f(xRoc1 - 200, yRoc1 - 75);
    glVertex2f(xRoc1 - 200, yRoc1 - 40);
    glVertex2f(xRoc1 - 175, yRoc1 - 40);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc1 - 100, yRoc1 + 15);
    glVertex2f(xRoc1 - 100, yRoc1 - 15);
    glVertex2f(xRoc1 - 85, yRoc1);
    glVertex2f(xRoc1 - 100, yRoc1 + 15);
    glVertex2f(xRoc1 - 100, yRoc1 - 15);
    glVertex2f(xRoc1 - 115, yRoc1);
    glEnd();
}

void rocket2()
{
    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2, yRoc2);
    glVertex2f(xRoc2 - 50, yRoc2 + 40);
    glVertex2f(xRoc2 - 50, yRoc2 - 40);
    glEnd();

    glColor3f(0.9, 0.9, 0.9);
    glRectf(xRoc2 - 50, yRoc2 + 40, xRoc2 - 200, yRoc2 - 40);

    glColor3f(0, 0, 0);
    glRectf(xRoc2 - 195, yRoc2 + 40, xRoc2 - 200, yRoc2 - 40);
    glRectf(xRoc2 - 185, yRoc2 + 40, xRoc2 - 190, yRoc2 - 40);
    glRectf(xRoc2 - 50, yRoc2 + 40, xRoc2 - 55, yRoc2 - 40);

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2 - 200, yRoc2 + 35);
    glVertex2f(xRoc2 - 200, yRoc2 - 35);
    glVertex2f(xRoc2 - 240, yRoc2);
    glEnd();

    glColor3f(1, 0.5, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2 - 200, yRoc2 + 30);
    glVertex2f(xRoc2 - 200, yRoc2 - 30);
    glVertex2f(xRoc2 - 230, yRoc2);
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2 - 200, yRoc2 + 25);
    glVertex2f(xRoc2 - 200, yRoc2 - 25);
    glVertex2f(xRoc2 - 220, yRoc2);
    glEnd();

    glColor3f(1, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2 - 200, yRoc2 + 75);
    glVertex2f(xRoc2 - 200, yRoc2 + 40);
    glVertex2f(xRoc2 - 175, yRoc2 + 40);
    glVertex2f(xRoc2 - 200, yRoc2 - 75);
    glVertex2f(xRoc2 - 200, yRoc2 - 40);
    glVertex2f(xRoc2 - 175, yRoc2 - 40);
    glEnd();

    glColor3f(0, 0, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(xRoc2 - 100, yRoc2 + 15);
    glVertex2f(xRoc2 - 100, yRoc2 - 15);
    glVertex2f(xRoc2 - 85, yRoc2);
    glVertex2f(xRoc2 - 100, yRoc2 + 15);
    glVertex2f(xRoc2 - 100, yRoc2 - 15);
    glVertex2f(xRoc2 - 115, yRoc2);
    glEnd();
}

void Heart0()
{
    glColor3f(1, 0, 0);
    circle(440, -240, 10);
    circle(460, -240, 10);
    glBegin(GL_TRIANGLES);
    glVertex2f(429, -240);
    glVertex2f(471, -240);
    glVertex2f(450, -280);
    glEnd();
}

void Heart1()
{
    glColor3f(1, 0, 0);
    circle(490, -240, 10);
    circle(510, -240, 10);
    glBegin(GL_TRIANGLES);
    glVertex2f(479, -240);
    glVertex2f(521, -240);
    glVertex2f(500, -280);
    glEnd();
}

void Heart2()
{
    glColor3f(1, 0, 0);
    circle(540, -240, 10);
    circle(560, -240, 10);
    glBegin(GL_TRIANGLES);
    glVertex2f(529, -240);
    glVertex2f(571, -240);
    glVertex2f(550, -280);
    glEnd();
}

void gameOver()
{
    glColor3f(0, 0, 0.14);
    glBegin(GL_QUADS);
    glVertex2f(-1000, 1000);
    glVertex2f(1000, 1000);
    glVertex2f(1000, -1000);
    glVertex2f(-1000, -1000);
    glVertex2f(-1000, 1000);
    glEnd();

    glColor3f(1, 1, 1);
    vprint(-100, 0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER   :(");

    glColor3f(1, 0, 0);
    vprint(-75, -50, GLUT_BITMAP_HELVETICA_18, "Your Score: %d", score);

    glColor3f(0, 1, 0);
    vprint(-163, -270, GLUT_BITMAP_HELVETICA_18, "Use                  to Start the Game Again");
    glColor3f(1, 0, 0);
    vprint(-125, -270, GLUT_BITMAP_HELVETICA_18, "Spacebar     ");

    over = true;
}

void item()
{
    glColor3f(1, 1, 0);
    circle(xItem, yItem, 20);

    glBegin(GL_TRIANGLES);
    glVertex2f(xItem - 15, yItem);
    glVertex2f(xItem + 15, yItem);
    glVertex2f(xItem, yItem + 30);

    glVertex2f(xItem, yItem + 15);
    glVertex2f(xItem, yItem - 15);
    glVertex2f(xItem + 30, yItem);

    glVertex2f(xItem - 15, yItem);
    glVertex2f(xItem + 15, yItem);
    glVertex2f(xItem, yItem - 30);

    glVertex2f(xItem, yItem + 15);
    glVertex2f(xItem, yItem - 15);
    glVertex2f(xItem - 30, yItem);

    glVertex2f(xItem - 20, yItem);
    glVertex2f(xItem, yItem - 20);
    glVertex2f(xItem + 25, yItem + 20);

    glVertex2f(xItem - 20, yItem);
    glVertex2f(xItem, yItem + 20);
    glVertex2f(xItem + 25, yItem - 20);

    glVertex2f(xItem + 20, yItem);
    glVertex2f(xItem, yItem + 20);
    glVertex2f(xItem - 25, yItem - 20);

    glVertex2f(xItem + 20, yItem);
    glVertex2f(xItem, yItem - 20);
    glVertex2f(xItem - 25, yItem + 20);
    glEnd();

    glColor3f(0, 0, 0);
    circle_wire(xItem, yItem, 20);
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0.14, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    background();

    if (!ufo)
    {
        glColor3f(1, 0, 0);
        vprint(-150, 0, GLUT_BITMAP_TIMES_ROMAN_24, "Click");

        glColor3f(0.5, 1, 0.1);
        vprint(-90, 0, GLUT_BITMAP_TIMES_ROMAN_24, "to create the UFO");
    }

    if (ufo)
    {
        if (!start) {

            firstUfo();

            glColor3f(0.5, 1, 0.1);
            vprint(-590, -270, GLUT_BITMAP_TIMES_ROMAN_24, "Wait for                 to get into the UFO");
            glColor3f(1, 0, 0);
            vprint(-550, -270, GLUT_BITMAP_TIMES_ROMAN_24, "        the Alien                 ");

            alien();
        }

        if (start)
        {
            item();

            gamer();

            message();

            rocket0();
            rocket1();
            rocket2();

            if (heart0)
                Heart0();

            if (heart1)
                Heart1();

            if (heart2)
                Heart2();
            else
            {
                gameOver();
                over = true;
            }

        }
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (over)
        if (key == ' ')
        {
            start = false;
            ufo = false;
            over = false;
            score = 0;
            heart0 = true;
            heart1 = true;
            heart2 = true;
            xAl = -600;
            yAl = 0;
            direction = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    if (start) {
        switch (key) {
        case GLUT_KEY_UP:
            up = true;
            direction = 1;
            if (yUFO + 105 < WINDOW_HEIGHT / 2)
                yUFO += 5;
            break;
        case GLUT_KEY_DOWN:
            down = true;
            direction = 2;
            if (yUFO - 95 > -(WINDOW_HEIGHT / 2))
                yUFO -= 5;
            break;
        case GLUT_KEY_LEFT:
            left = true;
            direction = 3;
            if (xUFO - 60 > -(WINDOW_WIDTH / 2))
                xUFO -= 5;
            break;
        case GLUT_KEY_RIGHT:
            right = true;
            direction = 4;
            if (xUFO + 60 < WINDOW_WIDTH / 2)
                xUFO += 5;
            break;
        }
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//

void onClick(int button, int state, int x, int y)
{
    // Write your codes here.

    if (!ufo) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {
            ufo = true; //UFO appears at the start point

            xUFO = 525;
            yUFO = 0;

        }
    }
    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//Timer for the blinking background
#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    blink = !blink;

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif


#if TIMER_ON2 == 1
void onTimer2(int m) {

    glutTimerFunc(TIMER_PERIOD2, onTimer2, 0);
    // Write your codes here.

    dist0 = sqrtf(powf(xUFO - xRoc0, 2) + powf(yUFO - yRoc0, 2));
    dist1 = sqrtf(powf(xUFO - xRoc1, 2) + powf(yUFO - yRoc1, 2));
    dist2 = sqrtf(powf(xUFO - xRoc2, 2) + powf(yUFO - yRoc2, 2));

    distNew0 = sqrtf(powf(xUFO - xRoc0, 2) + powf(yUFO + 40 - yRoc0, 2));
    distNew1 = sqrtf(powf(xUFO - xRoc1, 2) + powf(yUFO + 40 - yRoc1, 2));
    distNew2 = sqrtf(powf(xUFO - xRoc2, 2) + powf(yUFO + 40 - yRoc2, 2));

    srand(time(NULL));

    if (yItem >= yUFO + 40)
    {
        if (sqrtf(powf(xUFO - xItem, 2) + powf(yUFO + 40 - yItem, 2)) <= 65)
        {
            if (check) {
                xItem = rand() % 1081 - 540;
                yItem = rand() % 486 - 205;
                check = false;
            }
            else {
                yItem = rand() % 486 - 205;
                xItem = rand() % 1081 - 540;
                check = true;
            }
            score++;
        }
    }

    else
        if (sqrtf(powf(xUFO - xItem, 2) + powf(yUFO - yItem, 2)) <= 80)
        {
            if (check) {
               xItem = rand() % 1081 - 540;
               yItem = rand() % 486 - 205;  
               check = false;
            }
            else {
                yItem = rand() % 486 - 205;
                xItem = rand() % 1081 - 540;
                check = true;
            }
            score++;
        }

    if (!mode)
    {
        switch (direction)
        {
        case 0:
            break;
        case 1:
            if (yUFO + 105 < WINDOW_HEIGHT / 2)
                yUFO += 5;
            else
                direction = 2;
            break;
        case 2:
            if (yUFO - 95 > -(WINDOW_HEIGHT / 2))
                yUFO -= 5;
            else
                direction = 1;
            break;
        case 3:
            if (xUFO - 60 > -(WINDOW_WIDTH / 2))
                xUFO -= 5;
            else
                direction = 4;
            break;
        case 4:
            if (xUFO + 60 < WINDOW_WIDTH / 2)
                xUFO += 5;
            else
                direction = 3;
            break;
        }

        /***********************************/

        //1st area

        if (yUFO + 40 >= yRoc0 && dist0 <= 60)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 >= yRoc1 && dist1 <= 60)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 >= yRoc2 && dist2 <= 60)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        //2nd area

        if (yUFO + 40 < yRoc0 && distNew0 <= 45)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 < yRoc1 && distNew1 <= 45)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 < yRoc0 && distNew2 <= 45)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        //3rd area

        if (xUFO <= xRoc0 && xUFO >= xRoc0 - 200 && yUFO <= yRoc0 + 100 && yUFO > yRoc0)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }


        if (xUFO <= xRoc1 && xUFO >= xRoc1 - 200 && yUFO <= yRoc1 + 100 && yUFO > yRoc1)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }


        if (xUFO <= xRoc2 && xUFO >= xRoc2 - 200 && yUFO <= yRoc2 + 100 && yUFO > yRoc2)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        //4th area

        if (xUFO <= xRoc0 && xUFO >= xRoc0 - 200 && yUFO >= yRoc0 - 125 && yUFO < yRoc0)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (xUFO <= xRoc1 && xUFO >= xRoc1 - 200 && yUFO >= yRoc1 - 125 && yUFO < yRoc1)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (xUFO <= xRoc2 - 50 && xUFO >= xRoc2 - 200 && yUFO >= yRoc2 - 125 && yUFO < yRoc2)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        //5th area

        if (yUFO + 40 >= yRoc0 - 40 && yUFO - 60 <= yRoc0 + 40 && xUFO + 60 == xRoc0 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 >= yRoc1 - 40 && yUFO - 60 <= yRoc1 + 40 && xUFO + 60 == xRoc1 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 >= yRoc2 - 40 && yUFO - 60 <= yRoc2 + 40 && xUFO + 60 == xRoc2 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        //6th area

        if (yUFO + 40 < yRoc0 - 40 && yUFO + 85 >= yRoc0 - 40 && xUFO + 45 == xRoc0 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 < yRoc1 - 40 && yUFO + 85 >= yRoc1 - 40 && xUFO + 45 == xRoc1 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        if (yUFO + 40 < yRoc2 - 40 && yUFO + 85 >= yRoc2 - 40 && xUFO + 45 == xRoc2 - 200)
        {
            hit = true;

            if (heart0)
                heart0 = false;
            else if (heart1)
                heart1 = false;
            else if (heart2)
                heart2 = false;

            xUFO = 530;
            yUFO = 0;
            xRoc0 = -600;
            yRoc0 = 220;
            xRoc1 = -600;
            yRoc1 = 40;
            xRoc2 = -600;
            yRoc2 = -140;
        }

        hit = false;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

//Timer for the stars
#if TIMER_ON3 == 1
void onTimer3(int n) {

    glutTimerFunc(TIMER_PERIOD3, onTimer3, 0);
    // Write your codes here.

    if (S1 == 1)
    {
        xS1 -= 1;
        yS1 -= 1;
        if (yS1 + 27 == -300)
        {
            yS1 = 355;
            xS1 = 20;
        }
    }

    if (S2 == 1)
    {
        xS2 -= 3;
        yS2 -= 3;
        if (yS2 + 27 == -300)
        {
            yS2 = 300;
            xS2 = 650;
        }
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

//Timer for the alien 
#if TIMER_ON4 == 1
void onTimer4(int k) {

    glutTimerFunc(TIMER_PERIOD4, onTimer4, 0);
    // Write your codes here.

    if (ufo) {
        if (xAl < xUFO - 85)
            xAl += 5;
        else
        {
            start = true;
            mode = false;
        }
    }
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

//Timer for the first rocket
#if TIMER_ONr0 == 1
void onTimerr0(int w) {

    glutTimerFunc(TIMER_PERIODr0, onTimerr0, 0);
    // Write your codes here.

    if (start)
    {
        if (xRoc0 <= 840)
            xRoc0 += 5;
        else
            xRoc0 = -600;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

//Timer for the second rocket
#if TIMER_ONr1 == 1
void onTimerr1(int o) {

    glutTimerFunc(TIMER_PERIODr1, onTimerr1, 0);
    // Write your codes here.

    if (start) {
        if (xRoc1 <= 840)
            xRoc1 += 5;
        else
            xRoc1 = -600;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

//Timer for the third rocket
#if TIMER_ONr2 == 1
void onTimerr2(int j) {

    glutTimerFunc(TIMER_PERIODr2, onTimerr2, 0);
    // Write your codes here.

    if (start) {

        if (xRoc2 <= 840)
            xRoc2 += 5;
        else
            xRoc2 = -600;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("UFO in Space");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

#if  TIMER_ON2 == 1
    // timer event 2
    glutTimerFunc(TIMER_PERIOD2, onTimer2, 0);
#endif

#if  TIMER_ON3 == 1
    // timer event 3
    glutTimerFunc(TIMER_PERIOD3, onTimer3, 0);
#endif

#if  TIMER_ON4 == 1
    // timer event 3
    glutTimerFunc(TIMER_PERIOD4, onTimer4, 0);
#endif

#if  TIMER_ONr0 == 1
    // timer event 3
    glutTimerFunc(TIMER_PERIODr0, onTimerr0, 0);
#endif

#if  TIMER_ONr1 == 1
    // timer event 3
    glutTimerFunc(TIMER_PERIODr1, onTimerr1, 0);
#endif

#if  TIMER_ONr2 == 1
    // timer event 3
    glutTimerFunc(TIMER_PERIODr2, onTimerr2, 0);
#endif

    Init();

    glutMainLoop();
}